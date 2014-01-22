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

#include "data/CSelectionData.h"
#include "exception/CException.h"

#include <iostream>

namespace soda {

CSelectionData::CSelectionData() :
    m_globalCodeElements(new CIDManager()),
    m_globalTestcases(new CIDManager()),

    m_changesetCodeElements(new CIDMapper(m_globalCodeElements)),
    m_coverageCodeElements(new CIDMapper(m_globalCodeElements)),
    m_coverageTestcases(new CIDMapper(m_globalTestcases)),
    m_resultsTestcases(new CIDMapper(m_globalTestcases)),

    m_changeset(new CChangeset(m_changesetCodeElements)),
    m_coverage(new CCoverageMatrix(m_coverageTestcases, m_coverageCodeElements)),
    m_results(new CResultsMatrix(m_resultsTestcases))
{
}

CSelectionData::~CSelectionData()
{
    delete m_changeset;
    delete m_coverage;
    delete m_results;

    delete m_changesetCodeElements;
    delete m_coverageCodeElements;
    delete m_coverageTestcases;
    delete m_resultsTestcases;

    delete m_globalCodeElements;
    delete m_globalTestcases;
}

void CSelectionData::loadChangeset(const char* fname)
{
    m_changeset->load(fname);
}

void CSelectionData::loadCoverage(const char* fname)
{
    m_coverage->load(fname);
}

void CSelectionData::loadResults(const char* fname)
{
    m_results->load(fname);
}

void CSelectionData::globalize()
{
    for (IndexType i = 0; i < m_globalCodeElements->size(); i++) {
        String cename = (*m_globalCodeElements)[i];
        m_changeset->addCodeElementName(cename);
        m_coverage->addCodeElementName(cename);
    }

    for (IndexType i = 0; i < m_globalTestcases->size(); i++) {
        String tcname = (*m_globalTestcases)[i];
        m_results->addTestcaseName(tcname);
        m_coverage->addTestcaseName(tcname);
    }

    m_changeset->refitSize();
    m_coverage->refitMatrixSize();
    m_results->refitMatrixSize();
}

} /* namespace soda*/
