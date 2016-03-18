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

#include "AdditionalGeneralIgnorePrioritizationPlugin.h"

namespace soda {

bool operator<(AdditionalGeneralIgnorePrioritizationPlugin::qelement d1, AdditionalGeneralIgnorePrioritizationPlugin::qelement d2) {
    return d1.priorityValue < d2.priorityValue;
}

AdditionalGeneralIgnorePrioritizationPlugin::AdditionalGeneralIgnorePrioritizationPlugin():
        m_data(NULL),
        m_nofElementsReady(0),
        m_elementsReady(NULL),
        m_priorityQueue(new std::vector<qelement>()),
        m_notCoveredCEIDs(new std::list<IndexType>())
{}

AdditionalGeneralIgnorePrioritizationPlugin::~AdditionalGeneralIgnorePrioritizationPlugin()
{
    delete m_priorityQueue;
    delete m_elementsReady;
    delete m_notCoveredCEIDs;
    m_elementsReady = NULL;
    m_nofElementsReady = 0;
}

String AdditionalGeneralIgnorePrioritizationPlugin::getName()
{
    return "additional-general-ignore";
}

String AdditionalGeneralIgnorePrioritizationPlugin::getDescription()
{
    return "GeneralIgnorePrioritization plugin is based on coverage information.";
}

void AdditionalGeneralIgnorePrioritizationPlugin::init(CSelectionData *data)
{
    m_data = data;
    IntVector initial;
    setState(initial);
}

void AdditionalGeneralIgnorePrioritizationPlugin::setState(IntVector &ordered)
{
    delete m_elementsReady;
    m_elementsReady = new IntVector(ordered);
    m_priorityQueue->clear();
    m_nofElementsReady = ordered.size();
    m_notCoveredCEIDs->clear();


    IndexType nofTestcases = m_data->getCoverage()->getNumOfTestcases();
    IndexType nofCodeElements = m_data->getCoverage()->getNumOfCodeElements();
    const IBitMatrix& coverageBitMatrix = m_data->getCoverage()->getBitMatrix();

    // Initialize the not covered code element list
    for (IndexType ceid = 0; ceid < nofCodeElements; ceid++) {
        m_notCoveredCEIDs->push_back(ceid);
    }

    // Fill the priority queue with the remaining tests
    for (IndexType tcid = 0; tcid < nofTestcases; tcid++) {
        if (std::find(m_elementsReady->begin(), m_elementsReady->end(), tcid) != m_elementsReady->end()) {
            continue;
        }
        qelement d;
        d.testcaseId    = tcid;
        d.priorityValue = coverageBitMatrix.getRow(tcid).count();
        m_priorityQueue->push_back(d);
    }

    // Update the prioritization values based on the received test list
    for (IntVector::iterator it = ordered.begin(); it != ordered.end(); it++) {
        updateData(*it);
    }

    return;
}

void AdditionalGeneralIgnorePrioritizationPlugin::reset(RevNumType rev)
{
    //revision information are not taken into account
    return;
}

void AdditionalGeneralIgnorePrioritizationPlugin::fillSelection(IntVector& selected, size_t size)
{
    for (; m_nofElementsReady < size && !(m_priorityQueue->empty()); m_nofElementsReady++) {
        sort(m_priorityQueue->begin(), m_priorityQueue->end());
        qelement nxt = m_priorityQueue->back();
        m_priorityQueue->pop_back();
        m_elementsReady->push_back(nxt.testcaseId);
        if (nxt.priorityValue == 0) {
            continue;
        }

        updateData(nxt.testcaseId);
    }

    selected.clear();
    for (size_t i = 0; i < size && i < m_nofElementsReady; i++) {
        selected.push_back((*m_elementsReady)[i]);
    }
}

void AdditionalGeneralIgnorePrioritizationPlugin::updateData(IndexType tcid)
{
    const IBitMatrix& coverageBitMatrix = m_data->getCoverage()->getBitMatrix();
    for (std::list<IndexType>::iterator ceit = m_notCoveredCEIDs->begin(); ceit != m_notCoveredCEIDs->end();) {
        if (coverageBitMatrix[tcid][*ceit]) {
            register IndexType ceid = *ceit;
            m_notCoveredCEIDs->erase(ceit++);
            for (std::vector<qelement>::iterator qi = m_priorityQueue->begin(); qi != m_priorityQueue->end(); ++qi) {
                if (coverageBitMatrix[qi->testcaseId][ceid]) {
                    qi->priorityValue--;
                }
            }
        } else {
            ++ceit;
        }
    }
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuitePrioritizationPluginManager().addPlugin(new AdditionalGeneralIgnorePrioritizationPlugin());
}

} /* namespace soda */
