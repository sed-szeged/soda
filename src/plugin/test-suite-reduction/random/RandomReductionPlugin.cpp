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

#include "RandomReductionPlugin.h"
#include "data/CReductionData.h"

namespace soda {

RandomReductionPlugin::RandomReductionPlugin() :
    m_data(NULL)
{
}

String RandomReductionPlugin::getName()
{
    return "random";
}

String RandomReductionPlugin::getDescription()
{
    return "RandomReductionPlugin is based on randomized test cases.";
}

void RandomReductionPlugin::init(CSelectionData *data, CJsonReader &reader)
{
    m_data = data;
    m_programName = reader.getStringFromProperty("program-name");
    m_dirPath = reader.getStringFromProperty("output-dir");
    m_iterationLimit = reader.getIntFromProperty("iteration");
    m_reductionSizes = reader.getIntVectorFromProperty("reduction-sizes");
    m_nrOfCodeElements = data->getCoverage()->getNumOfCodeElements();
    m_nrOfTestCases = data->getCoverage()->getNumOfTestcases();
}

void RandomReductionPlugin::reduction(std::ofstream &outStream)
{
    randomReduction(outStream);
}

void RandomReductionPlugin::randomReduction(std::ofstream &outStream)
{
    std::cerr << "[INFO] Random reduction (iteration limit: " << m_iterationLimit << ") ... " << std::endl;

    std::set<IndexType> Trand; // random test set
    std::set<IndexType> TrandIter; // random test set
    std::vector<IndexType> randomizedTests; // stores the shuffled test case id-s
    std::vector<IndexType> randomizedIterTests;

    // print initial test suite
    outStream << "Number of procedures: " << m_nrOfCodeElements << std::endl;

    // initialize the remaining set
    for(unsigned int i = 0; i < m_nrOfTestCases; i++) {
            randomizedTests.push_back(i);
    }

    unsigned int fullSize = randomizedTests.size();
    shuffle(randomizedTests); // make randomized order
    randomizedIterTests = randomizedTests;

    // print initial test suite
    outStream << "Total " << fullSize << " test cases in full test suite." << std::endl;

    Trand.clear();
    TrandIter.clear();

    CReductionData randomIterMatrix(m_data->getCoverage(), m_programName + "-RAND-ITER", m_dirPath);
    CReductionData randomMatrix(m_data->getCoverage(), m_programName + "-RAND", m_dirPath);

    unsigned int iter = 0;
    unsigned int numClasses = 1;

    while (iter < m_iterationLimit) {
        std::cerr << "[RANDOM] Iteration " << iter + 1 << " (number of classes: " << numClasses << ") " << std::endl;

        // fill the random matrix
        if (addRandom(randomizedIterTests, TrandIter, numClasses) != (int)numClasses) { // oops
            std::cerr << "[RANDOM] random test addition failed (adding 1 test)" << std::endl;
            numClasses *= 2;
            iter++;
            continue;
        }

        outStream << "Selected tests in the " << iter + 1 << "th iteration: " << std::endl;
        outStream << numClasses << " tests added" << std::endl;

        // create the random matrix
        randomIterMatrix.add(TrandIter);
        randomIterMatrix.save(iter + 1);
        numClasses *= 2;
        iter++;
        if (TrandIter.size() >= m_nrOfTestCases) {
            std::cerr << "[RANDOM] Full test suite size reached, exiting before iteration " << iter + 1 << " ..." << std::endl;
            break;
        }
    }

    for (int i = 0; i < (int)m_reductionSizes.size(); ++i) {
        int reductionSize = m_reductionSizes[i];
        std::cerr << "[RANDOM] Fixed size " << i << " (number of classes: " << reductionSize << ") " << std::endl;

        // fill the random matrix
        if (addRandom(randomizedTests, Trand, reductionSize) != (int)reductionSize) {// oops
            std::cerr << "[RANDOM] random test addition failed (adding 1 test)" << std::endl;
            continue;
        }

        // create the random matrix
        randomMatrix.add(Trand);
        randomMatrix.save(i + 1);

        if (Trand.size() >= m_nrOfTestCases) {
            std::cerr << "[RANDOM] Full test suite size reached, exiting before iteration " << iter + 1 << " ..." << std::endl;
            break;
        }
    }

    unsigned int reducedSize = TrandIter.size();
    outStream << "Random reduction ended" << std::endl;

    // print final test suite:
    outStream << "Fixed iteration reduction" << std::endl;
    outStream << "Total " << reducedSize << " test cases in reduced test suite." << std::endl;
    outStream << "Reduction rate: " << 100.0 * (fullSize - reducedSize) / fullSize << "\%" << std::endl;

    reducedSize = Trand.size();
    outStream << "Fixed size reduction" << std::endl;
    outStream << "Total " << reducedSize << " test cases in reduced test suite." << std::endl;
    outStream << "Reduction rate: " << 100.0 * (fullSize - reducedSize) / fullSize << "\%" << std::endl;

    std::cerr << "done." << std::endl;
}

void RandomReductionPlugin::shuffle(std::vector<IndexType>& v)
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

int RandomReductionPlugin::addRandom(std::vector<IndexType>& from, std::set<IndexType>& to, unsigned int N)
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

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteReductionPluginManager().addPlugin(new RandomReductionPlugin());
}

} /* namespace soda */

