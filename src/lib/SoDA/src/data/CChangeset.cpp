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

#include "data/CChangeset.h"
#include "data/CIndexBitList.h"
#include "data/CIDManager.h"
#include "exception/CException.h"

namespace soda {

CChangeset::CChangeset() :
    m_codeElements(new CIDManager()),
    m_changes(new CRevision<IBitList*>()),
    m_createCodeElements(true),
    m_createChanges(true)
{ }

CChangeset::CChangeset(IIDManager* codeElements) :
    m_codeElements(codeElements),
    m_changes(new CRevision<IBitList*>()),
    m_createCodeElements(false),
    m_createChanges(true)
{ }

CChangeset::CChangeset(IIDManager* codeElements, CRevision<IBitList*>* changes) :
    m_codeElements(codeElements),
    m_changes(changes),
    m_createCodeElements(false),
    m_createChanges(false)
{ }

CChangeset::~CChangeset()
{
    if(m_createChanges) {
        IntVector revNums = m_changes->getRevisionNumbers();
        for(IndexType i = 0; i < revNums.size(); i++) {
            delete m_changes->getRevision(revNums[i]);
            m_changes->getRevision(revNums[i]) = NULL;
        }
        delete m_changes;
        m_changes = NULL;
    }
    if(m_createCodeElements) {
        delete m_codeElements;
        m_codeElements = NULL;
    }
}

IBitList& CChangeset::operator[](const RevNumType revisionNumber) const
{
    return *(m_changes->getRevision(revisionNumber));
}

IBitList& CChangeset::at(const RevNumType revisionNumber) const
{
    return *(m_changes->getRevision(revisionNumber));
}

bool CChangeset::exists(const RevNumType revisionNumber) const
{
    return m_changes->revisionExists(revisionNumber);
}

IntVector CChangeset::getRevisions(const String& codeElementName) const
{
    IntVector result;
    IntVector revNums = m_changes->getRevisionNumbers();
    for(IndexType i = 0; i < revNums.size(); i++) {
        if(m_changes->getRevision(revNums[i])->at((*m_codeElements)[codeElementName]))
            result.push_back(revNums[i]);
    }
    return result;
}

bool CChangeset::isChanged(const RevNumType revisionNumber, const String& codeElementName) const
{
    return m_changes->getRevision(revisionNumber)->at((*m_codeElements)[codeElementName]);
}

StringVector CChangeset::getCodeElementNames(const RevNumType revisionNumber) const
{
    StringVector result;
    IBitList* bitList = m_changes->getRevision(revisionNumber);

    for(IndexType i = 0; i < bitList->size(); i++) {
        if((*bitList)[i]) {
            result.push_back(m_codeElements->getValue(i));
        }
    }

    return result;
}

const IIDManager& CChangeset::getCodeElements() const
{
    return *m_codeElements;
}

void CChangeset::toggle(const RevNumType revisionNumber, const String& codeElementName)
{
    m_changes->getRevision(revisionNumber)->set(m_codeElements->getID(codeElementName), !isChanged(revisionNumber, codeElementName));
}

void CChangeset::setChange(const RevNumType revisionNumber, const String& codeElementName, const bool isChanged)
{
    if (!m_codeElements->containsValue(codeElementName)) {
        throw CException("soda::CChangeset::setChange()", "Changeset does not contain item!");
    }
    m_changes->getRevision(revisionNumber)->set(m_codeElements->getID(codeElementName), isChanged);
}

void CChangeset::addOrSetChange(const RevNumType revisionNumber, const String& codeElementName, const bool isChanged)
{
    if(!m_changes->revisionExists(revisionNumber)) {
        addRevision(revisionNumber);
    }
    if (!m_codeElements->containsValue(codeElementName)) {
        addCodeElement(codeElementName);
    }

    m_changes->getRevision(revisionNumber)->set(m_codeElements->getID(codeElementName), isChanged);
}

void CChangeset::addRevision(const RevNumType revisionNumber, const IndexType numberOfCodeElements)
{
    if(m_changes->revisionExists(revisionNumber))
        return;

    IBitList* bitList = new CIndexBitList(numberOfCodeElements);
    m_changes->addRevision(revisionNumber, bitList);
}

void CChangeset::addRevision(const RevNumType revisionNumber)
{
    addRevision(revisionNumber, m_codeElements->size());
}

void CChangeset::addRevision(const IntVector& revisionNumbers)
{
    for(IntVector::const_iterator it = revisionNumbers.begin(); it != revisionNumbers.end(); ++it) {
        addRevision(*it);
    }
}

void CChangeset::addCodeElement(const String& codeElementName)
{
    m_codeElements->add(codeElementName);
    IntVector revNums = m_changes->getRevisionNumbers();
    for(IndexType i = 0; i < revNums.size(); i++) {
        m_changes->getRevision(revNums[i])->push_back(false);
    }
}

void CChangeset::addCodeElement(const IIDManager& codeElements)
{
    addCodeElement(codeElements.getValueList());
}

void CChangeset::addCodeElement(const StringVector& codeElements)
{
    for(StringVector::const_iterator it = codeElements.begin(); it != codeElements.end(); ++it) {
        addCodeElement(*it);
    }
}

IntVector CChangeset::getRevisions() const
{
    return m_changes->getRevisionNumbers();
}

void CChangeset::removeRevision(const RevNumType revNum)
{
    if (!exists(revNum))
        return;

    delete m_changes->getRevision(revNum);
    m_changes->getRevision(revNum) = NULL;
    m_changes->removeRevision(revNum);
}

void CChangeset::removeRevision(const IntVector& revNums)
{
    for(IntVector::const_iterator it = revNums.begin(); it != revNums.end(); ++it) {
        removeRevision(*it);
    }
}

void CChangeset::removeCodeElement(const String& codeElementName)
{
    IndexType index = (*m_codeElements)[codeElementName];
    IntVector revNums = m_changes->getRevisionNumbers();
    for(IndexType i = 0; i < revNums.size(); i++) {
        m_changes->getRevision(revNums[i])->erase(index);
    }
    m_codeElements->remove(codeElementName);
}

void CChangeset::refitSize()
{
    IntVector revNums = m_changes->getRevisionNumbers();
    IndexType metNums = m_codeElements->size();
    for(IndexType i = 0; i < revNums.size(); i++) {
        for(IndexType j = m_changes->getRevision(revNums[i])->size(); j < metNums; j++) {
            m_changes->getRevision(revNums[i])->push_back(false);
        }
    }
}

void CChangeset::addCodeElementName(const String& codeElementName)
{
    m_codeElements->add(codeElementName);
}

void CChangeset::save(io::CBinaryIO *out) const
{
    m_codeElements->save(out, io::CSoDAio::PRLIST);
    saveRevisionTable(out, io::CSoDAio::CHANGESET);
}

void CChangeset::load(io::CSoDAio *in)
{
    bool isPR = false;
    bool isCH = false;

    while(in->nextChunkID()) {
        if(in->getChunkID() == io::CSoDAio::PRLIST) {
            m_codeElements->load(in);
            isPR = true;
        } else if(in->getChunkID() == io::CSoDAio::CHANGESET) {
            loadRevisionTable(in);
            isCH = true;
        }
    }

    if(!isPR || !isCH) {
        throw CException("soda::CChangeset::load()","There is no Changeset info in this file!");
    }
}

void CChangeset::save(const char * filename) const
{
    io::CSoDAio *out;
    out = new io::CSoDAio(filename, io::CBinaryIO::omWrite);
    save(out);
    out->close();
}

void CChangeset::load(const char * filename)
{
    io::CSoDAio *in;
    in = new io::CSoDAio(filename, io::CBinaryIO::omRead);
    load(in);
    in->close();
}

void CChangeset::save(const String& filename) const
{
    save(filename.c_str());
}

void CChangeset::load(const String& filename)
{
    load(filename.c_str());
}

void CChangeset::saveRevisionTable(io::CBinaryIO* out, const io::CSoDAio::ChunkID chunk) const
{
    IntVector revs = m_changes->getRevisionNumbers();
    unsigned int revNumTypeLength = sizeof(RevNumType);
    unsigned long long int length = revs.size() * (revNumTypeLength + m_codeElements->size()) + 8;

    out->writeUInt4(chunk);
    out->writeULongLong8(length);

    out->writeUInt4(revs.size());
    out->writeUInt4(m_codeElements->size());
    for(IntVector::const_iterator it = revs.begin(); it!=revs.end(); it++) {
        //Write revision number
        RevNumType r = *it;
        out->writeData(&r, revNumTypeLength);

        //Write bitlist
        IBitList* bitlist = (*m_changes)[*it];
        for(IndexType i = 0; i < bitlist->size(); ++i) {
            out->writeBool1(bitlist->at(i));
        }
    }
}

void CChangeset::loadRevisionTable(io::CBinaryIO* in)
{
    unsigned int revSize = in->readUInt4();
    IndexType nOfCodeElements = in->readUInt4();

    for(RevNumType i = 0; i< revSize; i++) {
        //read revision number
        RevNumType rev;
        in->readData(&rev, sizeof(RevNumType));
        addRevision(rev, nOfCodeElements);

        //read bitlist;
        for(IndexType mid = 0; mid < nOfCodeElements; mid++) {
            (*m_changes)[rev]->set(mid, in->readBool1());
        }
    }
}

} //namespace soda
