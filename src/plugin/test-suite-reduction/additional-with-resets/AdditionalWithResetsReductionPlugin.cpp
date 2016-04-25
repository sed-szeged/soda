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
#include <list>

#include "AdditionalWithResetsReductionPlugin.h"
#include "data/CReductionData.h"
#include "data/CBitList.h"

using namespace std;

namespace soda {

bool operator<(AdditionalWithResetsReductionPlugin::qelement d1, AdditionalWithResetsReductionPlugin::qelement d2)
{
    return d1.priorityValue < d2.priorityValue;
}

AdditionalWithResetsReductionPlugin::AdditionalWithResetsReductionPlugin() :
    m_data(NULL),
    m_notCoveredCEIDs(new std::list<IndexType>()),
    m_priorityQueue(new std::vector<qelement>())
{}

AdditionalWithResetsReductionPlugin::~AdditionalWithResetsReductionPlugin()
{
    delete m_notCoveredCEIDs;
    delete m_priorityQueue;
}

String AdditionalWithResetsReductionPlugin::getName()
{
    return "additional-with-resets";
}

String AdditionalWithResetsReductionPlugin::getDescription()
{
    return "AdditionalWithResetsReductionPlugin is based on coverage information.";
}

void AdditionalWithResetsReductionPlugin::init(CSelectionData *data, CJsonReader &reader)
{
    m_data = data;
    m_programName = reader.getStringFromProperty("program-name");
    m_dirPath = reader.getStringFromProperty("output-dir");
    if (reader.existsProperty("covered-ce-goal")) {
        coveredCEGoal = reader.getIntFromProperty("covered-ce-goal");
    }
    m_nrOfCodeElements = data->getCoverage()->getNumOfCodeElements();
    m_nrOfTestCases = data->getCoverage()->getNumOfTestcases();
}

void AdditionalWithResetsReductionPlugin::reduction(std::ofstream &outStream)
{
    additionalWithResetsReduction(outStream);
}

void AdditionalWithResetsReductionPlugin::update(IndexType testcaseId)
{
    bool isSortNeeded = false;

    const IBitMatrix& coverageBitMatrix = m_data->getCoverage()->getBitMatrix();

    for (std::list<IndexType>::iterator ceit = m_notCoveredCEIDs->begin(); ceit != m_notCoveredCEIDs->end();) {
        if (coverageBitMatrix[testcaseId][*ceit]) {
            register IndexType ceid = *ceit;
            m_notCoveredCEIDs->erase(ceit++);

            for (std::vector<qelement>::iterator qi = m_priorityQueue->begin(); qi != m_priorityQueue->end(); ++qi) {
                if (coverageBitMatrix[qi->testcaseId][ceid]) {
                    qi->priorityValue--;
                    isSortNeeded |= true;
                }
            }
        } else {
            ++ceit;
        }
    }
}

void AdditionalWithResetsReductionPlugin::setState(const std::set<IndexType> &testcases)
{
    const IBitMatrix& coverageBitMatrix = m_data->getCoverage()->getBitMatrix();

    m_priorityQueue->clear();
    m_notCoveredCEIDs->clear();

    // Iterating through the testcases and setting the initial priority values.
    for (IndexType tcid = 0; tcid < m_nrOfTestCases; tcid++) {
        if (std::find(testcases.begin(), testcases.end(), tcid) == testcases.end()) {
            qelement d;
            d.testcaseId = tcid;
            d.priorityValue = coverageBitMatrix.getRow(tcid).count();
            m_priorityQueue->push_back(d);
        }
    }

    // Initializing the queue.
    std::sort(m_priorityQueue->begin(), m_priorityQueue->end());

    for (IndexType ceid = 0; ceid < m_nrOfCodeElements; ceid++) {
        m_notCoveredCEIDs->push_back(ceid);
    }
}

void AdditionalWithResetsReductionPlugin::additionalWithResetsReduction(std::ofstream &outStream)
{
    std::cerr << "[INFO] Additional-With-Resets reduction ... ";

    std::set<IndexType> T, Titer, Tce;

    // Initialization
    setState(T);

    CReductionData reducedMatrix(m_data->getCoverage(), m_programName + "-ADD-RES", m_dirPath);
    CReductionData reducedMatrix_iter(m_data->getCoverage(), m_programName + "-ADD-RES-ITER", m_dirPath);
    CReductionData reducedMatrix_ce(m_data->getCoverage(), m_programName + "-ADD-RES-CE-" + std::to_string(coveredCEGoal), m_dirPath);

    unsigned int iter = 1; // for the duplation iterations
    unsigned int itersize = 1; // for the duplation iterations
    IndexType previous = 0;

    while (!m_priorityQueue->empty()) {
        qelement top = m_priorityQueue->back();

        // Reinitialization.
        if (top.priorityValue == 0 && previous != 0) {
            setState(T);
            previous = 0;
            continue;
        }

        m_priorityQueue->pop_back();
        previous = top.priorityValue;

        T.insert(top.testcaseId);
        Titer.insert(top.testcaseId);

        if (coveredCEGoal > 0) {
            Tce.insert(top.testcaseId);

            if (top.priorityValue > coveredCEGoal) {
                coveredCEGoal = 0;
            } else {
                coveredCEGoal -= top.priorityValue;
            }
        }

        if (Titer.size() == itersize) { // new iteration size reached
            reducedMatrix_iter.add(Titer);
            reducedMatrix_iter.save(iter);
            Titer.clear();
            iter++; // 1,2,3,...
            itersize *= 2; // 1,2,4,...
        }

        if (top.priorityValue != 0) {
            update(top.testcaseId);
        }
    }

    // General method for all duplation iterations
    if (!Titer.empty()) { // there are some elements left from the last iteration but  itersize was not reached
      reducedMatrix_iter.add(Titer);
      reducedMatrix_iter.save(iter);
    }

    reducedMatrix.add(T);
    reducedMatrix.save(0);

    reducedMatrix_ce.add(Tce);
    reducedMatrix_ce.save(0);

    std::cerr << "done." << std::endl;
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteReductionPluginManager().addPlugin(new AdditionalWithResetsReductionPlugin());
}

} /* namespace soda */
