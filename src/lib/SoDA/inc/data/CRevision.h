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

#ifndef CREVISION_H
#define CREVISION_H

#include "io/CSoDAio.h"
#include "data/SoDALibDefs.h"

namespace soda {

/**
 * @brief The CRevision class stores revision number, Type pairs in a map.
 */
template <class Type>
class CRevision {
public:

    /**
     * @brief Constructor, creates an empty CRevision.
     */
    CRevision() :
        m_data(new std::map<RevNumType, Type>())
    {}

    /**
     * @brief Destroys a CRevision object.
     */
    ~CRevision()
    {
        delete m_data;
    }

    /**
     * @brief Returns true if the given revision number is in the revision map.
     * @param rev  Specified revision number.
     * @return True if the given revision is in the revision map otherwise false.
     */
    inline bool revisionExists(const RevNumType rev) const
    {
        return m_data->count(rev) > 0;
    }

    /**
     * @brief Returns a reference to the value of the specified revision number.
     * @param rev  Specified revision number.
     * @return Reference to the value of the specified revision number.
     */
    inline Type& getRevision(const RevNumType rev)
    {
        return m_data->at(rev);
    }

    /**
     * @brief Adds a new element to the revision map with a specified revision number and data structure pair.
     * @param revNum  Specified revision number.
     * @param datastructure  Specified data structure.
     */
    inline void addRevision(const RevNumType revNum, Type& datastructure)
    {
        (*m_data)[revNum] = datastructure;
    }

    /**
     * @brief Removes the specified revision number from the revision map.
     * @param revNum  Specified revision number.
     */
    inline void removeRevision(const RevNumType revNum)
    {
        m_data->erase(revNum);
    }

    /**
     * @brief Returns the revision numbers stored in the revision map as a vector
     * @return The revision numbers stored in the revision map as a vector.
     */
    inline IntVector getRevisionNumbers() const
    {
        IntVector tmp(0);

        for(typename std::map<RevNumType, Type>::iterator it = m_data->begin(); it != m_data->end(); ++it) {
            tmp.push_back(it->first);
        }

        return tmp;
    }

    /**
     * @brief Returns a reference to the value of the specified revision number.
     * @param rev  Specified revision number.
     * @return Reference to the value of the specified revision number.
     */
    inline Type& operator[](const int rev)
    {
        return getRevision(rev);
    }

    /**
     * @brief Returns the number of elements in the revision map.
     * @return Number of elements in the revision map.
     */
    inline typename std::map<RevNumType, Type>::size_type size() const
    {
        return m_data->size();
    }

    /**
     * @brief Saves the content of a manager to the out.
     * @param out  Output stream.
     * @param chunk  Type of the data.
     */
    inline void save(io::CBinaryIO * out, const io::CSoDAio::ChunkID chunk = io::CSoDAio::REVISIONS) const
    {
        out->writeInt4(chunk);
        out->writeULongLong8(size()*(4+sizeof(Type)));

        out->writeUInt4(size());
        for(typename std::map<RevNumType, Type>::iterator it = m_data->begin(); it != m_data->end(); ++it) {
            out->writeUInt4(it->first);
            out->writeData(&(it->second), sizeof(Type));
        }
    }

    /**
     * @brief Loads the content of a revision map from the in.
     * @param in  Input stream.
     */
    inline void load(io::CBinaryIO * in) const
    {
        m_data->clear();

        for(RevNumType size = in->readUInt4(); size > 0; size--) {
            RevNumType key = in->readUInt4();
            Type value;
            in->readData(&value, sizeof(Type));
            (*m_data)[key] = value;
        }
    }

private:

    /**
     * @brief Stores revision number, type pairs.
     */
    std::map<RevNumType, Type>* m_data;

private:

    /**
     * @brief NIY Copy constructor.
     */
    CRevision(const CRevision&);

    /**
     * @brief NIY operator =.
     * @return Reference to a CRevision object.
     */
    CRevision& operator=(const CRevision&);
};

} // namespace soda

#endif /* CREVISION_H */
