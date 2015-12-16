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
    CBitMatrix bitMatrix;

    EXPECT_EQ(0u, bitMatrix.getNumOfCols());
    EXPECT_EQ(0u, bitMatrix.getNumOfRows());
}

TEST(CBitMatrix, Constructor)
{
    IndexType n = 10;
    IndexType m = 10;
    CBitMatrix bitMatrix(n, m);

    EXPECT_NO_FATAL_FAILURE(bitMatrix.getRow(n-1));
    EXPECT_NO_FATAL_FAILURE(bitMatrix.getCol(m-1));

    EXPECT_EQ(10, bitMatrix.getNumOfRows());
    EXPECT_EQ(10, bitMatrix.getNumOfCols());
}

TEST(CBitMatrix, SetAndGet)
{
    IndexType n = 10;
    IndexType m = 10;

    CBitMatrix bitMatrix(n, m);

    EXPECT_ANY_THROW(bitMatrix.set(-1, 0, false));
    EXPECT_ANY_THROW(bitMatrix.set(n, 0, false));

    EXPECT_NO_THROW(bitMatrix.get(n-1, m-1));

    EXPECT_NO_THROW(bitMatrix.getCol(m-1));

    EXPECT_ANY_THROW(bitMatrix.getRow(n));
    EXPECT_ANY_THROW(bitMatrix.getRow(-1));
    EXPECT_NO_THROW(bitMatrix.getRow(n-1));

    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_NO_THROW(bitMatrix.set(i,j,true));
            EXPECT_TRUE(bitMatrix.get(i,j));
        }
        EXPECT_NO_THROW(bitMatrix.getRow(i));
    }

    for (IndexType j = 0; j < m; ++j) {
        EXPECT_NO_THROW(bitMatrix.getCol(j));
        IBitList& bl = bitMatrix.getCol(j);
        for (IndexType i = 0; i < n; ++i) {
            EXPECT_TRUE(bl.at(i));
        }
    }
}

TEST(CBitMatrix, ToggleValue)
{
    IndexType n = 10;
    IndexType m = 10;

    CBitMatrix bitMatrix(n, m);
    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_NO_THROW(bitMatrix.toggleValue(i,j));
            EXPECT_TRUE(bitMatrix.get(i,j));
            EXPECT_NO_THROW(bitMatrix.toggleValue(i,j));
            EXPECT_FALSE(bitMatrix.get(i,j));
        }
    }

    EXPECT_ANY_THROW(bitMatrix.toggleValue(n, 0));
    EXPECT_ANY_THROW(bitMatrix.toggleValue(-1, 0));
}

TEST(CBitMatrix, Extend)
{
    IndexType n = 10;
    IndexType m = 10;

    CBitMatrix bitMatrix(n, m);
    EXPECT_NO_THROW(bitMatrix.resize(n*2,m*2));

    EXPECT_NO_THROW(bitMatrix.get(n*2-1, m*2-1));
    EXPECT_NO_THROW(bitMatrix.getRow(n*2-1));
    EXPECT_NO_THROW(bitMatrix.getCol(m*2-1));

    EXPECT_EQ(n*2, bitMatrix.getNumOfRows());
    EXPECT_EQ(m*2, bitMatrix.getNumOfCols());

    EXPECT_NO_THROW(bitMatrix.resize(n/2,m/2));

    EXPECT_NO_THROW(bitMatrix.get(n/2-1, m/2-1));
    EXPECT_NO_THROW(bitMatrix.getRow(n/2-1));
    EXPECT_NO_THROW(bitMatrix.getCol(m/2-1));

    EXPECT_EQ(n/2, bitMatrix.getNumOfRows());
    EXPECT_EQ(m/2, bitMatrix.getNumOfCols());
}

TEST(CBitMatrix, Counts)
{
    IndexType n = 10;
    IndexType m = 10;

    CBitMatrix bitMatrix(n, m);
    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            bitMatrix.set(i, j, ((rand() % 2) ? true : false));
        }
    }

    IntVector v;
    bitMatrix.rowCounts(v);
    for (IndexType i = 0; i < n; ++i)
        EXPECT_EQ(v[i], bitMatrix.getRow(i).count());

    bitMatrix.colCounts(v);
    for (IndexType i = 0; i < m; ++i)
        EXPECT_EQ(v[i], bitMatrix.getCol(i).count());
}

TEST(CBitMatrix, ArrayIndexOperator)
{
    IndexType n = 10;
    IndexType m = 10;

    CBitMatrix bitMatrix(n, m);
    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_NO_THROW(bitMatrix.set(i, j, true));
            EXPECT_TRUE(bitMatrix.get(i, j));
        }
        IBitList& bl = bitMatrix.getRow(i);
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_TRUE(bl.at(j));
        }
    }
}

TEST(CBitMatrix, MatrixIterator)
{
    IndexType n = 10;
    IndexType m = 10;

    CBitMatrix bitMatrix(n, m);

    IndexType i = 0;
    for(IBitMatrixIterator& it = bitMatrix.begin(); it != bitMatrix.end();) {
        i++;
        EXPECT_FALSE(*it);

        if (i == 1)
            EXPECT_TRUE(it == bitMatrix.begin());

        switch(i) {
            case 10: ++it; break;
            case 20: it++; break;
            default: (i % 2) == 0 ? ++it : it++; break;
        }
    }

    EXPECT_EQ(n*m,i);
}

TEST(CBitMatrix, Clear)
{
    IndexType n = 10;
    IndexType m = 10;

    CBitMatrix bitMatrix(n, m);
    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_NO_THROW(bitMatrix.set(i, j, true));
        }
    }

    EXPECT_NO_THROW(bitMatrix.clear());

    EXPECT_EQ(0u, bitMatrix.getNumOfRows());
    EXPECT_EQ(0u, bitMatrix.getNumOfCols());

    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_ANY_THROW(bitMatrix.get(i,j));
        }
    }
}

TEST(CBitMatrix, SaveAndLoad)
{
    IndexType n = 100;
    IndexType m = 100;

    CBitMatrix bitMatrix(n, m);
    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_NO_THROW(bitMatrix.set(i,j,true));
        }
    }

    io::CSoDAio *io = new io::CSoDAio("sample/bitMatrixTest.saved", io::CBinaryIO::omWrite);
    EXPECT_NO_THROW(bitMatrix.save(io));
    delete io;

    CBitMatrix bitMatrix2(n/2, m/2);
    io = new io::CSoDAio("sample/bitMatrixTest.saved", io::CBinaryIO::omRead);
    EXPECT_TRUE(io->findChunkID(io::CSoDAio::BITMATRIX));
    EXPECT_NO_THROW(bitMatrix2.load(io));
    delete io;

    for (IndexType i = 0; i < n; ++i) {
        for (IndexType j = 0; j < m; ++j) {
            EXPECT_TRUE(bitMatrix2.get(i,j));
        }
    }

    EXPECT_TRUE(bitMatrix == bitMatrix2);
    bitMatrix2.resize(50, 50);
    EXPECT_TRUE(bitMatrix != bitMatrix2);
    bitMatrix2.resize(100, 50);
    EXPECT_TRUE(bitMatrix != bitMatrix2);
}
