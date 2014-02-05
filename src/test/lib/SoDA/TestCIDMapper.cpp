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

#include "data/CIDMapper.h"
#include "data/CIDManager.h"
#include "exception/CException.h"
#include "gtest/gtest.h"

using namespace soda;

class CIDMapperTest : public testing::Test
{
protected:
    CIDManager* idManager;
    CIDMapper*  idMapper;

    virtual void SetUp() {
        idManager = new CIDManager();
        idMapper  = new CIDMapper(idManager);
    }

    virtual void TearDown() {
        delete idMapper;
        idMapper = 0;
        delete idManager;
        idManager = 0;
    }
};

TEST_F(CIDMapperTest, AddAndRemove)
{
    EXPECT_NO_THROW(idMapper->add(0, "First"));
    EXPECT_NO_THROW(idMapper->add(1, "Second"));
    EXPECT_NO_THROW(idMapper->add(2, "Third"));
    EXPECT_NO_THROW(idMapper->add(4, "Fourth"));
    EXPECT_EQ(4u, idMapper->size());
    EXPECT_EQ(4u, idMapper->getLastIndex());
    EXPECT_NO_THROW(idMapper->remove(2));
    EXPECT_EQ(3u, idMapper->size());
    EXPECT_EQ(4u, idMapper->getLastIndex());
    EXPECT_NO_THROW(idMapper->remove(4));
    EXPECT_EQ(2u, idMapper->size());
    EXPECT_EQ(1u, idMapper->getLastIndex());
    EXPECT_NO_THROW(idMapper->remove("Second"));
    EXPECT_EQ(1u, idMapper->size());
    EXPECT_EQ(0u, idMapper->getLastIndex());
    EXPECT_NO_THROW(idMapper->remove("this element does not exists"));
    EXPECT_NO_THROW(idMapper->remove(2));
    EXPECT_NO_THROW(idMapper->remove("Fourth"));
}

TEST_F(CIDMapperTest, GetIDAndValue)
{
    EXPECT_NO_THROW(idMapper->add(1, "First"));
    EXPECT_NO_THROW(idMapper->add(2, "Second"));
    EXPECT_NO_THROW(idMapper->add(3, "Third"));
    EXPECT_NO_THROW(idMapper->add(4, "Fourth"));

    EXPECT_EQ(1u, idMapper->getID("First"));
    EXPECT_EQ(2u, idMapper->getID("Second"));
    EXPECT_EQ(3u, idMapper->getID("Third"));
    EXPECT_EQ(4u, idMapper->getID("Fourth"));

    EXPECT_EQ("First",  idMapper->getValue(1));
    EXPECT_EQ("Second", idMapper->getValue(2));
    EXPECT_EQ("Third",  idMapper->getValue(3));
    EXPECT_EQ("Fourth", idMapper->getValue(4));
}

TEST_F(CIDMapperTest, ArrayIndexOperator)
{
    EXPECT_NO_THROW(idMapper->add("First"));
    EXPECT_NO_THROW(idMapper->add("Second"));
    EXPECT_NO_THROW(idMapper->add("Third"));
    EXPECT_NO_THROW(idMapper->add("Fourth"));

    EXPECT_EQ(0u, (*idMapper)["First"]);
    EXPECT_EQ(1u, (*idMapper)["Second"]);
    EXPECT_EQ(2u, (*idMapper)["Third"]);
    EXPECT_EQ(3u, (*idMapper)["Fourth"]);

    EXPECT_EQ("First",  (*idMapper)[0]);
    EXPECT_EQ("Second", (*idMapper)[1]);
    EXPECT_EQ("Third",  (*idMapper)[2]);
    EXPECT_EQ("Fourth", (*idMapper)[3]);
}

