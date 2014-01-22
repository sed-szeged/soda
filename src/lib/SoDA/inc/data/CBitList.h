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

#ifndef CBITLIST_H
#define CBITLIST_H

#include "interface/IBitList.h"

namespace soda {

/**
 * @brief The CBitList class stores list of bit values represented as bool values in a vector.
 */
class CBitList :
    public IBitList {

public:

    /**
     * @brief Contructor, creates an empty CBitList object.
     */
    CBitList();

    /**
     * @brief Creates a CBitList ojbect and contructs a vector with size elements.
     * @param size  Size of the vector.
     */
    CBitList(IndexType size);

    /**
     * @brief Destorys a CBitList object.
     */
    ~CBitList();

    /**
     * @brief Returns the value of the first element in the vector.
     * @throw Exception if the vector is empty.
     * @return Value stored at the first element in the vector.
     */
    bool front() const;

    /**
     * @brief Returns the value of the last element in the vector.
     * @throw Exception if the vector is empty.
     * @return Value stored at the last element in the vector.
     */
    bool back() const;

    /**
     * @brief Returns the value stored at given position in the vector.
     * @param index  Position of the element.
     * @throw Exception if index is out of bounds.
     * @return Value stored at the given position.
     */
    bool at(IndexType index) const;

    /**
     * @brief Adds a new element at the end of the vector.
     * @param value  Value to be stored in the new element.
     */
    void push_back(bool value);

    /**
     * @brief Set the value at the given position in the vector.
     * @param index  Position of the element.
     * @param value  Value to be set at index position.
     * @throw Exception if index is out of bounds.
     */
    void set(IndexType index, bool value);

    /**
     * @brief Set the value to the opposite at the given position in the vector.
     * @param index  Position of the element.
     * @throw Exception if index is out of bounds.
     */
    void toggleValue(IndexType index);

    /**
     * @brief Removes the last element in the vector.
     * @throw Exception if the vector is empty.
     */
    void pop_back();

    /**
     * @brief Removes the first element in the vector.
     * @throw Exception if the vector is empty.
     */
    void pop_front();

    /**
     * @brief Removes an element at the given position from the vector.
     * @param index  Position of the element.
     * @throw Exception if index is out of bounds.
     */
    void erase(IndexType index);

    /**
     * @brief Change the size of the vector.
     * @param newSize  New size of the vector.
     */
    void resize(IndexType newSize);

    /**
     * @brief Set the object's attributes to default value.
     */
    void clear();

    /**
     * @brief Returns the value stored at the given position in the vector.
     * @param index  Position of the element.
     * @return Value stored at the given position.
     */
    bool operator[](IndexType index) const;

    /**
     * @brief Returns the size of the vector.
     * @return Size of the vector.
     */
    IndexType size() const;

    /**
     * @brief Returns the number of 1 elements in the vector.
     * @return Number of 1 elements.
     */
    IndexType count() const;

    /**
     * @brief Returns an iterator which points at the first element in the vector.
     * @return Iterator which points at the first element in the vector.
     */
    IBitListIterator& begin();

    /**
     * @brief Returns an iterator which points at the last element in the vector.
     * @return Iterator which points at the last element in the vector.
     */
    IBitListIterator& end();

private:
    class ListIterator;

    /**
     * @brief NIY Copy constructor.
     */
    CBitList(const CBitList&);

    /**
     * @brief NIY operator =.
     * @return Reference to a CBitList object.
     */
    CBitList& operator=(const CBitList&);

private:

    /**
     * @brief Bitlist represented as bool values.
     */
    std::vector<bool>* m_data;

    /**
     * @brief An iterator pointer to the first element of the vector.
     */
    ListIterator* m_beginIterator;

    /**
     * @brief An iterator pointer to the last element of the vector.
     */
    ListIterator* m_endIterator;

    /**
     * @brief Number of 1 elements in the vector.
     */
    IndexType m_count;
};

} // namespace soda

#endif /* CBITLIST_H */
