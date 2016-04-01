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
#include "data/CClusterDefinition.h"

namespace soda {

CClusterDefinition::CClusterDefinition() :
    m_testCases(new std::vector<IndexType>),
    m_codeElements(new std::vector<IndexType>)
{

}

CClusterDefinition::CClusterDefinition(const CClusterDefinition &other) :
    m_testCases(new std::vector<IndexType>(*(other.m_testCases))),
    m_codeElements(new std::vector<IndexType>(*(other.m_codeElements)))
{

}

CClusterDefinition::~CClusterDefinition()
{
    delete m_testCases;
    delete m_codeElements;
}

CClusterDefinition& CClusterDefinition::operator=(const CClusterDefinition &rhs)
{
    CClusterDefinition tmp(rhs);

    std::swap(m_testCases, tmp.m_testCases);
    std::swap(m_codeElements, tmp.m_codeElements);

    return *this;
}

void CClusterDefinition::addTestCase(IndexType tcid)
{
    m_testCases->push_back(tcid);
}

void CClusterDefinition::addTestCases(const std::vector<IndexType> &testCases)
{
    m_testCases->insert(m_testCases->end(), testCases.begin(), testCases.end());
}

void CClusterDefinition::clearTestCases()
{
    m_testCases->clear();
}

void CClusterDefinition::removeTestCase(IndexType tcid)
{
    IntVector::iterator pos = std::find(m_testCases->begin(), m_testCases->end(), tcid);
    if (pos != m_testCases->end()) {
        m_testCases->erase(pos);
    }
}

void CClusterDefinition::addCodeElement(IndexType cid)
{
    m_codeElements->push_back(cid);
}

void CClusterDefinition::addCodeElements(const std::vector<IndexType> &codeElements)
{
    m_codeElements->insert(m_codeElements->end(), codeElements.begin(), codeElements.end());
}

const std::vector<IndexType>& CClusterDefinition::getTestCases()
{
    return *m_testCases;
}

const std::vector<IndexType>& CClusterDefinition::getCodeElements()
{
    return *m_codeElements;
}


} /* namespace soda */
