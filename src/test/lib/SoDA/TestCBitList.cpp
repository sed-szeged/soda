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
    CBitList bitList;
    EXPECT_EQ(0u, bitList.size());
}

TEST(CBitList, Constructor)
{
    CBitList bitList(1000);
    EXPECT_EQ(1000u, bitList.size());
    EXPECT_NO_THROW(bitList.front());
    EXPECT_NO_THROW(bitList.back());
}

TEST(CBitList, SetAndGet)
{
    int n = 10;
    CBitList bitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(bitList.set(i,true));
        EXPECT_TRUE(bitList.at(i));
    }

    EXPECT_ANY_THROW(bitList.set(-1, false));
    EXPECT_ANY_THROW(bitList.set(n, false));

    EXPECT_NO_THROW(bitList.front());
    EXPECT_NO_THROW(bitList.back());
    EXPECT_TRUE(bitList.front());
    EXPECT_TRUE(bitList.back());

    EXPECT_ANY_THROW(bitList.at(-1));
    EXPECT_ANY_THROW(bitList.at(n));
}

TEST(CBitList, AddAndRemove)
{
    IndexType n = 100;
    CBitList bitList;
    for (IndexType i = 0; i < n; ++i) {
        EXPECT_NO_THROW(bitList.push_back(true));
        EXPECT_TRUE(bitList.at(i));
    }
    EXPECT_EQ(n, bitList.size());

    EXPECT_NO_THROW(bitList.pop_front());
    n-=1;
    EXPECT_EQ(n, bitList.size());

    EXPECT_NO_THROW(bitList.pop_back());
    n-=1;
    EXPECT_EQ(n, bitList.size());

    EXPECT_NO_THROW(bitList.erase(n/2));
    n-=1;
    EXPECT_EQ(n, bitList.size());

    EXPECT_NO_THROW(bitList.erase(n-1));
    n-=1;
    EXPECT_EQ(n, bitList.size());

    EXPECT_ANY_THROW(bitList.erase(-1));
    EXPECT_ANY_THROW(bitList.erase(n));

    bitList.clear();
    EXPECT_ANY_THROW(bitList.pop_back());
    EXPECT_ANY_THROW(bitList.pop_front());
}

TEST(CBitList, Toggle)
{
    int n = 10;
    CBitList bitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(bitList.set(i,false));
        EXPECT_NO_THROW(bitList.toggleValue(i));
        EXPECT_TRUE(bitList.at(i));
        EXPECT_NO_THROW(bitList.toggleValue(i));
        EXPECT_FALSE(bitList.at(i));
    }

    EXPECT_ANY_THROW(bitList.toggleValue(-1));
    EXPECT_ANY_THROW(bitList.toggleValue(n));
}

TEST(CBitList, ArrayIndexOperator)
{
    int n = 10;
    CBitList bitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(bitList.set(i, true));
        EXPECT_TRUE(bitList[i]);
        EXPECT_NO_THROW(bitList.set(i, false));
        EXPECT_FALSE(bitList[i]);
    }
}

TEST(CBitList, Iterators)
{
    int n = 100;
    CBitList bitList(n);
    int i = 0;
    for(IBitListIterator& it = bitList.begin(); it != bitList.end();) {
        EXPECT_NO_THROW(bitList.set(i,false));
        i++;
        EXPECT_FALSE(*it);
        if (i == 1)
            EXPECT_TRUE(it == bitList.begin());

        if ((i % 2) == 0)
            it++;
        else
            ++it;
    }

    EXPECT_EQ(n,i);
}

TEST(CBitList, Clear)
{
    int n = 100;
    CBitList bitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(bitList.set(i,true));
    }

    EXPECT_NO_THROW(bitList.clear());

    EXPECT_ANY_THROW(bitList.at(0));
    EXPECT_ANY_THROW(bitList.front());
    EXPECT_ANY_THROW(bitList.back());
}

TEST(CBitList, SaveAndLoad)
{
    int n = 100;
    CBitList bitList(n);

    for (int i = 0; i < n; ++i) {
        EXPECT_NO_THROW(bitList.set(i,true));
    }

    io::CSoDAio *io = new io::CSoDAio("sample/bitListTest.saved", io::CBinaryIO::omWrite);
    EXPECT_NO_THROW(bitList.save(io));
    delete io;

    CBitList bitList2(n/2);
    io = new io::CSoDAio("sample/bitListTest.saved", io::CBinaryIO::omRead);
    EXPECT_TRUE(io->findChunkID(io::CSoDAio::BITLIST));
    EXPECT_NO_THROW(bitList2.load(io));
    delete io;

    for (int i = 0; i < n; ++i) {
        EXPECT_TRUE(bitList2.at(i));
    }

    EXPECT_TRUE(bitList == bitList2);
    bitList2.resize(50);
    EXPECT_FALSE(bitList == bitList2);
}

TEST(CBitList, Resize_Count)
{
    IndexType n = 100;
    CBitList bitList;

    for(IndexType i = 0; i < n; ++i){
        EXPECT_NO_THROW(bitList.push_back(true));
        EXPECT_TRUE(bitList.at(i));
    }

    EXPECT_EQ(100u, bitList.count());
    EXPECT_NO_THROW(bitList.resize(104));
    EXPECT_EQ(104u, bitList.size());
    EXPECT_EQ(100u, bitList.count());
}
