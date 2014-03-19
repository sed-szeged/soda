/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors:
 *
 * This file is part of SoDA.
 *
 *  SoDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SoDA is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with SoDA.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <cmath>
#include <time.h>

#include "CReductionPluginManager.h"
#include "DuplationReductionPlugin.h"
#include "CReductionData.h"

namespace soda {

DuplationReductionPlugin::DuplationReductionPlugin()
{
}

String DuplationReductionPlugin::getName()
{
    return "duplation";
}

String DuplationReductionPlugin::getDescription()
{
    return "DuplationReductionPlugin is based on duplation.";
}

void DuplationReductionPlugin::init(CSelectionData *data, String programName, String dirPath, unsigned int iterationLimit)
{
    m_data = data;
    m_programName = programName;
    m_dirPath = dirPath;
    m_nrOfCodeElements = data->getCoverage()->getNumOfCodeElements();
    m_nrOfTestCases = data->getCoverage()->getNumOfTestcases();
    m_iterationLimit = iterationLimit;
}

void DuplationReductionPlugin::reduction(std::ofstream &outStream)
{
    duplationReduction(outStream);
}

void DuplationReductionPlugin::duplationReduction(std::ofstream &outStream)
{
    std::cerr << "[INFO] Duplation reduction (iteration limit: " << m_iterationLimit << ") ... ";

    std::set<IndexType> Trem;  // remaining test set
    std::set<IndexType> Tv;    // actual test set
    std::set<IndexType> Trand; // random test set
    std::vector<IndexType> randomizedTests; // stores the shuffled test case id-s

    // print initial test suite
    outStream << "Number of procedures: " << m_nrOfCodeElements << std::endl;

    // initialize the remaining set
    std::set<IndexType>::iterator it = Trem.begin();
    for(unsigned int i = 0; i < m_nrOfTestCases; i++) {
            it = Trem.insert(it, i);
            randomizedTests.push_back(i);
    }

    unsigned int fullSize = Trem.size();
    shuffle(randomizedTests); // make randomized order

    // print initial test suite
    outStream << "Total " << fullSize << " test cases in full test suite." << std::endl;

    Tv.clear();
    Trand.clear();

    CReductionData reducedMatrix(m_data->getCoverage(), m_programName, m_dirPath);
    CReductionData randomMatrix(m_data->getCoverage(), m_programName + "-RANDOM", m_dirPath);

    /* ----- algo start ----- */

    std::vector<unsigned int> R(m_nrOfCodeElements,1); // stores the classification of methods: in each iteration there are 2^i different values
    std::vector<unsigned int> RCount; // counter for R for finding optimal division: in each iteration its size is 2^i and stores the counts for each category
    std::map<IndexType,std::vector<unsigned int> >  divCount;  // stores 2^i counts for each test case
    unsigned int iter = 0;
    unsigned int numClasses = 1;

    while (iter < m_iterationLimit) {
        std::cerr << "[DUPLATION] Iteration " << iter+1 << " (number of classes: " << numClasses << ") " << std::endl;
        divCount.clear();

        if (iter == 0) { // first iteration: fill count vectors and R based on existing coverage count data
            IndexType selectedTest = *Trem.begin();
            double coverOptimal = m_nrOfCodeElements / 2.0;
            unsigned int coverActual;
            unsigned int coverBest = m_nrOfCodeElements * 2 + 1; // sufficiently large
            for (it = Trem.begin(); it != Trem.end(); it++) {
                coverActual = m_data->getCoverage()->getBitMatrix().getRow(*it).count();
                if (std::abs(coverOptimal - coverActual) < std::abs(coverOptimal - coverBest)) {
                    coverBest = coverActual;
                    selectedTest = *it;
                }
            }

            // mark test case:
            Tv.insert(selectedTest);
            Trem.erase(selectedTest);
            // in parallel, fill the random matrix:
            if (addRandom(randomizedTests, Trand, 1) != 1) // oops
                std::cerr << "[DUPLATION] random test addition failed (adding 1 test)" << std::endl;
            // update R:
            RCount.clear();
            RCount.resize(numClasses * 2, 0); // for the next iteration
            for (size_t i = 0; i < m_nrOfCodeElements; i++) {
                R[i] = m_data->getCoverage()->getBitMatrix().getRow(selectedTest)[i] ? R[i]*2 : R[i]*2-1;
                RCount[R[i] - 1]++;
            }

            outStream << "Selected test in the 1st iteration: " << selectedTest << "; " << std::endl;
            // add it to the new global matrix:
            reducedMatrix.add(Tv);
            reducedMatrix.save(1);
            // in parallel, creat the random matrix as well:
            randomMatrix.add(Trand);
            randomMatrix.save(1);
        } else { // all other iterations: fill count vectors and R based on the matrix
            std::vector<IndexType> selectedTestVec(numClasses,0);
            std::vector<unsigned int> coverBestVec(numClasses,m_nrOfCodeElements*2+1); // sufficiently large init value
            std::set<IndexType> testOccupied; // not to select the best test case for multiple classes
            for (it = Trem.begin(); it != Trem.end(); it++) {
                IBitList& actRow = m_data->getCoverage()->getBitMatrix().getRow(*it);
                divCount[*it] = std::vector<unsigned int>(numClasses, 0);
                for (IndexType i = 0; i < m_nrOfCodeElements; i++)
                    if (actRow.at(i))
                        divCount[*it][R[i] - 1]++;
            }

            unsigned int maxc = numClasses - 1;
            for (size_t c = 0; c < numClasses; c++) {
                bool haveIt = false;
                for (it = Trem.begin(); it != Trem.end(); it++) {
                    if (std::abs(RCount[c]/2.0 - divCount[*it][c]) < std::abs(RCount[c]/2.0 - coverBestVec[c])
                            && testOccupied.find(*it)==testOccupied.end() ) { // we want exactly i^2 different tests to be selected
                        coverBestVec[c] = divCount[*it][c];
                        selectedTestVec[c] = *it;
                        haveIt=true;
                    }
                }
                if (!haveIt) { // could not add test case for this class -> finished
                    maxc = c - 1;
                    break;
                }
                if (!testOccupied.insert(selectedTestVec[c]).second) // oops
                    std::cerr << "[DUPLATION] twice occupied test OCCUP: [" << c << "] " << selectedTestVec[c] << std::endl;
                if (Tv.find(selectedTestVec[c]) != Tv.end()) // oops
                    std::cerr << "[DUPLATION] twice occupied test TV: [" << c << "] " << selectedTestVec[c] << std::endl;
            }

            // mark test cases:
            for (size_t c = 0; c <= maxc; c++) {
                Tv.insert(selectedTestVec[c]);
                Trem.erase(selectedTestVec[c]);
                if (Trem.empty()) {
                    maxc = c;
                    break;
                }
            }

            // in parallel, fill the random matrix:
            if (addRandom(randomizedTests, Trand, maxc + 1) != (int)maxc + 1) // oops
                std::cerr << "[DUPLATION] random test addition failed (adding " << maxc+1 << " tests)" << std::endl;

            // update R:
            RCount.clear();
            RCount.resize(numClasses * 2, 0); // for the next iteration
            for (size_t i = 0; i < m_nrOfCodeElements ; i++) {
                R[i] = m_data->getCoverage()->getBitMatrix().getRow(selectedTestVec[R[i]-1])[i] ? R[i]*2 : R[i]*2-1;
                RCount[R[i]-1]++;
            }
            outStream << "Selected tests in the " << iter+1 << "th iteration: " << std::endl;
            outStream << maxc + 1 << " tests added" << std::endl;
            // add it to the new global matrix:
            reducedMatrix.add(Tv);
            reducedMatrix.save(iter+1);
            // in parallel, creat the random matrix as well:
            randomMatrix.add(Trand);
            randomMatrix.save(iter+1);
        }

        numClasses *= 2;
        iter++;
        if (Tv.size() >= m_nrOfTestCases) {
            std::cerr << "[DUPLATION] Full test suite size reached, exiting before iteration " << iter+1 << " ..." << std::endl;
            break;
        }
    }

    /* ----- algo end ----- */

    unsigned int reducedSize=Tv.size();
    outStream << "Duplation reduction ended" << std::endl;

    // print final test suite:
    outStream << "Total " << reducedSize << " test cases in reduced test suite." << std::endl;
    outStream << "Reduction rate: " << 100.0 * (fullSize - reducedSize) / fullSize << "\%" << std::endl;

    std::cerr << "done." << std::endl;
}

void DuplationReductionPlugin::shuffle(std::vector<IndexType>& v)
{
    srand(time(0));
    double norm = ((double)v.size() - 1) / RAND_MAX;
    size_t i2;
    IndexType t;
    for (size_t i = 0; i < v.size(); i++) {
        i2 = (size_t)(rand() * norm);
        if (i2 < 0 || i2 > v.size() - 1) // oops
            std::cerr << "[COVERAGE] wrong random index: " << i2 << std::endl;
        t = v[i];
        v[i] = v[i2];
        v[i2] = t;
    }
}

int DuplationReductionPlugin::addRandom(std::vector<IndexType>& from, std::set<IndexType>& to, unsigned int N)
{
    size_t origsize = from.size();
    if (N > origsize)
        return -1;

    for (size_t i = origsize - N; i < origsize; i++) {
        if (!to.insert(from[i]).second) { // already in, should not happen
            return 0;
        }
    }

    from.erase(from.begin() + origsize - N, from.end());
    return origsize - from.size();
}

extern "C" void registerPlugin(CReductionPluginManager &manager)
{
    manager.addReductionPlugin(new DuplationReductionPlugin());
}

} /* namespace soda */

