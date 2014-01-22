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
#include "interface/IBitList.h"
#include "interface/IIterators.h"
#include "gtest/gtest.h"

using namespace soda;

TEST(CBitMatrix, DefaultConstructor)
{
    CBitMatrix vectorBitMatrix;

    EXPECT_EQ(0u, vectorBitMatrix.getNumOfCols());
    EXPECT_EQ(0u, vectorBitMatrix.getNumOfRows());
}

TEST(CBitMatrix, Constructor)
{
    IndexType n = 1000;
    IndexType m = 1000;
    CBitMatrix vectorBitMatrix(n, m);

    EXPECT_NO_FATAL_FAILURE(vectorBitMatrix.getRow(n-1));
    EXPECT_NO_FATAL_FAILURE(vectorBitMatrix.getCol(m-1));

    EXPECT_EQ(1000u, vectorBitMatrix.getNumOfRows());
    EXPECT_EQ(1000u, vectorBitMatrix.getNumOfCols());
}

TEST(CBitMatrix, SetAndGet)
{
    IndexType n = 10;
    IndexType m = 10;

    CBitMatrix vectorBitMatrix(n, m);

    EXPECT_NO_THROW(vectorBitMatrix.get(n-1, m-1));
    EXPECT_NO_THROW(vectorBitMatrix.getRow(n-1));
    EXPECT_NO_THROW(vectorBitMatrix.getCol(m-1));

    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_NO_THROW(vectorBitMatrix.set(i,j,true));
            EXPECT_TRUE(vectorBitMatrix.get(i,j));
        }
        EXPECT_NO_THROW(vectorBitMatrix.getRow(i));
    }
    for (IndexType j = 0; j < m; ++j) {
        EXPECT_NO_THROW(vectorBitMatrix.getCol(j));
        IBitList& bl = vectorBitMatrix.getCol(j);
        for (IndexType i = 0; i < n; ++i) {
            EXPECT_TRUE(bl.at(i));
        }
    }
}

TEST(CBitMatrix, ToggleValue)
{
    IndexType n = 10;
    IndexType m = 10;

    CBitMatrix vectorBitMatrix(n, m);
    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_NO_THROW(vectorBitMatrix.toggleValue(i,j));
            EXPECT_TRUE(vectorBitMatrix.get(i,j));
            EXPECT_NO_THROW(vectorBitMatrix.toggleValue(i,j));
            EXPECT_FALSE(vectorBitMatrix.get(i,j));
        }
    }

}

TEST(CBitMatrix, Extend)
{
    IndexType n = 10;
    IndexType m = 10;

    CBitMatrix vectorBitMatrix(n, m);
    EXPECT_NO_THROW(vectorBitMatrix.resize(n*2,m*2));

    EXPECT_NO_THROW(vectorBitMatrix.get(n*2-1, m*2-1));
    EXPECT_NO_THROW(vectorBitMatrix.getRow(n*2-1));
    EXPECT_NO_THROW(vectorBitMatrix.getCol(m*2-1));

    EXPECT_EQ(n*2, vectorBitMatrix.getNumOfRows());
    EXPECT_EQ(m*2, vectorBitMatrix.getNumOfCols());

    EXPECT_NO_THROW(vectorBitMatrix.resize(n/2,m/2));

    EXPECT_NO_THROW(vectorBitMatrix.get(n/2-1, m/2-1));
    EXPECT_NO_THROW(vectorBitMatrix.getRow(n/2-1));
    EXPECT_NO_THROW(vectorBitMatrix.getCol(m/2-1));

    EXPECT_EQ(n/2, vectorBitMatrix.getNumOfRows());
    EXPECT_EQ(m/2, vectorBitMatrix.getNumOfCols());
}

TEST(CBitMatrix, ArrayIndexOperator)
{
    IndexType n = 10;
    IndexType m = 10;

    CBitMatrix vectorBitMatrix(n, m);
    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_NO_THROW(vectorBitMatrix.set(i, j, true));
            EXPECT_TRUE(vectorBitMatrix.get(i, j));
        }
        IBitList& bl = vectorBitMatrix.getRow(i);
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_TRUE(bl.at(j));
        }
    }
}

TEST(CBitMatrix, MatrixIterator)
{
    IndexType n = 10;
    IndexType m = 1;

    CBitMatrix vectorBitMatrix(n, m);

    IndexType i = 0;
    for(IBitMatrixIterator& it = vectorBitMatrix.begin() ; it != vectorBitMatrix.end(); ++it ) {
        i++;
        EXPECT_FALSE(*it);
    }

    EXPECT_EQ(n*m,i);
}

TEST(CBitMatrix, Clear)
{
    IndexType n = 10;
    IndexType m = 10;

    CBitMatrix vectorBitMatrix(n, m);
    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_NO_THROW(vectorBitMatrix.set(i, j, true));
        }
    }

    EXPECT_NO_THROW(vectorBitMatrix.clear());

    EXPECT_EQ(0u, vectorBitMatrix.getNumOfRows());
    EXPECT_EQ(0u, vectorBitMatrix.getNumOfCols());

    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_ANY_THROW(vectorBitMatrix.get(i,j));
        }
    }
}

TEST(CBitMatrix, SaveAndLoad)
{
    IndexType n = 100;
    IndexType m = 100;

    CBitMatrix vectorBitMatrix(n, m);
    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_NO_THROW(vectorBitMatrix.set(i,j,true));
        }
    }
    io::CSoDAio *io = new io::CSoDAio("sample/vectorBitMatrixTest.saved", io::CBinaryIO::omWrite);
    EXPECT_NO_THROW(vectorBitMatrix.save(io));
    delete io;

    CBitMatrix vectorBitMatrix2(n/2, m/2);
    io = new io::CSoDAio("sample/vectorBitMatrixTest.saved", io::CBinaryIO::omRead);
    EXPECT_TRUE(io->findChunkID(io::CSoDAio::BITMATRIX));
    EXPECT_NO_THROW(vectorBitMatrix2.load(io));
    delete io;

    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_TRUE(vectorBitMatrix2.get(i,j));
        }
    }
}
