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

#include "data/CIDManager.h"
#include "gtest/gtest.h"

using namespace soda;

class CIDManagerTest : public testing::Test
{
protected:
    CIDManager* idManager;

    virtual void SetUp() {
        idManager = new CIDManager();
    }

    virtual void TearDown() {
        delete idManager;
        idManager = 0;
    }
};

TEST(CIDManager, Constructor)
{
    EXPECT_NO_THROW(
        CIDManager c;

        StringVector v(4, "something");
        CIDManager c1(v);
        EXPECT_EQ(4u, c1.size());

        StringMap strmap;
        strmap.insert(std::pair<IndexType, String>(0, "Zero"));
        strmap.insert(std::pair<IndexType, String>(1, "One"));
        CIDManager c2(strmap);
        EXPECT_EQ(2u, c2.size());
        EXPECT_EQ("Zero", c2[0]);
        EXPECT_EQ("One",  c2[1]);

        IdxStrMap idxmap;
        idxmap.insert(std::pair<String, IndexType>("Zero", 0));
        idxmap.insert(std::pair<String, IndexType>("One", 1));
        CIDManager c3(idxmap);
        EXPECT_EQ(2u, c3.size());
        EXPECT_EQ("Zero", c3[0]);
        EXPECT_EQ("One", c3[1]);
    );
}

TEST_F(CIDManagerTest, AddAndRemove)
{
    EXPECT_NO_THROW(idManager->add(0, "First"));
    EXPECT_NO_THROW(idManager->add(1, "Second"));
    EXPECT_NO_THROW(idManager->add(2, "Third"));
    EXPECT_NO_THROW(idManager->add(4, "Fourth"));
    EXPECT_EQ(4u, idManager->size());
    EXPECT_EQ(4u, idManager->getLastIndex());
    EXPECT_NO_THROW(idManager->remove(2));
    EXPECT_EQ(3u, idManager->getIDList().size());
    EXPECT_EQ(4u, idManager->getLastIndex());
    EXPECT_NO_THROW(idManager->remove(4));
    EXPECT_EQ(2u, idManager->getIDList().size());
    EXPECT_EQ(1u, idManager->getLastIndex());
}

TEST_F(CIDManagerTest, GetIDAndValue)
{
    EXPECT_NO_THROW(idManager->add(1, "First"));
    EXPECT_NO_THROW(idManager->add(2, "Second"));
    EXPECT_NO_THROW(idManager->add(3, "Third"));
    EXPECT_NO_THROW(idManager->add(4, "Fourth"));

    EXPECT_EQ(1u, idManager->getID("First"));
    EXPECT_EQ(2u, idManager->getID("Second"));
    EXPECT_EQ(3u, idManager->getID("Third"));
    EXPECT_EQ(4u, idManager->getID("Fourth"));

    EXPECT_EQ("First",  idManager->getValue(1));
    EXPECT_EQ("Second", idManager->getValue(2));
    EXPECT_EQ("Third",  idManager->getValue(3));
    EXPECT_EQ("Fourth", idManager->getValue(4));
}

TEST_F(CIDManagerTest, ArrayIndexOperator)
{
    EXPECT_NO_THROW(idManager->add("First"));
    EXPECT_NO_THROW(idManager->add("Second"));
    EXPECT_NO_THROW(idManager->add("Third"));
    EXPECT_NO_THROW(idManager->add("Fourth"));

    EXPECT_EQ(0u, (*idManager)["First"]);
    EXPECT_EQ(1u, (*idManager)["Second"]);
    EXPECT_EQ(2u, (*idManager)["Third"]);
    EXPECT_EQ(3u, (*idManager)["Fourth"]);

    EXPECT_EQ("First",  (*idManager)[0]);
    EXPECT_EQ("Second", (*idManager)[1]);
    EXPECT_EQ("Third",  (*idManager)[2]);
    EXPECT_EQ("Fourth", (*idManager)[3]);
}

TEST_F(CIDManagerTest, GetIDAndValueList)
{
    unsigned int n = 10;
    char str[50];

    for (unsigned int i = 0; i < n; ++i) {
        sprintf(str, "%d th", i);
        EXPECT_NO_THROW(idManager->add(i, str));
    }

    IntVector    idList    = idManager->getIDList();
    StringVector valueList = idManager->getValueList();

    EXPECT_EQ(n, idList.size());
    EXPECT_EQ(n, valueList.size());

    for (unsigned int i = 0; i < n; ++i) {
        sprintf(str, "%d th", i);
        EXPECT_EQ(str, valueList[i]);
        EXPECT_EQ(i, idList[i]);
    }
}

TEST_F(CIDManagerTest, SaveAndLoad)
{
    unsigned int n = 10;
    char str[50];

    for (unsigned int i = 0; i < n; ++i) {
        sprintf(str, "%d th", i);
        EXPECT_NO_THROW(idManager->add(i, str));
    }

    io::CSoDAio *io = new io::CSoDAio("sample/idManagerTest.saved",  io::CBinaryIO::omWrite);
    EXPECT_NO_THROW(idManager->save(io));
    delete io;

    CIDManager loadedIdManager;
    io = new io::CSoDAio("sample/idManagerTest.saved",  io::CBinaryIO::omRead);
    EXPECT_TRUE(io->findChunkID(io::CSoDAio::IDMANAGER));
    EXPECT_NO_THROW(loadedIdManager.load(io));
    delete io;
    EXPECT_EQ(n, loadedIdManager.size());

    for (unsigned int i = 0; i < n; ++i) {
        sprintf(str, "%d th", i);
        EXPECT_EQ(str, loadedIdManager.getValue(i));
        EXPECT_EQ(i, loadedIdManager.getID(str));
    }
}

TEST_F(CIDManagerTest, GetIDThrowsException)
{
    EXPECT_THROW(idManager->getID("this element does not exists"), std::out_of_range);
}

TEST_F(CIDManagerTest, GetIDNotThrowsException)
{
    idManager->add("codeElement1");
    EXPECT_NO_THROW(idManager->getID("codeElement1"));
}

TEST_F(CIDManagerTest, GetLastIndexThrowsException)
{
    EXPECT_THROW(idManager->getLastIndex(), std::length_error);
}

TEST_F(CIDManagerTest, ContainsValue)
{
    idManager->add("codeElement1");
    EXPECT_TRUE(idManager->containsValue("codeElement1"));
    EXPECT_FALSE(idManager->containsValue("unknown code element"));
}

TEST_F(CIDManagerTest, Clear)
{
    idManager->clear();
    EXPECT_EQ(0u, idManager->getIDList().size());
    EXPECT_EQ(0u, idManager->getValueList().size());
}