TEST_F(CIDMapperTest, GetIDAndValueList)
{
    unsigned int n = 10;
    char str[50];

    for (unsigned int i = 0; i < n; ++i) {
        sprintf(str, "%d th", i);
        EXPECT_NO_THROW(idMapper->add(i, str));
    }

    IntVector    idList    = idMapper->getIDList();
    StringVector valueList = idMapper->getValueList();

    EXPECT_EQ(n, idList.size());
    EXPECT_EQ(n, valueList.size());

    for (unsigned int i = 0; i < n; ++i) {
        sprintf(str, "%d th", i);
        EXPECT_EQ(str, valueList[i]);
        EXPECT_EQ(i, idList[i]);
    }
}

TEST_F(CIDMapperTest, SaveAndLoad)
{
    unsigned int n = 10;
    char str[50];

    for (unsigned int i = 0; i < n; ++i) {
        sprintf(str, "%d th", i);
        EXPECT_NO_THROW(idMapper->add(i, str));
    }

    io::CSoDAio *io = new io::CSoDAio("sample/idMapperTest.saved",  io::CBinaryIO::omWrite);
    EXPECT_NO_THROW(idMapper->save(io));
    delete io;

    CIDMapper loadedIdMapper(idManager);
    io = new io::CSoDAio("sample/idMapperTest.saved",  io::CBinaryIO::omRead);
    EXPECT_TRUE(io->findChunkID(io::CSoDAio::IDMANAGER));
    EXPECT_NO_THROW(loadedIdMapper.load(io));
    delete io;
    EXPECT_EQ(n, loadedIdMapper.size());

    for (unsigned int i = 0; i < n; ++i) {
        sprintf(str, "%d th", i);
        EXPECT_EQ(str, loadedIdMapper.getValue(i));
        EXPECT_EQ(i, loadedIdMapper.getID(str));
    }
}

TEST_F(CIDMapperTest, translateFromAnotherId)
{
    CIDMapper* anotherMapper = new CIDMapper(idManager);

    idMapper->add(0, "Zero");
    idMapper->add(1, "Alpha");
    idMapper->add(2, "Beta");
    idMapper->add(3, "Third");
    anotherMapper->add(0, "Zero");
    anotherMapper->add(1, "Beta");
    anotherMapper->add(2, "Alpha");
    anotherMapper->add(4, "Fourth");

    EXPECT_EQ(0u, idMapper->translateFromAnotherId(*anotherMapper, 0));
    EXPECT_EQ(2u, idMapper->translateFromAnotherId(*anotherMapper, 1));
    EXPECT_EQ(1u, idMapper->translateFromAnotherId(*anotherMapper, 2));
    EXPECT_THROW(idMapper->translateFromAnotherId(*anotherMapper, 4), CException);

    EXPECT_EQ(0u, anotherMapper->translateFromAnotherId(*idMapper, 0));
    EXPECT_EQ(2u, anotherMapper->translateFromAnotherId(*idMapper, 1));
    EXPECT_EQ(1u, anotherMapper->translateFromAnotherId(*idMapper, 2));
    EXPECT_THROW(anotherMapper->translateFromAnotherId(*idMapper, 3), CException);
    delete anotherMapper;

    CIDManager *differentidMgr = new CIDManager();
    anotherMapper = new CIDMapper(differentidMgr);
    EXPECT_THROW(idMapper->translateFromAnotherId(*anotherMapper, 4), CException);
    delete anotherMapper;
    delete differentidMgr;
}

TEST_F(CIDMapperTest, GetIDThrowsException)
{
    EXPECT_THROW(idMapper->getID("this element does not exists"), std::out_of_range);
    idMapper->add("test code element");
    idMapper->remove("test code element");
    EXPECT_THROW(idMapper->getID("test code element"), std::out_of_range);
}

TEST_F(CIDMapperTest, GetIDNotThrowsException)
{
    idMapper->add("codeElement1");
    EXPECT_NO_THROW(idMapper->getID("codeElement1"));
}

TEST_F(CIDMapperTest, Clear)
{
    idMapper->clear();
    EXPECT_EQ(0u, idMapper->getIDList().size());
    EXPECT_EQ(0u, idMapper->getValueList().size());
}
