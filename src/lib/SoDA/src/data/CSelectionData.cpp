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

namespace soda {

CSelectionData::CSelectionData() :
    m_globalCodeElements(new CIDManager()),
    m_globalTestcases(new CIDManager()),

    m_changesetCodeElements(new CIDMapper(m_globalCodeElements)),
    m_coverageCodeElements(new CIDMapper(m_globalCodeElements)),
    m_bugCodeElements(new CIDManager()),
    m_coverageTestcases(new CIDMapper(m_globalTestcases)),
    m_resultsTestcases(new CIDMapper(m_globalTestcases)),

    m_changeset(new CChangeset(m_changesetCodeElements)),
    m_coverage(new CCoverageMatrix(m_coverageTestcases, m_coverageCodeElements)),
    m_bugs(new CBugset(m_bugCodeElements)),
    m_results(new CResultsMatrix(m_resultsTestcases))
{
}

CSelectionData::~CSelectionData()
{
    delete m_changeset;
    delete m_coverage;
    delete m_results;
    delete m_bugs;

    delete m_bugCodeElements;
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

void CSelectionData::loadChangeset(const String &filename)
{
    m_changeset->load(filename);
}

void CSelectionData::loadCoverage(const char* fname)
{
    m_coverage->load(fname);
}

void CSelectionData::loadCoverage(const String &filename)
{
    m_coverage->load(filename);
}

void CSelectionData::loadResults(const char* fname)
{
    m_results->load(fname);
}

void CSelectionData::loadResults(const String &filename)
{
    m_results->load(filename);
}

void CSelectionData::loadBugs(const char* fname)
{
    m_bugs->load(fname);
}

void CSelectionData::loadBugs(const String &filename)
{
    m_bugs->load(filename);
}

void CSelectionData::globalize()
{
    for (IndexType i = 0; i < m_globalCodeElements->size(); i++) {
        String cename = (*m_globalCodeElements)[i];
        //m_bugs->addCodeElementName(cename);
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

void CSelectionData::filterToCoverage()
{
    CIDMapper *l_changesetCodeElements = new CIDMapper(m_globalCodeElements);
    CIDMapper *l_resultsTestcases = new CIDMapper(m_globalTestcases);

    CChangeset *l_changeset = new CChangeset(l_changesetCodeElements);
    CResultsMatrix *l_results = new CResultsMatrix(l_resultsTestcases);

    for (IndexType i = 0; i < m_coverageCodeElements->size(); i++) {
        String fname = (*m_globalCodeElements)[i];
        l_changeset->addCodeElementName(fname);
    }

    for (IndexType i = 0; i < m_coverageTestcases->size(); i++) {
        String tcname = (*m_globalTestcases)[i];
        l_results->addTestcaseName(tcname);
    }

    const IntVector& revs = m_results->getRevisionNumbers();
    for (size_t r = 0; r < revs.size(); ++r) {
        l_results->addRevisionNumber(revs[r]);
        l_changeset->addRevision(revs[r]);
    }

    l_changeset->refitSize();
    l_results->refitMatrixSize();

    for (IndexType i = 0; i < m_coverageTestcases->size(); i++) {
        String tcname = (*m_coverageTestcases)[i];
        for (size_t r = 0; r < revs.size(); ++r) {
            IndexType lid, ceid;
            if ((lid = l_resultsTestcases->getID(tcname)) && ((ceid = m_resultsTestcases->getID(tcname)))) {
                l_results->setResult(revs[r], lid, m_results->getResult(revs[r], ceid));
            }
        }
    }

    for (size_t r = 0; r < revs.size(); ++r) {
        if (m_changeset->exists(revs[r])) {
            const StringVector& chng = m_changeset->getCodeElementNames(revs[r]);
            for (size_t s = 0; s < chng.size(); ++s) {
                if (l_changesetCodeElements->getID(chng[s])) {
                    l_changeset->setChange(revs[r], chng[s]);
                }
            }
        }
    }

    delete m_changeset;
    m_changeset = l_changeset;
    delete m_results;
    m_results = l_results;
    delete m_changesetCodeElements;
    m_changesetCodeElements = l_changesetCodeElements;
    delete m_resultsTestcases;
    m_resultsTestcases = l_resultsTestcases;
}

} /* namespace soda*/
