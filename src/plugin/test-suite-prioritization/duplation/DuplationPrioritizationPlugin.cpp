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
#include <cstdlib>
#include <ctime>

#include "DuplationPrioritizationPlugin.h"

namespace soda {

DuplationPrioritizationPlugin::DuplationPrioritizationPlugin():
        m_data(NULL),
        m_nofElementsReady(0),
        m_elementsReady(new IntVector()),
        m_elementsRemaining(new IntVector()),
        m_partitions(new std::vector<IndexType>()),
        m_partitionSizes(new std::map<IndexType, IndexType>()),
        m_nextTcid(0)
{}

DuplationPrioritizationPlugin::~DuplationPrioritizationPlugin()
{
    delete m_elementsReady;
    delete m_elementsRemaining;
    delete m_partitions;
    delete m_partitionSizes;
    m_nofElementsReady = 0;
}

String DuplationPrioritizationPlugin::getName()
{
    return "duplation";
}

String DuplationPrioritizationPlugin::getDescription()
{
    return "DuplationPrioritizationPlugin optimize for fault localization";
}

void DuplationPrioritizationPlugin::init(CSelectionData *data, CKernel *kernel)
{
    m_data = data;
    IntVector initial;
    setState(initial);
}

void DuplationPrioritizationPlugin::setState(IntVector &ordered)
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

    m_nofElementsReady = ordered.size();
    if (m_nofElementsReady > 0) {
        m_nextTcid = m_nofElementsReady - 1;
    }



    // Put each code element into the same partition
    IndexType nOfCodeElements = m_data->getCoverage()->getNumOfCodeElements();
    m_partitions->resize(nOfCodeElements, 1);
    // There is only one partition and it contains all code elements
    (*m_partitionSizes)[1] = nOfCodeElements;
}

void DuplationPrioritizationPlugin::reset(RevNumType rev)
{
    // revision information are not taken into account
    return;
}

void DuplationPrioritizationPlugin::fillSelection(IntVector& selected, size_t size)
{
    while(m_nofElementsReady < size && !(m_elementsRemaining->empty())) {
        next();
    }

    selected.clear();
    for (size_t i = 0; i < size && i < m_nofElementsReady; i++) {
        selected.push_back((*m_elementsReady)[i]);
    }
}

IndexType DuplationPrioritizationPlugin::next()
{
    // We do not need to calculate anyithing
    if (m_nextTcid < m_nofElementsReady) {
        return (*m_elementsReady)[m_nextTcid++];
    }

    // Collect the coverage information of the remaining test cases for each partition
    IndexType nOfCodeElements = m_data->getCoverage()->getNumOfCodeElements();
    std::map<IndexType, std::map<IndexType, IndexType> > partitionCoverage;
    for (IndexType const &tcid : *m_elementsRemaining) {
        for (IndexType cid = 0; cid < nOfCodeElements ; cid++) {
            if (m_data->getCoverage()->getBitMatrix().get(tcid, cid)) {
                partitionCoverage[tcid][((*m_partitions)[cid])]++;
            }
        }
    }
    // Select the test that separates best the partitions
    std::set<IndexType> testOccupied;
    std::map<IndexType, IndexType> selectedTestcases;
    for (auto const &partitionInfo : *m_partitionSizes) {
        IndexType partitionId = partitionInfo.first;
        IndexType bestCoverage = nOfCodeElements * 2 + 1;
        for (IndexType const &tcid : *m_elementsRemaining) {
            IndexType coverage = partitionCoverage[tcid][partitionId];
            if (std::abs(partitionInfo.second / 2.0 - coverage) < std::abs(partitionInfo.second / 2.0 - bestCoverage) &&
                    testOccupied.find(tcid) == testOccupied.end()) {
                bestCoverage = coverage;
                selectedTestcases[partitionId] = tcid;
            }
        }
        if (selectedTestcases.find(partitionId) != selectedTestcases.end()) {
            if (!testOccupied.insert(selectedTestcases[partitionId]).second) {
                std::cerr << "[DUPLATION] twice occupied test OCCUP: [" << partitionId << "] " << selectedTestcases[partitionId] << std::endl;
            }
        }
    }

    // Mark the selected test cases as ready
    IndexType result;
    for (auto const &selectionInfo : selectedTestcases) {
        IndexType tcid = selectionInfo.second;
        IntVector::iterator pos = std::find(m_elementsRemaining->begin(), m_elementsRemaining->end(), tcid);
        m_elementsRemaining->erase(pos);
        m_elementsReady->push_back(tcid);
        m_nofElementsReady++;
    }
    updatePartitions(selectedTestcases);
    return (*m_elementsReady)[m_nextTcid++];
}

void DuplationPrioritizationPlugin::updatePartitions(std::map<IndexType, IndexType> &selectedTestcases)
{
    if (selectedTestcases.size() < m_partitionSizes->size()) {
        return;
    }
    m_partitionSizes->clear();
    IndexType nOfCodeElements = m_data->getCoverage()->getNumOfCodeElements();
    for (IndexType cid = 0; cid < nOfCodeElements ; cid++) {
        IndexType partitionId = (*m_partitions)[cid];

        if (m_data->getCoverage()->getBitMatrix().get(selectedTestcases[partitionId], cid)) {
            (*m_partitions)[cid] = (*m_partitions)[cid] * 2;
        } else {
            (*m_partitions)[cid] = (*m_partitions)[cid] * 2 - 1;
        }
        (*m_partitionSizes)[(*m_partitions)[cid]]++;
    }
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuitePrioritizationPluginManager().addPlugin(new DuplationPrioritizationPlugin());
}

} /* namespace soda */
