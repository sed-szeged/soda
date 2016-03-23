/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: Tamás Gergely <gertom@inf.u-szeged.hu>
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
#include <stdexcept>
#include "GeneralIgnorePrioritizationPlugin.h"

namespace soda {

bool operator<(GeneralIgnorePrioritizationPlugin::qelement d1, GeneralIgnorePrioritizationPlugin::qelement d2) {
    return d1.priorityValue < d2.priorityValue;
}

GeneralIgnorePrioritizationPlugin::GeneralIgnorePrioritizationPlugin():
        m_data(NULL),
        m_nofElementsReady(0),
        m_elementsReady(NULL),
        m_priorityQueue(new std::vector<qelement>())
{}

GeneralIgnorePrioritizationPlugin::~GeneralIgnorePrioritizationPlugin()
{
    delete m_priorityQueue;
    delete m_elementsReady;
    m_elementsReady = NULL;
    m_nofElementsReady = 0;
}

String GeneralIgnorePrioritizationPlugin::getName()
{
    return "general-ignore";
}

String GeneralIgnorePrioritizationPlugin::getDescription()
{
    return "GeneralIgnorePrioritization plugin is based on coverage information.";
}

void GeneralIgnorePrioritizationPlugin::init(CSelectionData *data, CKernel *kernel)
{
    m_data = data;
    IntVector initial;
    setState(initial);
}

void GeneralIgnorePrioritizationPlugin::setState(IntVector &ordered)
{
    delete m_elementsReady;
    m_elementsReady = new IntVector(ordered);
    m_nofElementsReady = ordered.size();
    m_priorityQueue->clear();
    prioritize();
}

void GeneralIgnorePrioritizationPlugin::reset(RevNumType)
{
    // revision information are not taken into account
    return;
}

void GeneralIgnorePrioritizationPlugin::fillSelection(IntVector& selected, size_t size)
{
    /*for (; m_nofElementsReady < size && !(m_priorityQueue->empty()); m_nofElementsReady++) {
        m_elementsReady->push_back((m_priorityQueue->back()).testcaseId);
        m_priorityQueue->pop_back();
    }*/
    while (m_nofElementsReady < size && !(m_priorityQueue->empty())) {
        next();
    }

    selected.clear();
    for (size_t i = 0; i < size && i < m_nofElementsReady; i++) {
        selected.push_back((*m_elementsReady)[i]);
    }
}

IndexType GeneralIgnorePrioritizationPlugin::next()
{
    if (m_priorityQueue->empty()) {
        throw std::out_of_range("There are not any testcases left.");
    }
    IndexType tcid = (m_priorityQueue->back()).testcaseId;
    m_elementsReady->push_back(tcid);
    m_nofElementsReady++;
    m_priorityQueue->pop_back();
    return tcid;
}

void GeneralIgnorePrioritizationPlugin::prioritize()
{
    IndexType nofTestcases = m_data->getCoverage()->getNumOfTestcases();
    const IBitMatrix& coverageBitMatrix = m_data->getCoverage()->getBitMatrix();
    for(IndexType tcid = 0; tcid < nofTestcases; tcid++) {
        if (std::find(m_elementsReady->begin(), m_elementsReady->end(), tcid) != m_elementsReady->end()) {
            continue;
        }
        qelement d;
        d.testcaseId = tcid;
        d.priorityValue = coverageBitMatrix[tcid].count();
        m_priorityQueue->push_back(d);
    }

    sort(m_priorityQueue->begin(), m_priorityQueue->end());
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuitePrioritizationPluginManager().addPlugin(new GeneralIgnorePrioritizationPlugin());
}

} /* namespace soda */
