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

#include "io/CBinaryIO.h"
#include "data/CIDManager.h"

namespace soda {

CIDManager::CIDManager()
{
    m_data.clear();
    m_adat.clear();
}

CIDManager::CIDManager(const StringVector& valueList)
{
    m_data.clear();
    m_adat.clear();
    for (unsigned int i = 0; i < valueList.size(); ++i) {
        m_data.insert(std::pair<IndexType, String>(i,valueList[i]));
        m_adat.insert(std::pair<String, IndexType>(valueList[i],i));
    }
}

CIDManager::CIDManager(StringMap& map)
{
    m_data = map;
    m_adat.clear();
    for(StringMap::const_iterator i = m_data.begin(); i != m_data.end(); ++i) {
        m_adat.insert(std::pair<String, IndexType>(i->second,i->first));
    }

}

CIDManager::CIDManager(IdxStrMap& map)
{
    m_adat = map;
    m_data.clear();
    for(IdxStrMap::const_iterator i = m_adat.begin(); i != m_adat.end(); ++i) {
        m_data.insert(std::pair<IndexType, String>(i->second,i->first));
    }

}

CIDManager::~CIDManager()
{}

bool CIDManager::containsValue(const String &value) const
{
    if (m_adat.find(value) == m_adat.end()) {
        return false;
    }
    return true;
}

IndexType CIDManager::getID(const String& value) const
{
    IdxStrMap::const_iterator it=m_adat.find(value);
    if(it != m_adat.end()) {
        return it->second;
    }
    throw std::out_of_range("Value is not present in the Manager.");
}

String CIDManager::getValue(const IndexType id) const
{
    return m_data.at(id);
}

IntVector CIDManager::getIDList() const
{
    IntVector tmp;

    for(StringMap::const_iterator it = m_data.begin() ; it != m_data.end(); it++ ) {
        tmp.push_back(it->first);
    }

    return tmp;
}

StringVector CIDManager::getValueList() const
{
    StringVector tmp;
    for(StringMap::const_iterator it = m_data.begin() ; it != m_data.end(); it++ ) {
        tmp.push_back(it->second);
    }

    return tmp;

}

String CIDManager::operator[](const IndexType id) const
{
    return m_data.at(id);
}

IndexType CIDManager::operator[](const String& codeElementName) const
{
    return getID(codeElementName);
}

IndexType CIDManager::getLastIndex() const
{
    if (m_data.empty()) {
        throw std::length_error("The manager is empty.");
    }
    return m_data.rbegin()->first;
}

IndexType CIDManager::size() const
{
    return m_data.size();
}

void CIDManager::add(const IndexType id, const String& value)
{
    if(m_adat.find(value) == m_adat.end()) {
        m_data.insert(std::pair<IndexType, String>(id,value));
        m_adat.insert(std::pair<String, IndexType>(value,id));
    }
}

void CIDManager::add(const String& value)
{
    IndexType id = 0;
    try {
        id = getLastIndex();
        id++;
    } catch (std::length_error &e) {}

    add(id, value);
}

void CIDManager::remove(const IndexType id)
{
    StringMap::iterator it = m_data.find(id);
    if(it != m_data.end()) {
        m_adat.erase(it->second);
        m_data.erase(it);
    }
}

void CIDManager::remove(const String& value)
{
    IdxStrMap::iterator it = m_adat.find(value);
    if(it != m_adat.end()) {
        m_data.erase(it->second);
        m_adat.erase(it);
    }
}

void CIDManager::clear()
{
    m_data.clear();
    m_adat.clear();
}

void CIDManager::save(io::CBinaryIO *out, const io::CSoDAio::ChunkID chunk) const
{
    unsigned long long int length = sizeof(IndexType);
    for(StringMap::const_iterator it = m_data.begin() ; it != m_data.end(); ++it ) {
        length += sizeof(IndexType);
        length += it->second.length() + 1;
    }
    //write ChunkID
    out->writeUInt4(chunk);

    //write length
    out->writeULongLong8(length);

    //write the number of elements
    IndexType size = m_data.size();
    out->writeLongLong8(size);

    //write elements
    for(StringMap::const_iterator it = m_data.begin() ; it != m_data.end(); ++it ) {
        out->writeLongLong8(it->first);
        out->writeString(it->second);
    }
}

void CIDManager::load(io::CBinaryIO *in)
{
    m_data.clear();
    m_adat.clear();
    IndexType size = in->readLongLong8();

    for (IndexType i = 0; i < size; ++i) {
        IndexType index = in->readLongLong8();
        String value = in->readString();

        m_data.insert(std::pair<IndexType, String>(index, value));
        m_adat.insert(std::pair<String, IndexType>(value, index));
    }
}

} // namespace soda
