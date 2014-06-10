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

#ifndef CINDEXBITLIST_H
#define CINDEXBITLIST_H

#include "interface/IBitList.h"

namespace soda {

/**
 * @brief The CIndexBitList class stores list of bit values represented as bool values
 *          in a vector which stores the index positions of 1 values.
 */
class CIndexBitList : public IBitList {

public:

    /**
     * @brief Constructor, creates an empty CIndexBitList object.
     */
    CIndexBitList();

    /**
     * @brief Creates an empty CIndexBitList object.
     * @param size  Size of the vector.
     */
    CIndexBitList(IndexType size);

    /**
     * @brief Destorys a CIndexBitList object.
     */
    ~CIndexBitList();

    /**
     * @brief Returns the value of the first element in the vector.
     * @throw Exception if the vector is empty.
     * @return Value of the first element in the vector.
     */
    bool front() const;

    /**
     * @brief Returns the value of the last element in the vector
     * @throw Exception if the vector is empty.
     * @return Value of the last element in the vector.
     */
    bool back() const;

    /**
     * @brief Return the value at the given position of the vector.
     * @param pos  Index position.
     * @throw Exception if the parameter is out of bounds.
     * @return The value at the given position of the vector.
     */
    bool at(IndexType pos) const;

    /**
     * @brief Adds a new element at the end of the vector
     * @param value  Value to be stored in the new element.
     */
    void push_back(bool value);

    /**
     * @brief Set the value at the given position in the vector.
     * @param pos  Position of the element.
     * @param value  Value to be set at index position.
     * @throw Exception if index is out of bounds.
     */
    void set(IndexType pos, bool value);

    /**
     * @brief Set the element of the vector to the opposite at the given position.
     * @param pos  Index position
     * @throw Exception if index is out of bounds.
     */
    void toggleValue(IndexType pos);

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
     * @param newSize New size of the vector.
     */
    void resize(IndexType newSize);

    /**
     * @brief Set the object's attributes to default value.
     */
    void clear();

    /**
     * @brief Returns the number of elements in the vector.
     * @return Number of elements in the vector.
     */
    IndexType size() const;

    /**
     * @brief Returns the number of true elements in the vector.
     * @return Number of true elements in the vector.
     */
    IndexType count() const;

    /**
     * @brief Return the value at the given position of the vector.
     * @param pos  Position of the element.
     * @return Value at the given position of the vector.
     */
    bool operator[](IndexType pos) const;

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
    CIndexBitList(const CIndexBitList&);

    /**
     * @brief NIY operator =.
     * @return Reference to a CIndexBitList.
     */
    CIndexBitList& operator=(const CIndexBitList&);

private:
    /**
     * @brief Stores index positions of elements which are true in the vector.
     */
    std::vector<IndexType>* m_data;

    /**
     * @brief Size of the bit vector.
     */
    IndexType m_size;

    /**
     * @brief An iterator pointer to the first element of the vector.
     */
    ListIterator* m_beginIterator;

    /**
     * @brief An iterator pointer to the last element of the vector.
     */
    ListIterator* m_endIterator;

};

} // namespace soda

#endif /* CINDEXBITLIST_H */
