/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: László Langó <lango@inf.u-szeged.hu>
 *          Tamás Gergely <gertom@inf.u-szeged.hu>
 *          Havas Dávid <havasd@inf.u-szeged.hu>
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

#include "interface/IBitList.h"
#include "data/CRevision.h"

namespace soda {

template <typename T>
CRevision<T>::CRevision() :
    m_data(new std::map<RevNumType, T>())
{
}

template <typename T>
CRevision<T>::~CRevision()
{
    delete m_data;
}

template <typename T>
bool CRevision<T>::revisionExists(const RevNumType rev) const
{
    return m_data->count(rev) > 0;
}

template <typename T>
T& CRevision<T>::getRevision(const RevNumType rev)
{
    return m_data->at(rev);
}

template <typename T>
void CRevision<T>::addRevision(const RevNumType revNum, T& datastructure)
{
    (*m_data)[revNum] = datastructure;
}

template <typename T>
void CRevision<T>::removeRevision(const RevNumType revNum)
{
    m_data->erase(revNum);
}

template <typename T>
IntVector CRevision<T>::getRevisionNumbers() const
{
    IntVector tmp(0);

    for (typename std::map<RevNumType, T>::iterator it = m_data->begin(); it != m_data->end(); ++it) {
        tmp.push_back(it->first);
    }

    return tmp;
}

template <typename T>
T& CRevision<T>::operator[](const int rev)
{
    return getRevision(rev);
}

template <typename T>
typename std::map<RevNumType, T>::size_type CRevision<T>::size() const
{
    return m_data->size();
}

template <typename T>
void CRevision<T>::save(io::CBinaryIO *out, const io::CSoDAio::ChunkID chunk) const
{
    out->writeInt4(chunk);
    out->writeULongLong8(size() * (4 + sizeof(T)));

    out->writeUInt4(size());
    for (typename std::map<RevNumType, T>::iterator it = m_data->begin(); it != m_data->end(); ++it) {
        out->writeUInt4(it->first);
        out->writeData(&(it->second), sizeof(T));
    }
}

template <>
void CRevision<String>::save(io::CBinaryIO *out, const io::CSoDAio::ChunkID chunk) const
{
    RevNumType length = sizeof(RevNumType);
    for (std::map<RevNumType, String>::iterator it = m_data->begin(); it != m_data->end(); ++it) {
        length += sizeof(RevNumType);
        length += it->second.length() + 1;
    }

    // write chunk
    out->writeInt4(chunk);
    // write length of chunk
    out->writeULongLong8(length);
    // number of elements
    out->writeUInt4(size());

    for (std::map<RevNumType, String>::iterator it = m_data->begin(); it != m_data->end(); ++it) {
        out->writeUInt4(it->first);
        out->writeString(it->second);
    }
}

template <typename T>
void CRevision<T>::load(io::CBinaryIO *in) const
{
    m_data->clear();

    for (RevNumType size = in->readUInt4(); size > 0; size--) {
        RevNumType key = in->readUInt4();
        T value;
        in->readData(&value, sizeof(T));
        (*m_data)[key] = value;
    }
}

template <>
void CRevision<String>::load(io::CBinaryIO *in) const
{
    m_data->clear();

    RevNumType size = in->readUInt4();
    for (RevNumType i = 0; i < size; ++i) {
        RevNumType key = in->readUInt4();
        String value = in->readString();
        (*m_data)[key] = value;
    }
}

template class CRevision<String>;
template class CRevision<IBitList*>;
template class CRevision<IndexType>;

}
