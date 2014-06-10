/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Havas <havasd@inf.u-szeged.hu>
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

#include "gtest/gtest.h"
#include "io/CJsonReader.h"

using namespace soda;
using namespace soda::io;

class CJsonReaderTest : public testing::Test
{
protected:
    CJsonReader *reader;

    virtual void SetUp() {
        reader = new CJsonReader("sample/test.json");
    }

    virtual void TearDown() {
        delete reader;
        reader = 0;
    }
};

TEST_F(CJsonReaderTest, Clear)
{
    EXPECT_TRUE(reader->existsProperty("string"));
    EXPECT_NO_THROW(reader->clear());
    EXPECT_FALSE(reader->existsProperty("string"));
}

TEST_F(CJsonReaderTest, GetOperations)
{
    EXPECT_EQ("testString", reader->getStringFromProperty("string"));
    EXPECT_EQ(0u, reader->getBoolFromProperty("boolean"));
    EXPECT_EQ(132u, reader->getIntFromProperty("integer"));

    IntVector ints = reader->getIntVectorFromProperty("integers");
    EXPECT_EQ(4u, ints.size());
    EXPECT_EQ(342, ints[2]);

    StringVector strings = reader->getStringVectorFromProperty("strings");
    EXPECT_EQ(2u, strings.size());
    EXPECT_EQ("string2", strings[1]);

    std::vector<CJsonReader> properties = reader->getPropertyVectorFromProperty("persons");
    EXPECT_EQ(2u, properties.size());
    EXPECT_EQ("John Doe", properties[0].getStringFromProperty("name"));
    EXPECT_EQ(32u, properties[0].getIntFromProperty("age"));
}

TEST_F(CJsonReaderTest, Open)
{
    delete reader;
    EXPECT_NO_THROW(reader = new CJsonReader());
    EXPECT_FALSE(reader->existsProperty("string"));
    EXPECT_NO_THROW(reader->load("sample/test.json"));
    EXPECT_TRUE(reader->existsProperty("string"));
}
