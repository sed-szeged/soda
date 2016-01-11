/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Havas <havasd@inf.u-szeged.hu>
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

#include "exception/CException.h"
#include "util/CDataHandler.h"
#include "util/CCoverageDataManager.h"
#include "util/CResultsDataManager.h"
#include "util/CChangesDataManager.h"
#include <fstream>

namespace soda {

CDataHandler::CDataHandler() :
    printInfo(true), m_bWithPassFail(true), withNames(false),
    m_eReadFormat(rfUnknown), m_pChanges(NULL), m_pCoverage(NULL),
    m_pResults(NULL), m_pSelection(NULL), m_pTestcases(NULL),
    m_pCodeElements(NULL)
{}

CDataHandler::~CDataHandler()
{
    delete m_pChanges;
    delete m_pCoverage;
    delete m_pCodeElements;
    delete m_pResults;
    delete m_pSelection;
    delete m_pTestcases;
}

CCoverageDataManager CDataHandler::getCoverageDataMgr()
{
    return CCoverageDataManager(this);
}

CChangesDataManager CDataHandler::getChangesDataMgr()
{
    return CChangesDataManager(this);
}

CResultsDataManager CDataHandler::getResultsDataMgr()
{
    return CResultsDataManager(this);
}

bool CDataHandler::createSelection()
{
    return (m_pSelection = new CSelectionData()) != NULL;
}

CChangeset* CDataHandler::getChanges(bool create)
{
    if (create && !m_pChanges)
        m_pChanges = new CChangeset(GET(m_pTestcases));

    return m_pChanges;
}

CCoverageMatrix* CDataHandler::getCoverage(bool create)
{
    if (create && !m_pCoverage)
        m_pCoverage = new CCoverageMatrix(GET(m_pTestcases), GET(m_pCodeElements));

    return m_pCoverage;
}

void CDataHandler::setCoverage(CCoverageMatrix *coverage)
{
    m_pCoverage = coverage;
}

CResultsMatrix* CDataHandler::getResults(bool create)
{
    if(create && !m_pResults)
        m_pResults = new CResultsMatrix(GET(m_pTestcases));

    return m_pResults;
}

void CDataHandler::setResults(CResultsMatrix *results)
{
    m_pResults = results;
}

CSelectionData* CDataHandler::getSelection()
{
    return m_pSelection;
}

CIDManager* CDataHandler::getTestcases()
{
    return m_pTestcases;
}

CIDManager* CDataHandler::getCodeElements()
{
    return m_pCodeElements;
}

bool CDataHandler::getPrintInfo() const
{
    return printInfo;
}

void CDataHandler::setPrintInfo(bool set)
{
    printInfo = set;
}

bool CDataHandler::getWithPassFail() const
{
    return m_bWithPassFail;
}

void CDataHandler::setWithPassFail(bool set)
{
    m_bWithPassFail = set;
}

bool CDataHandler::getWithNames() const
{
    return withNames;
}

void CDataHandler::setWithNames(bool set)
{
    withNames = set;
}

IndexType CDataHandler::getRevision() const {
    return revision;
}

void CDataHandler::setRevision(IndexType rev)
{
    revision = rev;
}

time_t CDataHandler::getRevisionTimestamp() const {
    return revisionTime;
}

void CDataHandler::setRevisionTimestamp(time_t time) {
    revisionTime = time;
}

ReadFormat CDataHandler::getReadFormat() const
{
    return m_eReadFormat;
}

void CDataHandler::setReadFormat(ReadFormat format)
{
    m_eReadFormat = format;
}

void CDataHandler::loadCodeElementFilter(const String &filePath)
{
    loadFilter(filePath, m_codeElementFilter);
}

void CDataHandler::loadTestcaseFilter(const String &filePath)
{
    loadFilter(filePath, m_testFilter);
}

void CDataHandler::loadFilter(const String &filePath, std::set<String> &filterContainer)
{
    INFO(getPrintInfo(), "CDataHandler::loadFilter(\"" << filePath << "\")");
    path p(filePath);
    if (CDataManager::checkPathOk(p) == 1) {
        std::ifstream in(filePath);
        String line;
        while (std::getline(in, line)) {
            filterContainer.insert(line);
        }
    } else
        throw CException("CDataHandler::loadFilter", filePath + " is not a regular file");
}

CCoverageMatrix* CDataHandler::filterCoverage(CCoverageMatrix *coverage, bool dispose)
{
    CCoverageMatrix* filtered = new CCoverageMatrix();
    for (const String& ce : coverage->getCodeElements().getValueList()) {
        if (!m_codeElementFilter.count(ce)) {
            filtered->addCodeElementName(ce);
        }
    }
    for (const String& tc : coverage->getTestcases().getValueList()) {
        if (!m_testFilter.count(tc)) {
            filtered->addTestcaseName(tc);
        }
    }
    filtered->refitMatrixSize();
    for (const String& ce : filtered->getCodeElements().getValueList()) {
        for (const String& tc : filtered->getTestcases().getValueList()) {
            filtered->setRelation(tc, ce, coverage->getRelation(tc, ce));
        }
    }
    if (dispose) {
        delete coverage;
    }
    return filtered;
}

CResultsMatrix* CDataHandler::filterResults(CResultsMatrix *results, bool dispose)
{
    CResultsMatrix* filtered = new CResultsMatrix();
    for (const String& tc : results->getTestcases().getValueList()) {
        if (!m_testFilter.count(tc)) {
            filtered->addTestcaseName(tc);
        }
    }
    for (const IntVector::value_type rev : results->getRevisionNumbers()) {
        filtered->addRevisionNumber(rev);
    }
    filtered->refitMatrixSize();
    for (const String& tc : filtered->getTestcases().getValueList()) {
        for (const IntVector::value_type rev : filtered->getRevisionNumbers()) {
            filtered->setResult(rev, tc, results->getResult(rev, tc));
        }
    }
    if (dispose) {
        delete results;
    }
    return filtered;
}

} // namespace soda
