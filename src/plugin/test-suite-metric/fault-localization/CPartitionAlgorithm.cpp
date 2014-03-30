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

#include "CPartitionAlgorithm.h"

namespace soda {

bool operator<(CPartitionAlgorithm::code_element_info c1, CPartitionAlgorithm::code_element_info c2) {
    return c1.sum < c2.sum;
}

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
    CCoverageMatrix *coverage = data.getCoverage();
    std::map<IndexType, IndexType> tcMap;

    IntVector testCaseIds = cluster.getTestCases();
    IntVector codeElementIds = cluster.getCodeElements();

    for (IndexType i = 0; i < testCaseIds.size(); i++) {
        tcMap[testCaseIds[i]] = data.translateTestcaseIdFromCoverageToResults(testCaseIds[i]);
    }

    // The number of tests that covers the code elements respectively.
    std::map<IndexType, IndexType> S;
    // The sum of indices of test cases in the matrix that cover code elements.
    std::vector<code_element_info> SI;

    for (IndexType i = 0; i < codeElementIds.size(); i++) {
        IndexType cid = codeElementIds[i];

        IndexType sum = 0;
        IndexType indexSum = 0;

        // Count the number of tests covering this code element and
        // summarize these test case ids.
        for (IndexType j = 0; j < testCaseIds.size(); j++) {
            IndexType tcid = testCaseIds[0];
            if (data.getResults()->getExecutionBitList(revision).at(tcMap[tcid]) &&
                    coverage->getBitMatrix().get(tcid, cid)) {
                sum++;
                indexSum += tcid;
            }
        }

        S[cid] = sum;

        code_element_info c;
        c.cid = cid;
        c.sum = indexSum;
        SI.push_back(c);
    }

    std::sort(SI.begin(), SI.end());

    partition_info pInfo;
    IndexType partitionId = 1;
    while (!SI.empty()) {
        // Get the element with biggest indexSum.
        code_element_info c = SI.back();
        SI.pop_back();

        pInfo.cid = c.cid;
        pInfo.partitionId = partitionId;
        m_partitionInfo->push_back(pInfo);
        (*m_partitions)[partitionId].insert(c.cid);

        std::vector<code_element_info> tmp;
        std::vector<bool> cVector;

        for (IndexType i = 0; i < testCaseIds.size(); i++) {
            IndexType tcid = testCaseIds[i];
            if (data.getResults()->getExecutionBitList(revision).at(tcMap[tcid])) {
                cVector.push_back(coverage->getBitMatrix().get(tcid, c.cid));
            }
        }
        while (!SI.empty() && SI.back().sum == c.sum) {
            code_element_info i = SI.back();
            SI.pop_back();
            if (S[i.cid] == S[c.cid]) {
                std::vector<bool> iVector;
                for (IndexType j = 0; j < testCaseIds.size(); j++) {
                    IndexType tcid = testCaseIds[j];
                    if (data.getResults()->getExecutionBitList(revision).at(tcMap[tcid])) {
                        iVector.push_back(coverage->getBitMatrix().get(tcid, i.cid));
                    }
                }
                if (cVector == iVector) {
                    pInfo.cid = i.cid;
                    m_partitionInfo->push_back(pInfo);
                    (*m_partitions)[partitionId].insert(i.cid);
                } else {
                    tmp.push_back(i);
                }
            } else {
                tmp.push_back(i);
            }
        }

        SI.insert(SI.end(), tmp.rbegin(), tmp.rend());
        tmp.clear();

        partitionId++;
    }
}

} /* namespace soda */
