/*
 * Copyright (C): 2013-2016 Department of Software Engineering, University of Szeged
 *
 * Authors: David Tengeri <dtengeri@inf.u-szeged>
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
#include <chrono>
#include <stdexcept>
#include "RaptorPrioritizationPlugin.h"

namespace soda {

bool operator<(RaptorPrioritizationPlugin::qelement d1, RaptorPrioritizationPlugin::qelement d2) {
    if (d1.priorityValue == d2.priorityValue) {
        return d1.testcaseId > d2.testcaseId;
    }
    return d1.priorityValue < d2.priorityValue;
}

RaptorPrioritizationPlugin::RaptorPrioritizationPlugin():
        m_data(NULL),
        m_nofElementsReady(0),
        m_elementsReady(NULL),
        m_elementsRemaining(NULL),
        m_currentCluster(new CClusterDefinition()),
        m_currentAmbiguity(0.0),
        m_priorityQueue(new std::vector<qelement>()),
        m_recursionLevel(0)
{}

RaptorPrioritizationPlugin::~RaptorPrioritizationPlugin()
{
    delete m_priorityQueue;
    delete m_elementsReady;
    m_elementsReady = NULL;
    m_nofElementsReady = 0;
}

String RaptorPrioritizationPlugin::getName()
{
    return "raptor";
}

String RaptorPrioritizationPlugin::getDescription()
{
    return "RAPTOR algorithm by Gonzalez-Sanchez, Abreu, Gross and Gemund.";
}

void RaptorPrioritizationPlugin::init(CSelectionData *data, CKernel *kernel)
{
    m_data = data;
    m_currentCluster->addCodeElements(m_data->getCodeElements()->getIDList());
    IntVector initial;
    setState(initial);
}

void RaptorPrioritizationPlugin::setState(IntVector &ordered)
{
    if (m_elementsReady != NULL) {
        delete m_elementsReady;
    }
    if (m_elementsRemaining != NULL) {
        delete m_elementsRemaining;
    }

    m_elementsReady = new IntVector(ordered);
    m_elementsRemaining = new IntVector();
    IndexType nofTestcases = m_data->getCoverage()->getNumOfTestcases();
    for(IndexType tcid = 0; tcid < nofTestcases; tcid++) {
        if (std::find(m_elementsReady->begin(), m_elementsReady->end(), tcid) == m_elementsReady->end()) {
            m_elementsRemaining->push_back(tcid);
        }
    }

    m_currentCluster->clearTestCases();
    m_currentCluster->addTestCases(ordered);

    m_nofElementsReady = ordered.size();
    m_priorityQueue->clear();

    IndexType nofCodeElements = m_data->getCoverage()->getNumOfCodeElements();
    m_currentAmbiguity = (double) (nofCodeElements - 1) / 2.0;
    m_recursionLevel = 0;
}

void RaptorPrioritizationPlugin::reset(RevNumType revision)
{
    // revision information are not taken into account
    return;
}

void RaptorPrioritizationPlugin::fillSelection(IntVector& selected, size_t size)
{
    while (m_nofElementsReady < size && !(m_elementsRemaining->empty())) {
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        next();
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( end - start ).count();
        std::cerr << "[INFO][Raptor] Selected " << m_nofElementsReady << "/"
                  << ((size > m_data->getCoverage()->getNumOfTestcases()) ? m_data->getCoverage()->getNumOfTestcases() : size)
                  << " in " << duration << " microseconds."
                  << std::endl;
    }

    selected.clear();
    for (size_t i = 0; i < size && i < m_nofElementsReady; i++) {
        selected.push_back((*m_elementsReady)[i]);
    }
}

IndexType RaptorPrioritizationPlugin::next()
{
    if (m_elementsRemaining->empty()) {
        throw std::out_of_range("There are not any testcases left.");
    }

    IndexType tcid;

    prioritize();
    qelement d = m_priorityQueue->back();
    if (d.priorityValue > 0) {
        m_priorityQueue->pop_back();
        m_currentCluster->addTestCase(d.testcaseId);
        m_currentAmbiguity = d.priorityValue;
        tcid = d.testcaseId;
        m_recursionLevel = 0;
    } else if (m_recursionLevel == 0) {
        // Reset the internal state and prioritize again
        m_currentCluster->clearTestCases();
        IndexType nofCodeElements = m_data->getCoverage()->getNumOfCodeElements();
        m_currentAmbiguity = (double) (nofCodeElements - 1) / 2.0;
        // Call recursively
        // std::cerr << "[RAPTOR] Recursive call" << std::endl;
        m_recursionLevel++;
        tcid = next();
    } else {
        tcid = d.testcaseId;
    }

    // Remove the test from the remaining list
    IntVector::iterator pos = std::find(m_elementsRemaining->begin(), m_elementsRemaining->end(), tcid);
    if (pos != m_elementsRemaining->end()) {
        m_elementsRemaining->erase(pos);
        m_elementsReady->push_back(tcid);
        m_nofElementsReady++;
    }

    return tcid;
}

void RaptorPrioritizationPlugin::prioritize()
{
    m_priorityQueue->clear();

    for (IndexType i = 0; i < m_elementsRemaining->size(); i++) {
        qelement d;
        d.testcaseId = m_elementsRemaining->at(i);
        d.priorityValue = ambiguityReduction(m_elementsRemaining->at(i));
        m_priorityQueue->push_back(d);
    }

    sort(m_priorityQueue->begin(), m_priorityQueue->end());
}

double RaptorPrioritizationPlugin::ambiguityReduction(IndexType tcid)
{
    m_currentCluster->addTestCase(tcid);
    CPartitionAlgorithm algorithm;
    algorithm.compute(*m_data, *m_currentCluster);
    double extended = ambiguity(algorithm);
    m_currentCluster->removeTestCase(tcid);
    // std::cerr << "[RAPTOR] " << "Ambiguity reduction for: tcid(" << tcid << ") = " << m_currentAmbiguity << " - " << extended << std::endl;
    return m_currentAmbiguity - extended;
}

double RaptorPrioritizationPlugin::ambiguity(CPartitionAlgorithm &partition)
{
    IndexType nofCodeElements = m_data->getCoverage()->getNumOfCodeElements();
    double ambiguity = 0.0;
    CPartitionAlgorithm::PartitionData &partitionData = partition.getPartitions();
    for (CPartitionAlgorithm::PartitionData::iterator partIt = partitionData.begin(); partIt != partitionData.end(); partIt++) {
        IndexType size = partIt->second.size();
        //std::cerr << "[Raptor] " << "Size partition(" << partIt->first << "): " << size << std::endl;
        ambiguity += ((double)size / nofCodeElements) * (((double)size - 1.0) / 2.0);
    }
    return ambiguity;

}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuitePrioritizationPluginManager().addPlugin(new RaptorPrioritizationPlugin());
}

} /* namespace soda */
