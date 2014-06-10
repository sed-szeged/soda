/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
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
#include <iostream>
#include <map>

#include "CPartitionAlgorithm.h"

namespace soda {

CPartitionAlgorithm::CPartitionAlgorithm() :
    m_partitionInfo(new PartitionInfo()),
    m_partitions(new PartitionData())
{
}

CPartitionAlgorithm::~CPartitionAlgorithm()
{
    delete m_partitionInfo;
    delete m_partitions;
}

void CPartitionAlgorithm::compute(CSelectionData &data, CClusterDefinition &cluster, IndexType revision)
{
    std::cerr << "[INFO] Partitioning started" << std::endl;
    CCoverageMatrix *coverage = data.getCoverage();
    std::map<IndexType, IndexType> tcMap;

    IntVector testCaseIds = cluster.getTestCases();
    IntVector codeElementIds = cluster.getCodeElements();

    for (IndexType i = 0; i < testCaseIds.size(); i++) {
        tcMap[testCaseIds[i]] = data.translateTestcaseIdFromCoverageToResults(testCaseIds[i]);
    }

    std::map<IndexType, std::vector<bool> > coverageMap;

    // The number of tests that covers the code elements respectively.
    std::map<IndexType, IndexType> S;
    // The sum of indices of test cases in the matrix that cover code elements.
    std::multimap<IndexType, IndexType> SI;

    IndexType cnt = 0;
    for (IndexType i = 0; i < codeElementIds.size(); i++) {

        if (cnt == 1000) {
            cnt = 0;
            (std::cerr << "[INFO] Preprocessing code element: " << i << "\r").flush();
        }

        IndexType cid = codeElementIds[i];

        IndexType sum = 0;
        IndexType indexSum = 0;

        // Count the number of tests covering this code element and
        // summarize these test case ids.
        for (IndexType j = 0; j < testCaseIds.size(); j++) {
            IndexType tcid = testCaseIds[j];
            if (data.getResults()->getExecutionBitList(revision).at(tcMap[tcid])) {
                if (coverage->getBitMatrix().get(tcid, cid)) {
                    sum++;
                    indexSum += tcid;
                }
                coverageMap[cid].push_back(coverage->getBitMatrix().get(tcid, cid));
            }
        }

        S[cid] = sum;

        SI.insert(std::pair<IndexType, IndexType>(indexSum, cid));

        cnt++;
    }

    std::cerr << std::endl << "[INFO] Preprocessing finished." << std::endl;
    std::cerr << std::endl << "[INFO] Started the partitioning..." << std::endl;


    partition_info pInfo;
    IndexType partitionId = 1;

    cnt = 0;

    while (!SI.empty()) {
        cnt++;
        if (cnt == 1000) {
            cnt = 0;
            (std::cerr << "[INFO] Partitioning. Code elements left: " << SI.size() << "\r").flush();
        }

        // Get the element with biggest indexSum.
        std::multimap<IndexType, IndexType>::reverse_iterator it;

        it = SI.rbegin();
        IndexType cid = it->second;
        IndexType sum = it->first;
        SI.erase(--it.base());


        pInfo.cid = cid;
        pInfo.partitionId = partitionId;
        m_partitionInfo->push_back(pInfo);
        (*m_partitions)[partitionId].insert(cid);

        std::vector<IndexType> tmp;

        while (!SI.empty() && SI.rbegin()->first == sum) {
            cnt++;
            if (cnt == 1000) {
                cnt = 0;
                (std::cerr << "[INFO] Partitioning. Code elements left: " << SI.size() << "\r").flush();
            }

            it = SI.rbegin();
            IndexType iCid = it->second;
            SI.erase(--it.base());
            if (S[iCid] == 0) {
                // We do not need to compare the coverage vector of the code elements because they are not covered at all.
                pInfo.cid = iCid;
                m_partitionInfo->push_back(pInfo);
                (*m_partitions)[partitionId].insert(iCid);
            } else if (S[iCid] == S[cid]) {
                if (coverageMap[cid] == coverageMap[iCid]) {
                    pInfo.cid = iCid;
                    m_partitionInfo->push_back(pInfo);
                    (*m_partitions)[partitionId].insert(iCid);
                } else {
                    tmp.push_back(iCid);
                }
            } else {
                tmp.push_back(iCid);
            }
        }

        for (IndexType i = 0; i < tmp.size(); i++) {
            SI.insert(std::pair<IndexType, IndexType>(sum, tmp[i]));
        }
        tmp.clear();

        partitionId++;
    }

    std::cerr << std::endl << "[INFO] Partitioning FINISHED." << std::endl;
}

} /* namespace soda */
