/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: László Langó <lango@inf.u-szeged.hu>
 *          Tamás Gergely <gertom@inf.u-szeged.hu>
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

#include <iostream>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include "data/CResultsMatrix.h"
#include "data/CBitMatrix.h"
#include "data/CIDManager.h"
#include "exception/CException.h"

namespace soda {

CResultsMatrix::CResultsMatrix() :
    m_testcases(new CIDManager()),
    m_revisions(new CRevision<IndexType>()),
    m_exec(new CBitMatrix()),
    m_pass(new CBitMatrix()),
    m_createTestcases(true),
    m_createRevisions(true),
    m_createExecutionBitMatrix(true),
    m_createPassedBitMatrix(true)
{}

CResultsMatrix::CResultsMatrix(IIDManager* testcases) :
    m_testcases(testcases),
    m_revisions(new CRevision<IndexType>()),
    m_exec(new CBitMatrix()),
    m_pass(new CBitMatrix()),
    m_createTestcases(false),
    m_createRevisions(true),
    m_createExecutionBitMatrix(true),
    m_createPassedBitMatrix(true)
{}

CResultsMatrix::CResultsMatrix(IIDManager* testcases, CRevision<IndexType>* revisions, IBitMatrix* exec, IBitMatrix* pass) :
    m_testcases(testcases),
    m_revisions(revisions),
    m_exec(exec),
    m_pass(pass),
    m_createTestcases(false),
    m_createRevisions(false),
    m_createExecutionBitMatrix(false),
    m_createPassedBitMatrix(false)
{}

CResultsMatrix::~CResultsMatrix()
{
    if(m_createTestcases)
        delete m_testcases;
    if(m_createRevisions)
        delete m_revisions;
    if(m_createExecutionBitMatrix)
        delete m_exec;
    if(m_createPassedBitMatrix)
        delete m_pass;
}

IntVector CResultsMatrix::getRevisionNumbers() const
{
    return m_revisions->getRevisionNumbers();
}

const CRevision<IndexType>& CResultsMatrix::getRevisions() const
{
    return *m_revisions;
}

const IIDManager& CResultsMatrix::getTestcases() const
{
    return *m_testcases;
}

const IBitMatrix& CResultsMatrix::getExecutionBitMatrix() const
{
    return *m_exec;
}

const IBitMatrix& CResultsMatrix::getPassedBitMatrix() const
{
    return *m_pass;
}

const IndexType CResultsMatrix::getNumOfTestcases() const
{
    return m_testcases->size();
}

const IndexType CResultsMatrix::getNumOfRevisions() const
{
    return m_revisions->size();
}

const IBitList& CResultsMatrix::getExecutionBitList(const int revision) const
{
    return m_exec->getRow((*m_revisions)[revision]);
}

const IBitList& CResultsMatrix::getPassedBitList(const int revision) const
{
    return m_pass->getRow((*m_revisions)[revision]);
}

CResultsMatrix::TestResultType CResultsMatrix::getResult(const int revision, const String& testcaseName) const
{
    IndexType testcaseID = (*m_testcases)[testcaseName];
    return getResult(revision, testcaseID);
}

CResultsMatrix::TestResultType CResultsMatrix::getResult(const int revision, const IndexType testcaseID) const
{
    return (TestResultType)
          (((*m_exec)[(*m_revisions)[revision]][testcaseID] << 1) |
           ((*m_exec)[(*m_revisions)[revision]][testcaseID] &
            (*m_pass)[(*m_revisions)[revision]][testcaseID]));
}

bool CResultsMatrix::isExecuted(const int revision, const String& testcaseName) const
{
    return (*m_exec)[(*m_revisions)[revision]][(*m_testcases)[testcaseName]];
}

bool CResultsMatrix::isPassed(const int revision, const String& testcaseName) const
{
    return (*m_exec)[(*m_revisions)[revision]][(*m_testcases)[testcaseName]] &&
           (*m_pass)[(*m_revisions)[revision]][(*m_testcases)[testcaseName]];
}

void CResultsMatrix::setResult(const int revision, const String& testcaseName, const CResultsMatrix::TestResultType result)
{
    if(!m_revisions->revisionExists(revision) || !m_testcases->containsValue(testcaseName)) {
        throw CException("CResultsMatrix::setRelation()", "Results matrix does not contain item!");
    }
    m_exec->set((*m_revisions)[revision], (*m_testcases)[testcaseName], result != trtNotExecuted );
    m_pass->set((*m_revisions)[revision], (*m_testcases)[testcaseName], result == trtPassed);
}

void CResultsMatrix::setResult(const int revision, const IndexType testcaseID, const CResultsMatrix::TestResultType result)
{
    if(!m_revisions->revisionExists(revision) || m_testcases->size() <= testcaseID) {
        throw CException("CResultsMatrix::setRelation()", "Results matrix does not contain item!");
    }
    m_exec->set((*m_revisions)[revision], testcaseID, result != trtNotExecuted );
    m_pass->set((*m_revisions)[revision], testcaseID, result == trtPassed);
}

void CResultsMatrix::addOrSetResult(const int revision, const String& testcaseName, const CResultsMatrix::TestResultType result)
{
    if(!m_revisions->revisionExists(revision)) {
        IndexType index = m_exec->getNumOfRows();
        m_exec->resize(m_exec->getNumOfRows()+1, m_exec->getNumOfCols());
        m_pass->resize(m_pass->getNumOfRows()+1, m_pass->getNumOfCols());
        m_revisions->addRevision(revision, index);
    }
    if(!m_testcases->containsValue(testcaseName)) {
        m_exec->resize(m_exec->getNumOfRows(), m_exec->getNumOfCols()+1);
        m_pass->resize(m_pass->getNumOfRows(), m_pass->getNumOfCols()+1);
        m_testcases->add(m_exec->getNumOfCols()-1, testcaseName);
    }
    m_exec->set((*m_revisions)[revision], (*m_testcases)[testcaseName], result != trtNotExecuted );
    m_pass->set((*m_revisions)[revision], (*m_testcases)[testcaseName], result == trtPassed);
}

void CResultsMatrix::refitMatrixSize()
{
    m_exec->resize(m_revisions->size(), m_testcases->size());
    m_pass->resize(m_revisions->size(), m_testcases->size());
}

void CResultsMatrix::addRevisionNumber(const int revision)
{
    if(!m_revisions->revisionExists(revision)) {
        IndexType index = m_revisions->size();
        m_revisions->addRevision(revision, index);
    }
}

void CResultsMatrix::addTestcaseName(const String& testcaseName)
{
    if(!m_testcases->containsValue(testcaseName)) {
        m_testcases->add(testcaseName);
    }
}

void CResultsMatrix::save(const char * filename) const
{
    io::CSoDAio *out;
    out = new io::CSoDAio(filename, io::CBinaryIO::omWrite);
    save(out);
    out->close();
    delete out;
}

void CResultsMatrix::save(io::CBinaryIO * out) const
{
    m_revisions->save(out, io::CSoDAio::REVISIONS);
    m_testcases->save(out, io::CSoDAio::TCLIST);
    m_exec->save(out, io::CSoDAio::EXECUTION);
    m_pass->save(out, io::CSoDAio::PASSED);
}

void CResultsMatrix::load(const char * filename)
{
    io::CSoDAio *in;
    in = new io::CSoDAio(filename, io::CBinaryIO::omRead);
    load(in);
    in->close();
    delete in;
}

void CResultsMatrix::load(io::CSoDAio* in)
{
    bool isEXEC = false;
    bool isPASS = false;
    bool isTC = false;
    bool isREV = false;

    while(in->nextChunkID()) {
        if(in->getChunkID() == io::CSoDAio::TCLIST) {
            m_testcases->load(in);
            isTC = true;
        } else if(in->getChunkID() == io::CSoDAio::REVISIONS) {
            m_revisions->load(in);
            isREV = true;
        } else if(in->getChunkID() == io::CSoDAio::EXECUTION) {
            m_exec->load(in);
            isEXEC = true;
        } else if(in->getChunkID() == io::CSoDAio::PASSED) {
            m_pass->load(in);
            isPASS = true;
        }
    }

    if(!isEXEC || !isPASS || !isTC || !isREV) {
        throw CException("soda::CResultsMatrix","There is no results matrix info in this file!");
    }
}


void CResultsMatrix::save(const String& filename) const
{
    save(filename.c_str());
}

void CResultsMatrix::load(const String& filename)
{
    load(filename.c_str());
}

} /* namespace soda */
