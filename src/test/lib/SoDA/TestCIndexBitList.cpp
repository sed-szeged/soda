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

#include "data/CIndexBitList.h"
#include "interface/IIterators.h"
#include "gtest/gtest.h"
#include "exception/CException.h"

using namespace soda;

TEST(CIndexBitList, DefaultConstructor)
{
    CIndexBitList indexBitList;
    EXPECT_EQ(0u, indexBitList.size());
}

TEST(CIndexBitList, Constructor)
{
    CIndexBitList indexBitList(1000);
    EXPECT_EQ(1000u, indexBitList.size());
    EXPECT_NO_THROW(indexBitList.front());
    EXPECT_NO_THROW(indexBitList.back());
}

TEST(CIndexBitList, SetAndGet)
{
    int n = 10;
    CIndexBitList indexBitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(indexBitList.set(i,true));
        EXPECT_TRUE(indexBitList.at(i));
    }

    EXPECT_THROW(indexBitList.set(-1, true), CException);
    EXPECT_THROW(indexBitList.set(n, true), CException);

    EXPECT_NO_THROW(indexBitList.front());
    EXPECT_NO_THROW(indexBitList.back());
    EXPECT_TRUE(indexBitList.front());
    EXPECT_TRUE(indexBitList.back());

    EXPECT_ANY_THROW(indexBitList.at(-1));
    EXPECT_ANY_THROW(indexBitList.at(n));
}

TEST(CIndexBitList, AddAndRemove)
{
    IndexType n = 100;
    CIndexBitList indexBitList;
    for (IndexType i = 0; i < n; ++i) {
        EXPECT_NO_THROW(indexBitList.push_back(true));
        EXPECT_TRUE(indexBitList.at(i));
    }
    EXPECT_EQ(n, indexBitList.size());

    EXPECT_NO_THROW(indexBitList.pop_front());
    n-=1;
    EXPECT_EQ(n, indexBitList.size());

    EXPECT_NO_THROW(indexBitList.pop_back());
    n-=1;
    EXPECT_EQ(n, indexBitList.size());

    EXPECT_NO_THROW(indexBitList.erase(n/2));
    n-=1;
    EXPECT_EQ(n, indexBitList.size());

    EXPECT_NO_THROW(indexBitList.erase(n-1));
    n-=1;
    EXPECT_EQ(n, indexBitList.size());

    EXPECT_ANY_THROW(indexBitList.erase(-1));
    EXPECT_ANY_THROW(indexBitList.erase(n));
}

TEST(CIndexBitList, Toggle)
{
    int n = 10;
    CIndexBitList indexBitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(indexBitList.set(i,false));
        EXPECT_NO_THROW(indexBitList.toggleValue(i));
        EXPECT_TRUE(indexBitList.at(i));
        EXPECT_NO_THROW(indexBitList.toggleValue(i));
        EXPECT_FALSE(indexBitList.at(i));
    }

    EXPECT_ANY_THROW(indexBitList.toggleValue(-1));
    EXPECT_ANY_THROW(indexBitList.toggleValue(n));
}

TEST(CIndexBitList, ArrayIndexOperator)
{
    int n = 10;
    CIndexBitList indexBitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(indexBitList.set(i, true));
        EXPECT_TRUE(indexBitList[i]);
        EXPECT_NO_THROW(indexBitList.set(i, false));
        EXPECT_FALSE(indexBitList[i]);
    }
}

TEST(CIndexBitList, Iterators)
{
    int n = 100;
    CIndexBitList indexBitList(n);
    indexBitList.setAll(true);

    int i = 0;
    for(IBitListIterator& it = indexBitList.begin(); it != indexBitList.end();) {
        i++;
        EXPECT_TRUE(*it);

        if (i == 1)
            EXPECT_TRUE(it == indexBitList.begin());

        if ((i % 2) == 0)
            ++it;
        else
            it++;
    }

    EXPECT_EQ(n,i);
}

TEST(CIndexBitList, Clear)
{
    int n = 100;
    CIndexBitList indexBitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(indexBitList.set(i,true));
    }

    EXPECT_NO_THROW(indexBitList.clear());

    EXPECT_ANY_THROW(indexBitList.at(0));
    EXPECT_ANY_THROW(indexBitList.front());
    EXPECT_ANY_THROW(indexBitList.back());
    EXPECT_THROW(indexBitList.pop_back(), CException);
    EXPECT_THROW(indexBitList.pop_front(), CException);
}

TEST(CIndexBitList, Count)
{
    int n = 10;
    CIndexBitList indexBitList(n);
    for (IndexType i = 0; i < n; ++i) {
        if ((i % 2) == 0)
            indexBitList.set(i, false);
        else
            indexBitList.set(i, true);
    }

    EXPECT_EQ(5u, indexBitList.count());
}

TEST(CIndexBitList, Resize)
{
    CIndexBitList indexBitList(10);
    indexBitList.setAll(true);
    indexBitList.resize(5);
    EXPECT_EQ(5u, indexBitList.count());
}

TEST(CIndexBitList, SaveAndLoad)
{
    int n = 100;
    CIndexBitList indexBitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(indexBitList.set(i,true));
    }

    io::CSoDAio *io = new io::CSoDAio("sample/indexBitListTest.saved", io::CBinaryIO::omWrite);
    EXPECT_NO_THROW(indexBitList.save(io));
    delete io;

    CIndexBitList indexBitList2(n/2);
    io = new io::CSoDAio("sample/indexBitListTest.saved", io::CBinaryIO::omRead);
    EXPECT_TRUE(io->findChunkID(io::CSoDAio::BITLIST));
    EXPECT_NO_THROW(indexBitList2.load(io));
    delete io;

    for (int i = 0; i < n; ++i) {
        EXPECT_TRUE(indexBitList2.at(i));
    }
}
