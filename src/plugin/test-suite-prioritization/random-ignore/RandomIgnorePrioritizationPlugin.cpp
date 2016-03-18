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
#include <cstdlib>
#include <ctime>

#include "RandomIgnorePrioritizationPlugin.h"

namespace soda {

bool operator<(RandomIgnorePrioritizationPlugin::qelement d1, RandomIgnorePrioritizationPlugin::qelement d2) {
    return d1.priorityValue < d2.priorityValue;
}

RandomIgnorePrioritizationPlugin::RandomIgnorePrioritizationPlugin():
        m_data(NULL),
        m_nofElementsReady(0),
        m_elementsReady(new IntVector()),
        m_priorityQueue(new std::vector<qelement>())
{}

RandomIgnorePrioritizationPlugin::~RandomIgnorePrioritizationPlugin()
{
    delete m_priorityQueue;
    delete m_elementsReady;
    m_elementsReady = NULL;
    m_nofElementsReady = 0;
}

String RandomIgnorePrioritizationPlugin::getName()
{
    return "random-ignore";
}

String RandomIgnorePrioritizationPlugin::getDescription()
{
    return "RandomIgnorePrioritizationPlugin generates random priority values.";
}

void RandomIgnorePrioritizationPlugin::init(CSelectionData *data)
{
    m_data = data;
    IntVector initial;
    setState(initial);
}

void RandomIgnorePrioritizationPlugin::setState(IntVector &ordered)
{
    delete m_elementsReady;
    m_elementsReady = new IntVector(ordered);
    m_nofElementsReady = ordered.size();
    m_priorityQueue->clear();
    prioritize();
    return;
}

void RandomIgnorePrioritizationPlugin::reset(RevNumType rev)
{
    //revision information are not taken into account
    return;
}

void RandomIgnorePrioritizationPlugin::fillSelection(IntVector& selected, size_t size)
{
    for (; m_nofElementsReady < size && !(m_priorityQueue->empty()); m_nofElementsReady++) {
        m_elementsReady->push_back((m_priorityQueue->back()).testcaseId);
        m_priorityQueue->pop_back();
    }

    selected.clear();
    for (size_t i = 0; i < size && i < m_nofElementsReady; i++) {
        selected.push_back((*m_elementsReady)[i]);
    }
}

void RandomIgnorePrioritizationPlugin::prioritize()
{
    IndexType nofTestcases = m_data->getCoverage()->getNumOfTestcases();
    srand(time(NULL));
    for (IndexType tcid = 0; tcid < nofTestcases; tcid++) {
        if (std::find(m_elementsReady->begin(), m_elementsReady->end(), tcid) != m_elementsReady->end()) {
            continue;
        }
        qelement d;
        d.testcaseId    = tcid;
        d.priorityValue = rand();
        m_priorityQueue->push_back(d);
    }

    sort(m_priorityQueue->begin(), m_priorityQueue->end());
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuitePrioritizationPluginManager().addPlugin(new RandomIgnorePrioritizationPlugin());
}

} /* namespace soda */
