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

#ifndef IBITMATRIX_H
#define IBITMATRIX_H

#include "io/CSoDAio.h"
#include "io/CBitReader.h"
#include "io/CBitWriter.h"
#include "IBitList.h"

namespace soda {

class IBitMatrixIterator;

/**
 * @brief The IBitMatrix class is an interface class for multiple bitlists represented as a matrix.
 */
class IBitMatrix {
public:

    /**
     * @brief Returns the number of rows of the matrix.
     * @return Number of rows of the matrix.
     */
    virtual IndexType getNumOfRows() const = 0;

    /**
     * @brief Returns the number of columns of the matrix.
     * @return Number of columns of the matrix.
     */
    virtual IndexType getNumOfCols() const = 0;

    /**
     * @brief Returns in the given parameter how many true elements are in each row of the matrix.
     * @param v  Vector reference.
     */
    virtual void rowCounts(std::vector<IndexType> &v) const = 0;

    /**
     * @brief Returns in the given parameter how many true elements are in each column of the matrix.
     * @param v  Vector reference.
     */
    virtual void colCounts(std::vector<IndexType> &v) const = 0;

    /**
     * @brief Returns the value of element at the given position in the matrix.
     * @param row  Row of the element in the matrix.
     * @param col  Column of the element in the matrix.
     * @return Value at the given position in the matrix.
     */
    virtual bool get(IndexType row, IndexType col) const = 0;

    /**
     * @brief Set value of the element to the given value at the given position in the matrix.
     * @param row  Row of the element in the matrix.
     * @param col  Column of the element in the matrix.
     * @param value  Value to be set.
     */
    virtual void set(IndexType row, IndexType col, bool value) = 0;

    /**
     * @brief Set the value of the element to the opposite at the row x col position in the matrix.
     * @param row  Row of the element in the matrix.
     * @param col  Column of the element in the matrix.
     */
    virtual void toggleValue(IndexType row, IndexType col) = 0;

    /**
     * @brief Resize the matrix.
     * @param newRow  New number of rows of the matrix.
     * @param newCol  New number of columns of the matrix.
     */
    virtual void resize(IndexType newRow, IndexType newCol) = 0;

    /**
     * @brief Set the object's attributes to default value.
     */
    virtual void clear() = 0;

    /**
     * @brief Returns the given row of the matrix as a bitlist.
     * @param row  Row number.
     * @return Row of the matrix as bitlist.
     */
    virtual IBitList& operator[](IndexType row) const = 0;

    /**
     * @brief Returns the given row of the matrix as a bitlist.
     * @param row  Row number.
     * @return Row of the matrix as a bitlist.
     */
    virtual IBitList& getRow(IndexType row) const = 0;

    /**
     * @brief Returns the given column of the matrix as a bitlist.
     * @param col  Column number.
     * @return Column of the matrix as a bitlist.
     */
    virtual IBitList& getCol(IndexType col) const = 0;

    /**
     * @brief Returns an iterator which points at the first element in the matrix.
     * @return Iterator which points at first element in the matrix.
     */
    virtual IBitMatrixIterator& begin() = 0;

    /**
     * @brief Returns an iterator which points at the last element in the matrix.
     * @return Iterator which points at the last element in the matrix.
     */
    virtual IBitMatrixIterator& end() = 0;

/******************
 * Inline methods *
 ******************/

    /**
     * @brief Set the same value for the entire matrix.
     * @param value  Value to be set.
     */
    virtual void setAll(const bool value = false)
    {
        for(IndexType i = 0; i < getNumOfRows(); ++i) {
            for(IndexType j = 0; j < getNumOfCols(); ++j) {
                set(i,j, value);
            }
        }
    }

    /**
     * @brief Writes the values and the type of the object to an output stream.
     * @param out  Output stream.
     * @param chunk  Type of the data.
     */
    virtual void save(io::CBinaryIO* out, const io::CSoDAio::ChunkID chunk = io::CSoDAio::BITMATRIX) const
    {
        IndexType rows = getNumOfRows();
        IndexType cols = getNumOfCols();

        //write ChunkID
        out->writeUInt4(chunk);
        //write length
        out->writeULongLong8(2*sizeof(IndexType) + io::CBitWriter::predictSize(rows*cols));
        //Write rows count of the matrix
        out->writeULongLong8(rows);
        //Write columns count of the matrix
        out->writeULongLong8(cols);

        //Write the elements
        io::CBitWriter bo = io::CBitWriter(out);
        for (IndexType i = 0; i < rows; ++i) {
            for (IndexType j = 0; j < cols; ++j) {
                bo.writeBit(get(i,j));
            }
        }
        bo.flush();
    }

    /**
     * @brief Reads data from the input stream.
     * @param in  Input stream.
     */
    virtual void load(io::CBinaryIO* in)
    {
        IndexType row = in->readULongLong8();
        IndexType col = in->readULongLong8();

        resize(row, col);
        setAll();

        io::CBitReader bi = io::CBitReader(in);
        for (IndexType i = 0; i < row; i++) {
            for (IndexType j = 0; j < col; j++) {
                set(i, j, bi.readBit());
            }
        }
        bi.reset();
    }

    /**
     * @brief Returns true if the two objects have the same size and values otherwise false.
     * @param rhs  Object to be compared.
     * @return True if the two objects have the same size and values otherwise false.
     */
    virtual bool operator==(const IBitMatrix& rhs) const
    {
        if(getNumOfRows() != rhs.getNumOfRows()) {
            return false;
        }
        for (IndexType i = 0; i < getNumOfRows(); ++i) {
            if((*this)[i] != rhs[i]) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Returns true if the two objects have different size and values otherwise false.
     * @param rhs  Object to be compared.
     * @return True if the two objects have different size and values otherwise false.
     */
    virtual bool operator!=(const IBitMatrix& rhs) const
    {
        if(getNumOfRows() != rhs.getNumOfRows()) {
            return true;
        }
        for (IndexType i = 0; i < getNumOfRows(); ++i) {
            if((*this)[i] != rhs[i]) {
                return true;
            }
        }
        return false;
    }
};

} // namespace soda

#endif /* IBITMATRIX_H */
