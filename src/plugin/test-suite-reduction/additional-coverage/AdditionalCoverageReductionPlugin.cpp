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

#include "AdditionalCoverageReductionPlugin.h"
#include "data/CReductionData.h"
#include "data/CBitList.h"

using namespace std;

namespace soda {


AdditionalCoverageReductionPlugin::AdditionalCoverageReductionPlugin() :
    m_data(NULL)
{
}

AdditionalCoverageReductionPlugin::~AdditionalCoverageReductionPlugin()
{
}

String AdditionalCoverageReductionPlugin::getName()
{
    return "additional-coverage";
}

String AdditionalCoverageReductionPlugin::getDescription()
{
    return "AdditionalCoverageReductionPlugin is based on coverage information.";
}

void AdditionalCoverageReductionPlugin::init(CSelectionData *data, CJsonReader &reader)
{
    m_data = data;
    m_programName = reader.getStringFromProperty("program-name");
    m_dirPath = reader.getStringFromProperty("output-dir");
    m_nrOfCodeElements = data->getCoverage()->getNumOfCodeElements();
    m_nrOfTestCases = data->getCoverage()->getNumOfTestcases();
}

void AdditionalCoverageReductionPlugin::reduction(std::ofstream &outStream)
{
    additionalCoverageReduction(outStream);
}

void AdditionalCoverageReductionPlugin::additionalCoverageReduction(std::ofstream &outStream)
{
    std::cerr << "[INFO] Additional-Coverage reduction ... ";

    const IBitMatrix& coverageBitMatrix = m_data->getCoverage()->getBitMatrix();

    std::vector<qelement>* priorityQueue = new std::vector<qelement>();
    std::list<IndexType>*  notCoveredCEIDs = new std::list<IndexType>();

    for (IndexType tcid = 0; tcid < m_nrOfTestCases; tcid++) {
            qelement d;
            d.testcaseId = tcid;
            d.priorityValue = coverageBitMatrix.getRow(tcid).count();
            priorityQueue->push_back(d);
    }

    for (IndexType ceid = 0; ceid < m_nrOfCodeElements; ceid++) {
        notCoveredCEIDs->push_back(ceid);
    }

    CReductionData reducedMatrix_gen(m_data->getCoverage(), m_programName + "-ADD-GEN", m_dirPath);
    CReductionData reducedMatrix_geniter(m_data->getCoverage(), m_programName + "-ADD-GEN-ITER", m_dirPath);

    unsigned int iter = 1; // for the Additional General strategy for all duplation iterations
    unsigned int itersize = 1; // for the Additional General strategy for all duplation iterations

    std::set<IndexType> Tgen, Tgeniter;
    IndexType previous = 0;

    while (!priorityQueue->empty()) {
        if (previous != 0) {
            std::sort(priorityQueue->begin(), priorityQueue->end(), *this);
        }

        qelement nxt = priorityQueue->back();
        priorityQueue->pop_back();

        previous = nxt.priorityValue;

        Tgen.insert(nxt.testcaseId);
        Tgeniter.insert(nxt.testcaseId);

        if (Tgeniter.size() == itersize) { // new iteration size reached
            reducedMatrix_geniter.add(Tgeniter);
            reducedMatrix_geniter.save(iter);
            Tgeniter.clear();
            iter++; // 1,2,3,...
            itersize *= 2; // 1,2,4,...
        }

        if (nxt.priorityValue == 0) {
            continue;
        }

        //UPDATEDATA
        const IBitMatrix& coverageBitMatrix = m_data->getCoverage()->getBitMatrix();
        for (std::list<IndexType>::iterator ceit = notCoveredCEIDs->begin(); ceit != notCoveredCEIDs->end();) {
            if (coverageBitMatrix[nxt.testcaseId][*ceit]) {
                register IndexType ceid = *ceit;
                notCoveredCEIDs->erase(ceit++);
                for (std::vector<qelement>::iterator qi = priorityQueue->begin(); qi != priorityQueue->end(); ++qi) {
                    if (coverageBitMatrix[qi->testcaseId][ceid]) {
                        qi->priorityValue--;
                    }
                }
            } else {
                ++ceit;
            }
        }
    }

    // General method for all duplation iterations
    if (!Tgeniter.empty()) { // there are some elements left from the last iteration but  itersize was not reached
      reducedMatrix_geniter.add(Tgeniter);
      reducedMatrix_geniter.save(iter);
    }

    reducedMatrix_gen.add(Tgen);
    reducedMatrix_gen.save(0);

    std::cerr << "done." << std::endl;

    delete priorityQueue;
    delete notCoveredCEIDs;
}

bool AdditionalCoverageReductionPlugin::operator()(qelement d1, qelement d2)
{
    return d1.priorityValue < d2.priorityValue;
}


extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteReductionPluginManager().addPlugin(new AdditionalCoverageReductionPlugin());
}

} /* namespace soda */
