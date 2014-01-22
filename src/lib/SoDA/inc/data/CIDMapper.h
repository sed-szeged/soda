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

#ifndef CIDMAPPER_H
#define CIDMAPPER_H

#include "io/CSoDAio.h"
#include "interface/IIDManager.h"

namespace soda {

/**
 * @brief The CIDMapper class stores id, string pairs in a global
 *          object where we have to convert the id to a global id.
 */
class CIDMapper : public IIDManager {
public:

    /**
     * @brief Constructor, creates a CIDMapper object with the specified IIDManager
     */
    CIDMapper(IIDManager *);

    /**
     * @brief Destroys a CIDMapper object
     */
    ~CIDMapper();

    /**
     * @brief Returns true if the ID is present in the global Manager.
     * @return true if the ID is present in the global Manager.
     */
    virtual bool containsValue(const String&) const;

    /**
     * @brief Returns the id of the value.
     * @return The id of the string value.
     * @throw std::out_of_range If the value is not present in the Manager.
     */
    virtual IndexType getID(const String&) const;

    /**
     * @brief Returns the value that belongs to the id.
     * @return The value that belongs to the id.
     * @throw std::out_of_range If the id is not present in the Manager.
     */
    virtual String getValue(const IndexType) const;

    /**
     * @brief Returns the list of the available ids.
     * @return The list of the available ids.
     */
    virtual IntVector getIDList() const;

    /**
     * @brief Returns the list of the available values.
     * @return The list of the available values.
     */
    virtual StringVector getValueList() const;

    /**
     * @brief Returns the value that belongs to the id.
     * @return The value that belongs to the id.
     */
    virtual String operator[](const IndexType) const;

    /**
     * @brief Returns the id of the value.
     * @return The id of the string value.
     */
    virtual IndexType operator[](const String&) const;

    /**
     * @brief Returns the last inserted id.
     * @return The last inserted id.
     * @throw std::length_error If the manager is empty.
     */
    virtual IndexType getLastIndex() const;

    /**
     * @brief Returns the number of elements in the mapper.
     * @return The number of elements in the mapper.
     */
    virtual IndexType size() const;

    /**
     * @brief Translate another id to a local id.
     * @return Local id.
     */
    virtual IndexType translateFromAnotherId(CIDMapper&, IndexType);

    /**
     * @brief Adds a value with the specified id.
     */
    virtual void add(const IndexType, const String&);

    /**
     * @brief Adds a value to the mapper as the last element.
     */
    virtual void add(const String&);

    /**
     * @brief Removes an element by its id.
     */
    virtual void remove(const IndexType);

    /**
     * @brief Removes an element by its value.
     */
    virtual void remove(const String&);

    /**
     * @brief Clears the manager.
     */
    virtual void clear();

    /**
     * @brief Writes the content of the mapper to the out.
     * @param out
     */
    virtual void save(io::CBinaryIO* out, const io::CSoDAio::ChunkID = io::CSoDAio::IDMANAGER) const;

    /**
     * @brief Loads the content of a mapper from in.
     * @param in
     */
    virtual void load(io::CBinaryIO* in);

protected:

    /**
     * @brief Type defition of map of IndexType, IndexType pairs.
     */
    typedef std::map<IndexType, IndexType> maptype;

    /**
     * @brief A global id to string manager object
     */
    IIDManager *m_globalIdManager;

    /**
     * @brief Map of global id, local id pairs.
     */
    maptype m_globalToLocal;

    /**
     * @brief Map of local id, global id pairs.
     */
    maptype m_localToGlobal;
};

} // namespace soda

#endif /* CIDMAPPER_H */
