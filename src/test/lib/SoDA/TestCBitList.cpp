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

#include "data/CBitList.h"
#include "interface/IIterators.h"
#include "gtest/gtest.h"

using namespace soda;

TEST(CBitList, DefaultConstructor)
{
    CBitList vectorBitList;
    EXPECT_EQ(0u, vectorBitList.size());
}

TEST(CBitList, Constructor)
{
    CBitList vectorBitList(1000);
    EXPECT_EQ(1000u, vectorBitList.size());
    EXPECT_NO_THROW(vectorBitList.front());
    EXPECT_NO_THROW(vectorBitList.back());
}

TEST(CBitList, SetAndGet)
{
    int n = 10;
    CBitList vectorBitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(vectorBitList.set(i,true));
        EXPECT_TRUE(vectorBitList.at(i));
    }

    EXPECT_NO_THROW(vectorBitList.front());
    EXPECT_NO_THROW(vectorBitList.back());
    EXPECT_TRUE(vectorBitList.front());
    EXPECT_TRUE(vectorBitList.back());

    EXPECT_ANY_THROW(vectorBitList.at(-1));
    EXPECT_ANY_THROW(vectorBitList.at(n));
}

TEST(CBitList, AddAndRemove)
{
    IndexType n = 100;
    CBitList vectorBitList;
    for (IndexType i = 0; i < n; ++i) {
        EXPECT_NO_THROW(vectorBitList.push_back(true));
        EXPECT_TRUE(vectorBitList.at(i));
    }
    EXPECT_EQ(n, vectorBitList.size());

    EXPECT_NO_THROW(vectorBitList.pop_front());
    n-=1;
    EXPECT_EQ(n, vectorBitList.size());

    EXPECT_NO_THROW(vectorBitList.pop_back());
    n-=1;
    EXPECT_EQ(n, vectorBitList.size());

    EXPECT_NO_THROW(vectorBitList.erase(n/2));
    n-=1;
    EXPECT_EQ(n, vectorBitList.size());

    EXPECT_ANY_THROW(vectorBitList.erase(-1));
    EXPECT_ANY_THROW(vectorBitList.erase(n));
}

TEST(CBitList, Toggle)
{
    int n = 10;
    CBitList vectorBitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(vectorBitList.set(i,false));
        EXPECT_NO_THROW(vectorBitList.toggleValue(i));
        EXPECT_TRUE(vectorBitList.at(i));
        EXPECT_NO_THROW(vectorBitList.toggleValue(i));
        EXPECT_FALSE(vectorBitList.at(i));
    }

    EXPECT_ANY_THROW(vectorBitList.toggleValue(-1));
    EXPECT_ANY_THROW(vectorBitList.toggleValue(n));
}

TEST(CBitList, ArrayIndexOperator)
{
    int n = 10;
    CBitList vectorBitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(vectorBitList.set(i, true));
        EXPECT_TRUE(vectorBitList[i]);
        EXPECT_NO_THROW(vectorBitList.set(i, false));
        EXPECT_FALSE(vectorBitList[i]);
    }
}

TEST(CBitList, Iterators)
{
    int n = 100;
    CBitList vectorBitList(n);
    int i = 0;
    for(IBitListIterator& it = vectorBitList.begin(); it != vectorBitList.end(); it++) {
        EXPECT_NO_THROW(vectorBitList.set(i,false));
        i++;
        EXPECT_FALSE(*it);
    }

    EXPECT_EQ(n,i);
}

TEST(CBitList, Clear)
{
    int n = 100;
    CBitList vectorBitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(vectorBitList.set(i,true));
    }

    EXPECT_NO_THROW(vectorBitList.clear());

    EXPECT_ANY_THROW(vectorBitList.at(0));
    EXPECT_ANY_THROW(vectorBitList.front());
    EXPECT_ANY_THROW(vectorBitList.back());
}

TEST(CBitList, SaveAndLoad)
{
    int n = 100;
    CBitList vectorBitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(vectorBitList.set(i,true));
    }

    io::CSoDAio *io = new io::CSoDAio("sample/vectorBitListTest.saved", io::CBinaryIO::omWrite);
    EXPECT_NO_THROW(vectorBitList.save(io));
    delete io;

    io = new io::CSoDAio("sample/vectorBitListTest.saved", io::CBinaryIO::omRead);
    CBitList vectorBitList2(n/2);
    EXPECT_TRUE(io->findChunkID(io::CSoDAio::BITLIST));
    EXPECT_NO_THROW(vectorBitList2.load(io));
    delete io;

    for (int i = 0; i < n; ++i) {
        EXPECT_TRUE(vectorBitList2.at(i));
    }
}

TEST(CBitList, Resize_Count)
{
    IndexType n = 100;
    CBitList vectorBitList;

    for(IndexType i = 0; i < n; ++i){
        EXPECT_NO_THROW(vectorBitList.push_back(true));
        EXPECT_TRUE(vectorBitList.at(i));
    }

    EXPECT_EQ(100u, vectorBitList.count());
    EXPECT_NO_THROW(vectorBitList.resize(104));
    EXPECT_EQ(104u, vectorBitList.size());
    EXPECT_EQ(100u, vectorBitList.count());
}
