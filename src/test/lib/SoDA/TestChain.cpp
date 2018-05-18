/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Author: David Imre Adam <adam.david.imre@gmail.com>
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

#include <string>
#include "gtest/gtest.h"
#include "data/CChain.h"

using namespace soda;

TEST(Chain, BasicOperations) {
    CChain s = CChain();
    vector<string> sVector = vector<string>();
    sVector.push_back("_0");
    sVector.push_back("_1");

    ASSERT_EQ(s.count(), 0);

    s.add("_0");
    s.add("_1");
    s.add("_2");

    ASSERT_TRUE(s.contains("_0"));
    ASSERT_TRUE(s.contains("_1"));
    ASSERT_TRUE(s.contains("_2"));

    ASSERT_TRUE(s.getId("_0") == 0);
    ASSERT_TRUE(s.getId("_1") == 1);
    ASSERT_TRUE(s.getId("_2") == 2);
    ASSERT_EQ(s.count(), 3);

    s.remove("_1");

    ASSERT_EQ(s.count(), 2);
    ASSERT_FALSE(s.contains("_1"));
    EXPECT_THROW(s.getId("_1"), std::out_of_range);
    ASSERT_TRUE(s.getId("_2") == 2);
    ASSERT_EQ(s.getValue(0), "_0");
    ASSERT_EQ(s.getValue(2), "_2");

    s.clear();
    s.add("A");

    ASSERT_EQ(s.count(), 1);

    s.add(sVector);
    ASSERT_EQ(s.count(), 3);

    ASSERT_EQ(s.getValue(0), "A");
    ASSERT_EQ(s.getValue(1), "_0");
    ASSERT_EQ(s.getValue(2), "_1");
}

TEST(Chain, Iterators)
{
    CChain s = CChain();
    s.add("_0");
    s.add("_1");
    s.add("_2");

    ASSERT_EQ(*(s.first()), "_0");
    ASSERT_EQ(*--s.end(), "_2");
}

TEST(Chain, BinarySaveLoad)
{
    CChain s = CChain();
    s.add("_0");
    s.add("_1");
    s.add("_2");
    s.save("chainFile");

    CChain a = CChain();
    a.load("chainFile");

    ASSERT_EQ(a.count(), 3);
    ASSERT_EQ(*(a.first()), "_0");
    ASSERT_EQ(a.getValue(0), "_0");
    ASSERT_EQ(a.getValue(1), "_1");
    ASSERT_EQ(a.getValue(2), "_2");
}

TEST(Chain, JsonLoad)
{
    CChain s = CChain();
    s.loadJson("sample/cchain.json");

    ASSERT_EQ(s.count(), 4);
    ASSERT_EQ(*(s.first()), "Element1");
    ASSERT_EQ(*(s.end()-1), "Debug");
}
