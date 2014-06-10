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

#include <stdexcept>

#include "data/CCoverageMatrix.h"
#include "data/CBitMatrix.h"
#include "data/CIDManager.h"
#include "exception/CException.h"

namespace soda {

CCoverageMatrix::CCoverageMatrix() :
    m_testcases(new CIDManager()),
    m_codeElements(new CIDManager()),
    m_data(new CBitMatrix()),
    m_createTestcases(true),
    m_createCodeElements(true),
    m_createBitMatrix(true)
{ }

CCoverageMatrix::CCoverageMatrix(IIDManager* testcases, IIDManager* codeElements) :
    m_testcases(testcases),
    m_codeElements(codeElements),
    m_data(new CBitMatrix()),
    m_createTestcases(false),
    m_createCodeElements(false),
    m_createBitMatrix(true)
{ }

CCoverageMatrix::CCoverageMatrix(IIDManager* testcases, IIDManager* codeElements, IBitMatrix* data) :
    m_testcases(testcases),
    m_codeElements(codeElements),
    m_data(data),
    m_createTestcases(false),
    m_createCodeElements(false),
    m_createBitMatrix(false)
{ }

CCoverageMatrix::~CCoverageMatrix()
{
    if(m_createTestcases) {
        delete m_testcases;
    }
    if(m_createCodeElements) {
        delete m_codeElements;
    }
    if(m_createBitMatrix) {
        delete m_data;
    }
}

const IIDManager& CCoverageMatrix::getTestcases() const
{
    return *m_testcases;
}

StringVector CCoverageMatrix::getTestcases(const String& codeElementName) const
{
    StringVector testcaseList;
    if (!m_codeElements->containsValue(codeElementName)) {
        return testcaseList;
    }

    for(IndexType i = 0; i < getNumOfTestcases(); i++) {
        if((*m_data)[i][(*m_codeElements)[codeElementName]]) {
            testcaseList.push_back((*m_testcases)[i]);
        }
    }
    return testcaseList;
}

const IIDManager& CCoverageMatrix::getCodeElements() const
{
    return *m_codeElements;
}

StringVector CCoverageMatrix::getCodeElements(const String& testcaseName) const
{
    StringVector codeElementsList;
    if (!m_testcases->containsValue(testcaseName)) {
        return codeElementsList;
    }

    for(IndexType i = 0; i < getNumOfCodeElements(); i++) {
        if((*m_data)[(*m_testcases)[testcaseName]][i]) {
            codeElementsList.push_back((*m_codeElements)[i]);
        }
    }

    return codeElementsList;
}

const IBitMatrix& CCoverageMatrix::getBitMatrix() const
{
    return *m_data;
}

const IndexType CCoverageMatrix::getNumOfTestcases() const
{
    return m_testcases->getIDList().size();
}

const IndexType CCoverageMatrix::getNumOfCodeElements() const
{
    return m_codeElements->getIDList().size();
}

bool CCoverageMatrix::getRelation(const String& testcaseName, const String& codeElementName) const
{
    if (!m_testcases->containsValue(testcaseName) || !m_codeElements->containsValue(codeElementName)) {
        throw CException("CCoverageMatrix::setRelation()", "Coverage matrix does not contain item!");
    }

    return m_data->get((*m_testcases)[testcaseName], (*m_codeElements)[codeElementName]);
}

void CCoverageMatrix::setRelation(const String& testcaseName, const String& codeElementName, const bool isCovered)
{
    try {
        m_data->set((*m_testcases)[testcaseName], (*m_codeElements)[codeElementName], isCovered);
    } catch (std::out_of_range &e) {
        throw CException("CCoverageMatrix::setRelation()", "Coverage matrix does not contain item!");
    }
}

void CCoverageMatrix::setRelation(const IndexType testcaseIdx, const IndexType codeElementIdx, const bool isCovered)
{
    m_data->set(testcaseIdx, codeElementIdx, isCovered);
}

void CCoverageMatrix::addOrSetRelation(const String& testcaseName, const String& codeElementName, const bool isCovered)
{
    if (!m_testcases->containsValue(testcaseName)) {
        m_data->resize(m_data->getNumOfRows()+1, m_data->getNumOfCols());
        m_testcases->add(m_data->getNumOfRows()-1, testcaseName);
    }

    if (!m_codeElements->containsValue(codeElementName)) {
        m_data->resize(m_data->getNumOfRows(), m_data->getNumOfCols()+1);
        m_codeElements->add(m_data->getNumOfCols()-1, codeElementName);
    }

    m_data->set((*m_testcases)[testcaseName], (*m_codeElements)[codeElementName], isCovered);
}

void CCoverageMatrix::refitMatrixSize()
{
    m_data->resize(m_testcases->size(), m_codeElements->size());
}

void CCoverageMatrix::addTestcaseName(const String& testcaseName)
{
    if (!m_testcases->containsValue(testcaseName)) {
        m_testcases->add(testcaseName);
    }
}

void CCoverageMatrix::addCodeElementName(const String& codeElementName)
{
    if (!m_codeElements->containsValue(codeElementName)) {
        m_codeElements->add(codeElementName);
    }
}

void CCoverageMatrix::save(io::CBinaryIO *out) const
{
    m_testcases->save(out, io::CSoDAio::TCLIST);
    m_codeElements->save(out, io::CSoDAio::PRLIST);
    m_data->save(out, io::CSoDAio::COVERAGE);
}

void CCoverageMatrix::load(io::CSoDAio *in)
{
    bool isTC = false;
    bool isPR = false;
    bool isCOV = false;

    while(in->nextChunkID()) {
        if(in->getChunkID() == io::CSoDAio::TCLIST) {
            m_testcases->load(in);
            isTC = true;
        } else if(in->getChunkID() == io::CSoDAio::PRLIST) {
            m_codeElements->load(in);
            isPR = true;
        } else if(in->getChunkID() == io::CSoDAio::COVERAGE) {
            m_data->load(in);
            isCOV = true;
        }
    }

    if(!isTC || !isPR || !isCOV) {
        throw CException("soda::CCoverageMatrix","There is no coverage info in this file!");
    }
}

void CCoverageMatrix::save(const char * filename) const
{
    io::CSoDAio *out = new io::CSoDAio(filename, io::CBinaryIO::omWrite);
    save(out);
    delete out;
}

void CCoverageMatrix::load(const char * filename)
{
    io::CSoDAio *in = new io::CSoDAio(filename, io::CBinaryIO::omRead);
    load(in);
    delete in;
}

void CCoverageMatrix::save(const String& filename) const
{
    save(filename.c_str());
}

void CCoverageMatrix::load(const String& filename)
{
    load(filename.c_str());
}

} // namespace soda
