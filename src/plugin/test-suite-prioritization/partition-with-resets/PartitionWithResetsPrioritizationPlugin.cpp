/*
 * Copyright (C): 2013-2016 Department of Software Engineering, University of Szeged
 *
 * Authors: David Tengeri <dtengeri@inf.u-szeged.hu>
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

#include "algorithm/CPartitionAlgorithm.h"
#include "PartitionWithResetsPrioritizationPlugin.h"

namespace soda {

bool operator<(PartitionWithResetsPrioritizationPlugin::qelement d1, PartitionWithResetsPrioritizationPlugin::qelement d2) {
    if (d1.priorityValue == d2.priorityValue) {
        return d1.testcaseId > d2.testcaseId;
    }
    return d1.priorityValue < d2.priorityValue;
}

PartitionWithResetsPrioritizationPlugin::PartitionWithResetsPrioritizationPlugin():
        m_data(NULL),
        m_nofElementsReady(0),
        m_elementsReady(NULL),
        m_elementsRemaining(NULL),
        m_priorityQueue(new std::vector<qelement>()),
        m_currentCluster(new CClusterDefinition()),
        m_currentPartitionMetric(0.0),
        m_recursionLevel(0)
{}

PartitionWithResetsPrioritizationPlugin::~PartitionWithResetsPrioritizationPlugin()
{
    delete m_priorityQueue;
    delete m_elementsReady;
    delete m_elementsRemaining;
    delete m_currentCluster;
    m_elementsReady = NULL;
    m_nofElementsReady = 0;
}

String PartitionWithResetsPrioritizationPlugin::getName()
{
    return "partition-with-resets";
}

String PartitionWithResetsPrioritizationPlugin::getDescription()
{
    return "Prioritize the tests based on the best partition metric value";
}

void PartitionWithResetsPrioritizationPlugin::init(CSelectionData *data, CKernel *kernel)
{
    m_data = data;
    m_currentCluster->addCodeElements(m_data->getCodeElements()->getIDList());
    IntVector initial;
    setState(initial);
}

void PartitionWithResetsPrioritizationPlugin::setState(IntVector &ordered)
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

    m_currentPartitionMetric = 0.0;
    m_recursionLevel = 0;
}

void PartitionWithResetsPrioritizationPlugin::reset(RevNumType rev)
{
    //revision information are not taken into account
    return;
}

void PartitionWithResetsPrioritizationPlugin::fillSelection(IntVector& selected, size_t size)
{
    while(m_nofElementsReady < size && !(m_elementsRemaining->empty())) {
        next();
    }

    selected.clear();
    for (size_t i = 0; i < size && i < m_nofElementsReady; i++) {
        selected.push_back((*m_elementsReady)[i]);
    }
}

IndexType PartitionWithResetsPrioritizationPlugin::next()
{
    if (m_elementsRemaining->empty()) {
        throw std::out_of_range("There are not any testcases left.");
    }

    IndexType tcid;

    prioritize();
    qelement d = m_priorityQueue->back();
    if (d.priorityValue - m_currentPartitionMetric > 0) {
        m_priorityQueue->pop_back();
        m_currentCluster->addTestCase(d.testcaseId);
        m_currentPartitionMetric = d.priorityValue;
        tcid = d.testcaseId;
        m_recursionLevel = 0;
        // std::cout << "[SELECTED] tcid(" << d.testcaseId << ") metric: " << d.priorityValue << std::endl;
    } else if (m_recursionLevel == 0) {
        // Reset the internal state and prioritize again
        m_currentCluster->clearTestCases();
        m_currentPartitionMetric = 0.0;
        // Call recursively
        // std::cerr << "[PartitionWithResets] Recursive call" << std::endl;
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

void PartitionWithResetsPrioritizationPlugin::prioritize()
{
    m_priorityQueue->clear();

    for (IndexType i = 0; i < m_elementsRemaining->size(); i++) {
        qelement d;
        d.testcaseId = m_elementsRemaining->at(i);
        d.priorityValue = partitionMetric(m_elementsRemaining->at(i));
        m_priorityQueue->push_back(d);
    }

    sort(m_priorityQueue->begin(), m_priorityQueue->end());
}

double PartitionWithResetsPrioritizationPlugin::partitionMetric(IndexType tcid)
{
    m_currentCluster->addTestCase(tcid);

    CPartitionAlgorithm algorithm;
    algorithm.compute(*m_data, *m_currentCluster);

    CPartitionAlgorithm::PartitionInfo &partitionInfo = algorithm.getPartitionInfo();
    CPartitionAlgorithm::PartitionData &partitions = algorithm.getPartitions();

    IndexType nrOfCodeElementsInPartition = partitionInfo.size();
    double flMetric = 0.0;

    for (CPartitionAlgorithm::PartitionData::iterator partIt = partitions.begin(); partIt != partitions.end(); partIt++) {
        IndexType size = partIt->second.size();

        flMetric += size * (size - 1);
    }

    if(nrOfCodeElementsInPartition == 0){
        flMetric = 0;
    } else if(nrOfCodeElementsInPartition == 1){
        flMetric /= nrOfCodeElementsInPartition * (nrOfCodeElementsInPartition);
    } else {
        flMetric /= nrOfCodeElementsInPartition * (nrOfCodeElementsInPartition - 1);
    }

    m_currentCluster->removeTestCase(tcid);

    return 1.0 - flMetric;
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuitePrioritizationPluginManager().addPlugin(new PartitionWithResetsPrioritizationPlugin());
}

} /* namespace soda */
