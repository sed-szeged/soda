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

#ifndef IIDMANAGER_H
#define IIDMANAGER_H

#include "io/CSoDAio.h"

namespace soda {

/**
 * @brief Interface of an id to string value mapper.
 */
class IIDManager {
public:

    /**
     * @brief Returns true if the ID is present in the Manager.
     * @return true if the ID is present in the Manager.
     */
    virtual bool containsValue(const String&) const = 0;

    /**
     * @brief Returns the id of the value.
     * @return The id of the string value.
     * @throw std::out_of_range If the value is not present in the Manager.
     */
    virtual IndexType getID(const String&) const = 0;

    /**
     * @brief Returns the value that belongs to the id.
     * @return The value that belongs to the id.
     * @throw std::out_of_range If the id is not present in the Manager.
     */
    virtual String getValue(const IndexType) const = 0;

    /**
     * @brief Returns the list of the available ids.
     * @return The list of the available ids.
     */
    virtual IntVector getIDList() const = 0;

    /**
     * @brief Returns the list of the available values.
     * @return The list of the available values.
     */
    virtual StringVector getValueList() const = 0;

    /**
     * @brief Returns the value that belongs to the id.
     * @return The value that belongs to the id.
     * @throw std::out_of_range If the value is not present in the Manager.
     */
    virtual String operator[](const IndexType) const = 0;

    /**
     * @brief Returns the id of the value.
     * @return The id of the string value.
     * @throw std::out_of_range If the id is not present in the Manager.
     */
    virtual IndexType operator[](const String&) const = 0;

    /**
     * @brief Returns the last inserted id.
     * @return The last inserted id.
     * @throw std::length_error If the manager is empty.
     */
    virtual IndexType getLastIndex() const = 0;

    /**
     * @brief Returns the number of elements in the manager.
     * @return The number of elements in the manager.
     */
    virtual IndexType size() const = 0;

    /**
     * @brief Adds a value with the specified id.
     */
    virtual void add(const IndexType, const String&) = 0;

    /**
     * @brief Adds a value to the manager as the last element.
     */
    virtual void add(const String&) = 0;

    /**
     * @brief Removes an element by its id.
     */
    virtual void remove(const IndexType) = 0;

    /**
     * @brief Removes an element by its value.
     */
    virtual void remove(const String&) = 0;

    /**
     * @brief Clears the manager.
     */
    virtual void clear() = 0;

    /**
     * @brief Writes the content of the manager to the out.
     * @param out
     */
    virtual void save(io::CBinaryIO* out, const io::CSoDAio::ChunkID = io::CSoDAio::IDMANAGER) const = 0;

    /**
     * @brief Loads the content of a manager from in.
     * @param in
     */
    virtual void load(io::CBinaryIO* in) = 0;
};

} // namespace soda

#endif /* IIDMANAGER_H */
