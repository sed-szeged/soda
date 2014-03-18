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

#include <sstream>

#include "CReductionData.h"

namespace soda {

CReductionData::CReductionData(CCoverageMatrix *baseCoverage, String programName, String dirPath) :
    m_testcases(new CIDManager()),
    m_baseCoverage(baseCoverage),
    m_coverage(new CCoverageMatrix(m_testcases, const_cast<IIDManager *>(&(baseCoverage->getCodeElements())))),
    m_storedTestcases(new std::vector<bool>(m_baseCoverage->getNumOfTestcases())),
    m_programName(programName),
    m_dirPath(dirPath)
{
}

CReductionData::~CReductionData()
{
    delete m_testcases;
    delete m_coverage;
    delete m_storedTestcases;
}

void CReductionData::save(IndexType iteration)
{
    std::stringstream fullPath;
    if (m_dirPath != "") {
        fullPath << m_dirPath << "/";
    }
    fullPath << m_programName << ".cov.";
    fullPath.fill('0');
    fullPath.width(3);
    fullPath << std::right << iteration << ".SoDA";
    m_coverage->save(fullPath.str());
}

void CReductionData::add(const std::set<IndexType> &testcases)
{
    IdxIdxMap tcMap;
    IndexType nrOfMethods = m_baseCoverage->getNumOfCodeElements();
    // Add new testcases.
    for (std::set<IndexType>::iterator it = testcases.begin(); it != testcases.end(); it++) {
        if (!(*m_storedTestcases)[*it]) {
            // The local id of the new element is equal to the actual number of stored testcases.
            tcMap[*it] = m_coverage->getNumOfTestcases();
            String testcaseName = m_baseCoverage->getTestcases().getValue(*it);
            m_coverage->addTestcaseName(testcaseName);
            // Add test to stored elements.
            (*m_storedTestcases)[*it] = true;
        }
    }

    if (tcMap.size() > 0) {
        // Refit size.
        m_coverage->refitMatrixSize();
        // Copy matrix values.
        for (IdxIdxMap::iterator it = tcMap.begin(); it != tcMap.end(); it++) {
            IndexType bTcid = it->first;
            IndexType cTcid = it->second;

            for (IndexType mid = 0; mid < nrOfMethods; mid++) {
                m_coverage->setRelation(cTcid, mid, m_baseCoverage->getBitMatrix().get(bTcid, mid));
            }
        }
    }
}

} /* namespace soda */
