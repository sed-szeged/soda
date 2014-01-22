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

#ifndef IBITLIST_H
#define IBITLIST_H

#include "io/CSoDAio.h"
#include "io/CBitReader.h"
#include "io/CBitWriter.h"
#include "data/SoDALibDefs.h"

namespace soda {

class IBitListIterator;

/**
 * @brief The IBitList class is an interface class for bitlist representations.
 */
class IBitList
{
public:

    /**
     * @brief Returns the value of the first element in the vector.
     * @throw Exception if the vector is empty.
     * @return Value stored at the first element in the vector
     */
    virtual bool front() const = 0;

    /**
     * @brief Returns the value of the last element in the vector.
     * @throw Exception if the vector is empty.
     * @return Value stored at the last element in the vector.
     */
    virtual bool back() const = 0;

    /**
     * @brief Returns the value stored at the given position in the vector.
     * @param index  Position of the element.
     * @throw Exception if the parameter is out of bounds.
     * @return Value stored at the given position.
     */
    virtual bool at(IndexType index) const = 0;

    /**
     * @brief Adds a new element at the end of the vector.
     * @param value  Value to be stored at the new element.
     */
    virtual void push_back(bool value) = 0;

    /**
     * @brief Set the given value at the given position in the vector.
     * @param index  Position of the element.
     * @param valu  Value to be set at the given position.
     * @throw Exception if index is out of bounds.
     */
    virtual void set(IndexType index, bool value) = 0;

    /**
     * @brief Set the value to the opposite at the given position in the vector.
     * @param index  Position of the element.
     * @throw Exception if index is out of bounds.
     */
    virtual void toggleValue(IndexType index) = 0;

    /**
     * @brief Removes the last element from the vector.
     * @throw Exception if the vector is empty.
     */
    virtual void pop_back() = 0;

    /**
     * @brief Removes the first element from the vector.
     * @throw Exception if the vector is empty.
     */
    virtual void pop_front() = 0;

    /**
     * @brief Removes an element at the given position in the vector.
     * @param index  Position of the element.
     * @throw Exception if index is out of bounds.
     */
    virtual void erase(IndexType index) = 0;

    /**
     * @brief Change the size of the vector.
     * @param newSize  New size of the vector.
     */
    virtual void resize(IndexType newSize) = 0;

    /**
     * @brief Set the object's attributes to default value.
     */
    virtual void clear() = 0;

    /**
     * @brief Returns the size of the vector.
     * @return Size of the vector.
     */
    virtual IndexType size() const = 0;

    /**
     * @brief Returns the number of 1 values in the vector.
     * @return Number of 1 values in the vector.
     */
    virtual IndexType count() const = 0;

    /**
     * @brief Returns the value stored at the given position in the vector.
     * @param index  position of the element.
     * @return Value stored at the given position.
     */
    virtual bool operator[](IndexType index) const = 0;

    /**
     * @brief Returns an iterator which points at the first element in the vector.
     * @return Iterator which points at first element in the vector.
     */
    virtual IBitListIterator& begin() = 0;

    /**
     * @brief Returns an iterator which points at the last element in the vector.
     * @return Iterator which points at the last element in the vector.
     */
    virtual IBitListIterator& end() = 0;

/******************
 * Inline methods *
 ******************/

    /**
     * @brief Set the same value for the entire vector.
     * @param value  Value to be set.
     */
    virtual void setAll(const bool value = false)
    {
        for(IndexType i = 0; i < size(); ++i) {
            set(i, value);
        }
    }

    /**
     * @brief Writes the values and the type of the object to an output stream.
     * @param out  Output stream.
     * @param chunk  Type of the data.
     */
    virtual void save(io::CBinaryIO* out, const io::CSoDAio::ChunkID chunk = io::CSoDAio::BITLIST) const
    {
        IndexType count = size();
        //write ChunkID
        out->writeUInt4(chunk);
        //write length
        out->writeULongLong8(sizeof(IndexType) + io::CBitWriter::predictSize(count));
        //Write size of the list
        out->writeULongLong8(count);

        //Write the elements
        io::CBitWriter bo(out);
        for (IndexType i = 0; i < count; ++i) {
            bo.writeBit(at(i));
        }
        bo.flush();
    }

    /**
     * @brief Reads data from the input stream.
     * @param in  Input stream.
     */
    virtual void load(io::CBinaryIO* in)
    {
        IndexType size = in->readULongLong8();
        resize(size);
        setAll();

        io::CBitReader bi(in);
        for (IndexType i = 0; i < size; i++) {
            set(i, bi.readBit());
        }
        bi.reset();
    }

    /**
     * @brief Returns true if the two objects have the same size and values otherwise false.
     * @param rhs  Object to be compared.
     * @return True if the two objects have the same size and values otherwise false.
     */
    virtual bool operator==(const IBitList& rhs) const
    {
        if(size() != rhs.size()) {
            return false;
        }
        for (IndexType i = 0; i < size(); ++i) {
            if((*this)[i] != rhs[i]) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Rrue if the two objects have different size and values otherwise false.
     * @param rhs  Object to be compared.
     * @return True if the two objects have different size and values otherwise false.
     */
    virtual bool operator!=(const IBitList& rhs) const
    {
        if(size() != rhs.size()) {
            return true;
        }
        for (IndexType i = 0; i < size(); ++i) {
            if((*this)[i] != rhs[i]) {
                return true;
            }
        }
        return false;
    }
};

} // namespace soda

#endif /* IBITLIST_H */
