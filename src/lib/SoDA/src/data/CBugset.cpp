/*
 * Copyright (C): 2015 Department of Software Engineering, University of Szeged
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

#include <stdexcept>

#include "data/CBugset.h"
#include "data/CIndexBitList.h"
#include "data/CIDManager.h"
#include "exception/CException.h"

namespace soda {

CBugset::CBugset() :
    m_codeElements(new CIDManager()),
    m_revisions(new CIDManager()),
    m_reports(new ReportMap()),
    m_reportDatas(new ReportDatas()),
    m_createCodeElements(true),
    m_createReports(true)
{ }

CBugset::CBugset(IIDManager* codeElements) :
    m_codeElements(codeElements),
    m_revisions(new CIDManager()),
    m_reports(new ReportMap()),
    m_reportDatas(new ReportDatas()),
    m_createCodeElements(false),
    m_createReports(true)
{ }

CBugset::CBugset(IIDManager* codeElements, IIDManager* revisions, ReportMap* reports, ReportDatas* data) :
    m_codeElements(codeElements),
    m_revisions(revisions),
    m_reports(reports),
    m_reportDatas(data),
    m_createCodeElements(false),
    m_createReports(false)
{ }

CBugset::~CBugset()
{
    if(m_createReports) {
        delete m_reports;
        m_reports = NULL;
        delete m_revisions;
        m_revisions = NULL;
        delete m_reportDatas;
        m_reportDatas = NULL;
    }
    if(m_createCodeElements) {
        delete m_codeElements;
        m_codeElements = NULL;
    }
}

CodeElementReports& CBugset::operator[](const String& revisionNumber) const
{
    return (*m_reports)[m_revisions->getID(revisionNumber)];
}

CodeElementReports& CBugset::at(const String& revisionNumber) const
{
    return m_reports->at(m_revisions->getID(revisionNumber));
}

bool CBugset::exists(const String& revisionNumber) const
{
    return m_reports->count(m_revisions->getID(revisionNumber)) != 0;
}

StringVector CBugset::getRevisions(const String& codeElementName) const
{
    StringVector result;
    IntVector revNums = m_revisions->getIDList();
    for(IndexType i = 0; i < revNums.size(); i++) {
        if ((*m_reports)[revNums[i]].find((*m_codeElements)[codeElementName]) != (*m_reports)[revNums[i]].end())
            result.push_back(m_revisions->getValue(revNums[i]));
    }
    return result;
}

bool CBugset::containsData(const String& revisionNumber, const String& codeElementName) const
{
    return (*m_reports)[m_revisions->getID(revisionNumber)].count((*m_codeElements)[codeElementName]) != 0;
}

Report const& CBugset::getReportInformations(String const& revisionNumber, String const& codeElementName) const
{
    return (*m_reportDatas)[(*m_reports)[m_revisions->getID(revisionNumber)].at((*m_codeElements)[codeElementName])];
}

StringVector CBugset::getCodeElementNames(const String& revisionNumber) const
{
    StringVector result;
    CodeElementReports& reports = (*m_reports)[m_revisions->getID(revisionNumber)];
    for (CodeElementReports::const_iterator it = reports.begin(); it != reports.end(); ++it) {
        result.push_back(m_codeElements->getValue(it->first));
    }

    return result;
}

const IIDManager& CBugset::getCodeElements() const
{
    return *m_codeElements;
}

void CBugset::addReported(const String& revisionNumber, const String& codeElementName, RevNumType const reportId, Report& data)
{
    if (!m_revisions->containsValue(revisionNumber)) {
        addRevision(revisionNumber);
    }

    if (!m_codeElements->containsValue(codeElementName)) {
        addCodeElement(codeElementName);
    }

    if (!m_reportDatas->count(reportId)) {
        (*m_reportDatas)[reportId] = data;
    }

    addReported(m_revisions->getID(revisionNumber), m_codeElements->getID(codeElementName), reportId);
}

void CBugset::addReported(RevNumType const revisionNumber, RevNumType const codeElementName, RevNumType const reportId)
{
    (*m_reports)[revisionNumber].insert(std::make_pair(codeElementName, reportId));
}

void CBugset::addRevision(const String& revisionNumber)
{
    m_revisions->add(revisionNumber);
}

void CBugset::addRevision(const StringVector& revisionNumbers)
{
    for(StringVector::const_iterator it = revisionNumbers.begin(); it != revisionNumbers.end(); ++it) {
        addRevision(*it);
    }
}

void CBugset::addCodeElement(const String& codeElementName)
{
    m_codeElements->add(codeElementName);
}

void CBugset::addCodeElement(const IIDManager& codeElements)
{
    addCodeElement(codeElements.getValueList());
}

void CBugset::addCodeElement(const StringVector& codeElements)
{
    for(StringVector::const_iterator it = codeElements.begin(); it != codeElements.end(); ++it) {
        addCodeElement(*it);
    }
}

StringVector CBugset::getRevisions() const
{
    return m_revisions->getValueList();
}

void CBugset::removeRevision(const String& revNum)
{
    RevNumType rev = m_revisions->getID(revNum);
    if (!m_reports->count(rev))
        return;

    m_reports->erase(rev);
    m_revisions->remove(revNum);
}

void CBugset::removeRevision(const StringVector& revNums)
{
    for(StringVector::const_iterator it = revNums.begin(); it != revNums.end(); ++it) {
        removeRevision(*it);
    }
}

void CBugset::removeCodeElement(const String& codeElementName)
{
    IndexType index = (*m_codeElements)[codeElementName];
    for (ReportMap::iterator it = m_reports->begin(); it != m_reports->end(); ++it) {
        if (it->second.count(index))
            it->second.erase(index);
    }
    m_codeElements->remove(codeElementName);
}

void CBugset::addCodeElementName(const String& codeElementName)
{
    m_codeElements->add(codeElementName);
}

void CBugset::save(io::CBinaryIO *out) const
{
    m_codeElements->save(out, io::CSoDAio::PRLIST);
    m_revisions->save(out, io::CSoDAio::REVLIST);
    saveRevisionTable(out, io::CSoDAio::BUGSET);
}

void CBugset::load(io::CSoDAio *in)
{
    bool isPR = false;
    bool isRL = false;
    bool isBS = false;

    while(in->nextChunkID()) {
        if (in->getChunkID() == io::CSoDAio::PRLIST) {
            m_codeElements->load(in);
            isPR = true;
        } else if (in->getChunkID() == io::CSoDAio::REVLIST) {
            m_revisions->load(in);
            isRL = true;
        } else if(in->getChunkID() == io::CSoDAio::BUGSET) {
            loadRevisionTable(in);
            isBS = true;
        }
    }

    if(!isPR || !isRL || !isBS) {
        throw CException("soda::CBugset::load()","There is no Bugset info in this file!");
    }
}

void CBugset::save(const char * filename) const
{
    io::CSoDAio *out = new io::CSoDAio(filename, io::CBinaryIO::omWrite);
    save(out);
    delete out;
}

void CBugset::load(const char * filename)
{
    io::CSoDAio *in = new io::CSoDAio(filename, io::CBinaryIO::omRead);
    load(in);
    delete in;
}

void CBugset::save(const String& filename) const
{
    save(filename.c_str());
}

void CBugset::load(const String& filename)
{
    load(filename.c_str());
}

void CBugset::saveRevisionTable(io::CBinaryIO* out, const io::CSoDAio::ChunkID chunk) const
{
    RevNumType numberOfReports = 0;
    for (ReportMap::const_iterator it = m_reports->begin(); it != m_reports->end(); ++it) {
        numberOfReports += it->second.size();
    }
    // 2x RevnumType, 2x time_t
    // revid, ceid, time, time
    unsigned long long int length = numberOfReports * (2 * sizeof(RevNumType) + 2 * sizeof(time_t)) + 4;

    out->writeUInt4(chunk);
    out->writeULongLong8(length);
    out->writeUInt4(m_reportDatas->size()); // number of reports
    for (std::map<RevNumType, Report>::const_iterator it = m_reportDatas->begin(); it != m_reportDatas->end(); ++it) {
        out->writeUInt4(it->first); // id
        out->writeULongLong8(it->second.fixTime); // fix time
        out->writeULongLong8(it->second.reportTime); // report time
    }
    out->writeUInt4(numberOfReports); // (revision, code element) -> report id
    for (ReportMap::const_iterator it = m_reports->begin(); it != m_reports->end(); ++it) {
        for (CodeElementReports::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            out->writeUInt4(it->first); // revid
            out->writeUInt4(it2->first); // ceid
            out->writeUInt4(it2->second); // report id
        }
    }
}

void CBugset::loadRevisionTable(io::CBinaryIO* in)
{
    RevNumType numberOfReportDatas = in->readUInt4();
    for (RevNumType i = 0; i < numberOfReportDatas; ++i) {
        RevNumType reportId = in->readUInt4();
        Report time;
        time.fixTime = in->readULongLong8();
        time.reportTime = in->readULongLong8();
        (*m_reportDatas)[reportId] = time;
    }

    RevNumType numberOfReports = in->readUInt4();
    for (RevNumType i = 0; i< numberOfReports; i++) {
        RevNumType revId = in->readUInt4();
        RevNumType ceId = in->readUInt4();
        RevNumType reportId = in->readUInt4();
        (*m_reports)[revId].insert(std::make_pair(ceId, reportId));
    }
}

} //namespace soda
