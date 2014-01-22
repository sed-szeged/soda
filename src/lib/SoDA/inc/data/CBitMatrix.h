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

#ifndef CBITMATRIX_H
#define CBITMATRIX_H

#include "interface/IBitMatrix.h"
#include "data/CBitList.h"

namespace soda {

/**
 * @brief The CBitMatrix class stores multiple bitlists represented as a matrix.
 */
class CBitMatrix :
    public IBitMatrix
{
public:

    /**
     * @brief Constructor, creates an empty CBitMatrix object.
     */
    CBitMatrix();

    /**
     * @brief Creates a CBitMatrix object with a row x col matrix.
     * @param row  Number of rows of the matrix.
     * @param col  Number of columns of the matrix.
     */
    CBitMatrix(IndexType row, IndexType col);

    /**
     * @brief Destroys a CBitMatrix object.
     */
    ~CBitMatrix();

    /**
     * @brief Returns the number of rows of the matrix.
     * @return Number of rows of the matrix.
     */
    IndexType getNumOfRows() const;

    /**
     * @brief Returns the number of columns of the matrix.
     * @return Number of columns of the matrix.
     */
    IndexType getNumOfCols() const;

    /**
     * @brief Returns in the given parameter how many true elements are in each row of the matrix.
     * @param v  Vector reference.
     */
    void rowCounts(std::vector<IndexType> &v) const;

    /**
     * @brief Returns in the given parameter how many true elements are in each column of the matrix.
     * @param v  Vector reference.
     */
    void colCounts(std::vector<IndexType> &v) const;

    /**
     * @brief Returns the value of element at the given position in the matrix.
     * @param row  row of the element in the matrix.
     * @param col  column of the element in the matrix.
     * @return Value at the row x col position in the matrix.
     */
    bool get(IndexType row, IndexType col) const;

    /**
     * @brief Set value of the element to the given value at the given position in the matrix.
     * @param row  Row of the element in the matrix.
     * @param col  Column of the element in the matrix.
     * @param value  Value to be set.
     */
    void set(IndexType row, IndexType col, bool value);

    /**
     * @brief Set the value of the element to the opposite at the given position in the matrix.
     * @param row  Row of the element in the matrix.
     * @param col  Column of the element in the matrix.
     */
    void toggleValue(IndexType row, IndexType col);

    /**
     * @brief Resize the matrix. If the new size is smaller than
     *        the older one, then it will cut off the out of bound areas!
     * @param newRow  New number of rows of the matrix.
     * @param newCol  New number of columns of the matrix.
     */
    void resize(IndexType newRow, IndexType newCol);

    /**
     * @brief Set the object's attributes to default value.
     */
    void clear();

    /**
     * @brief Returns the given row of the matrix as a bitlist.
     * @param row  Row number.
     * @return Row of the matrix as a bitlist.
     */
    IBitList& getRow(IndexType row) const;

    /**
     * @brief Returns the given column of the matrix as a bitlist.
     * @param col  Column number.
     * @return Column of the matrix as a bitlist.
     */
    IBitList& getCol(IndexType col) const;

    /**
     * @brief Returns an iterator which points at the first element in the matrix.
     * @return Iterator which points at first element in the matrix.
     */
    IBitMatrixIterator& begin();

    /**
     * @brief Returns an iterator which points at the last element in the matrix.
     * @return Iterator which points at the last element in the matrix.
     */
    IBitMatrixIterator& end();

private:
    class MatrixIterator;

    /**
     * @brief NIY Copy constructor.
     */
    CBitMatrix(const CBitMatrix&);

    /**
     * @brief NIY operator =.
     * @return Reference to a CBitMatrix object.
     */
    CBitMatrix& operator=(const CBitMatrix&);

private:

    /**
     * @brief Number of rows of the matrix.
     */
    IndexType  m_row;

    /**
     * @brief Number of columns of the matrix.
     */
    IndexType  m_col;

    /**
     * @brief Dynamic array of bitlist objects
     */
    CBitList* m_data;

    /**
     * @brief An iterator pointer to the first element of the matrix.
     */
    MatrixIterator* m_beginIterator;

    /**
     * @brief An iterator pointer to the last element of the matrix.
     */
    MatrixIterator* m_endIterator;

    /**
     * @brief Returns the given row of the matrix as a bitlist.
     * @param row  Row number.
     * @return Row of the matrix as bitlist.
     */
    IBitList& operator[](IndexType row) const;
};

} // namespace soda

#endif /* CBITMATRIX_H */
