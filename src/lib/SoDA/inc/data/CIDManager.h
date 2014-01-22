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

#ifndef CIDMANAGER_H
#define CIDMANAGER_H

#include "io/CSoDAio.h"
#include "interface/IIDManager.h"

namespace soda {

/**
 * @brief The CIDManager class manages maps of id, string and string, id pairs.
 */
class CIDManager : public IIDManager {
public:

    /**
     * @brief Constructor, creates an empty CIDManager.
     */
    CIDManager();

    /**
     * @brief Constructor, creates a CIDManager object and fills the id, string pairs from the specified strings.
     */
    CIDManager(const StringVector&);

    /**
     * @brief Constructor, creates a CIDManager object from the specified id, string pairs.
     */
    CIDManager(StringMap&);

    /**
     * @brief Constructor, creates a CIDManager object from the specified string, id pairs.
     */
    CIDManager(IdxStrMap&);

    /**
     * @brief Destroys the CIDManager object.
     */
    ~CIDManager();

    /**
     * @brief Returns true if the ID is present in the Manager.
     * @return true if the ID is present in the Manager.
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
     * @throw std::out_of_range If the index is not present in the Manager.
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
     * @throw std::out_of_range If the value is not present in the Manager.
     */
    virtual String operator[](const IndexType) const;

    /**
     * @brief Returns the id of the value.
     * @return The id of the string value.
     * @throw std::out_of_range If the id is not present in the Manager.
     */
    virtual IndexType operator[](const String&) const;

    /**
     * @brief Returns the last inserted id.
     * @return The last inserted id.
     * @throw std::length_error If the manager is empty.
     */
    virtual IndexType getLastIndex() const;

    /**
     * @brief Returns the number of elements in the manager.
     * @return The number of elements in the manager.
     */
    virtual IndexType size() const;

    /**
     * @brief Adds a value with the specified id.
     */
    virtual void add(const IndexType, const String&);

    /**
     * @brief Adds a value to the manager as the last element.
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
     * @brief Writes the content of the manager to the out.
     * @param out
     */
    virtual void save(io::CBinaryIO* out, const io::CSoDAio::ChunkID = io::CSoDAio::IDMANAGER) const;

    /**
     * @brief Loads the content of a manager from in.
     * @param in
     */
    virtual void load(io::CBinaryIO* in);

protected:

    /**
     * @brief Stores id, string pairs
     */
    StringMap m_data;

    /**
     * @brief Stores string, id pairs
     */
    IdxStrMap m_adat;
};

} // namespace soda

#endif /* CIDMANAGER_H */
