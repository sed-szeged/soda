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

#include "data/CBitMatrix.h"
#include "exception/CException.h"
#include "interface/IIterators.h"

namespace soda {

/**
 * @brief The CBitMatrix::MatrixIterator class is an iterator for CBitMatrix.
 */
class CBitMatrix::MatrixIterator :
        public IBitMatrixIterator,
        public std::iterator<std::input_iterator_tag, CBitList*>
{
    CBitList* p;
    IndexType col;

public:
    MatrixIterator() : p(0), col(0) {}
    MatrixIterator(CBitList* x) : p(x), col(0) {}
    MatrixIterator(CBitList* x, int y) : p(x), col(y) {}
    MatrixIterator(const MatrixIterator& it) : p(it.p), col(it.col){}

    IBitMatrixIterator& operator++()
    {
        if(col != p->size()-1) {
            col++;
        } else {
            ++p;    // next row
            col = 0; // first element of the new row
        }
        return *this;
    }
    IBitMatrixIterator& operator++(int)
    {
        if(col != p->size()-1) {
            col++;
        } else {
            ++p;    // next row
            col = 0; // first element of the new row
        }
        return *this;
    }
    bool operator==(IBitMatrixIterator& rhs)
    {
        return (p == static_cast<CBitMatrix::MatrixIterator*>(&rhs)->p) &&
                (col == static_cast<CBitMatrix::MatrixIterator*>(&rhs)->col);
    }
    bool operator!=(IBitMatrixIterator& rhs)
    {
        return (p != static_cast<CBitMatrix::MatrixIterator*>(&rhs)->p) ||
                (col != static_cast<CBitMatrix::MatrixIterator*>(&rhs)->col);
    }
    bool operator*()
    {
        return (*p).at(col);
    }
};

CBitMatrix::CBitMatrix() :
    m_row(0),
    m_col(0),
    m_data(0),
    m_beginIterator(0),
    m_endIterator(0)
{}

CBitMatrix::CBitMatrix(IndexType rows, IndexType cols) :
        m_row(rows),
        m_col(cols),
        m_beginIterator(0),
        m_endIterator(0)
{
    m_data = new CBitList[rows]();

    for (IndexType i = 0; i < rows; i++) {
        for (IndexType j = 0; j < cols; j++) {
            m_data[i].push_back(false);
        }
    }
}

CBitMatrix::~CBitMatrix()
{
    delete [] m_data;
    delete m_beginIterator;
    delete m_endIterator;
}

IndexType CBitMatrix::getNumOfRows() const
{
    return m_row;
}

IndexType CBitMatrix::getNumOfCols() const
{
    return m_col;
}

void CBitMatrix::rowCounts(std::vector<IndexType> &v) const
{
    v.clear();
    v.resize(m_row);
    for (IndexType r = 0; r < m_row; r++) {
        v[r] = m_data[r].count();
    }
}

void CBitMatrix::colCounts(std::vector<IndexType> &v) const
{
    v.clear();
    v.resize(m_col);
    for (IndexType c = 0; c < m_col; c++) {
        IndexType sum = 0;
        for (IndexType r = 0; r < m_row; r++) {
            if (m_data[r][c]) {
                sum++;
            }
        }
        v[c] = sum;
    }
}

bool CBitMatrix::get(IndexType row, IndexType col) const
{
    if(row >= m_row || row < 0)
        throw CException("soda::CBitMatrix", "Index out of bound!");
    return m_data[row].at(col);
}

void CBitMatrix::set(IndexType row, IndexType col, bool value)
{
    if(row >= m_row || row < 0)
        throw CException("soda::CBitMatrix", "Index out of bound!");
    m_data[row].set(col, value);
}

void CBitMatrix::toggleValue(IndexType row, IndexType col)
{
    if(row >= m_row || row < 0)
        throw CException("soda::CBitMatrix", "Index out of bound!");
    if(m_data[row].at(col)) {
        m_data[row].set(col, false);
    } else {
        m_data[row].set(col, true);
    }
}

void CBitMatrix::resize(IndexType newRow, IndexType newCol)
{
    CBitList* tmp;
    tmp = new CBitList[newRow]();

    //Copy old elements
    IndexType rowCounter = m_row;
    IndexType colCounter = m_col;

    if(newRow < m_row)
        rowCounter = newRow;
    if(newCol < m_col)
        colCounter = newCol;

    for(IndexType i = 0; i < rowCounter; i++){
        for(IndexType j = 0; j < colCounter; j++) {
            tmp[i].push_back(m_data[i].at(j));
        }
        for(IndexType j = colCounter; j < newCol; j++) {
            tmp[i].push_back(false);
        }
    }

    //Extend the matrix
    for(IndexType i = rowCounter; i < newRow; i++){
        for(IndexType j = 0; j < newCol; j++){
            tmp[i].push_back(false);
        }
    }

    delete [] m_data;
    m_data = tmp;

    m_row = newRow;
    m_col = newCol;
}

void CBitMatrix::clear()
{
    delete [] m_data;
    m_data = 0;
    delete m_beginIterator;
    m_beginIterator = 0;
    delete m_endIterator;
    m_endIterator = 0;
    m_col = 0;
    m_row = 0;
}

IBitList& CBitMatrix::operator[](IndexType row) const
{
    return m_data[row];
}

IBitList& CBitMatrix::getRow(IndexType row) const
{
    if(row >= m_row || row < 0)
        throw CException("soda::CBitMatrix", "Index out of bound!");

    return m_data[row];
}

IBitList& CBitMatrix::getCol(IndexType col) const
{
    CBitList* column = new CBitList(m_row);

    for(IndexType i = 0; i < m_row; i++){
        column->set(i, get(i, col));
    }

    return *column;
}

IBitMatrixIterator& CBitMatrix::begin()
{
    delete m_beginIterator;
    m_beginIterator = new CBitMatrix::MatrixIterator(m_data);
    return *m_beginIterator;
}

IBitMatrixIterator& CBitMatrix::end()
{
    delete m_endIterator;
    m_endIterator = new CBitMatrix::MatrixIterator(m_data + m_row);
    return *m_endIterator;
}

} // namespace soda
