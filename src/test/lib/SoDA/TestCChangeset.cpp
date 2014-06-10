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

#include "gtest/gtest.h"
#include "data/CChangeset.h"
#include "data/CIDManager.h"
#include "data/CBitList.h"
#include "exception/CException.h"

using namespace soda;

class CChangesetTest : public testing::Test
{
protected:
    CChangeset  *changeset;
    CIDManager  *codeElements;
    IntVector    intVector;
    StringVector strVector;
    unsigned int revCount, prCount;

    virtual void SetUp() {
        revCount = 10;
        prCount = 10;
        codeElements = new CIDManager();
        changeset = new CChangeset(codeElements);
        for(unsigned int i = 1; i <= revCount; ++i) {
            intVector.push_back(i);
        }
        char str[15];
        for (unsigned int i = 1; i <= prCount; ++i) {
            sprintf(str,"codeElement%d", i);
            strVector.push_back(str);
        }
    }

    virtual void TearDown() {
        delete changeset;
        changeset = 0;
        delete codeElements;
        codeElements = 0;
    }

};

TEST_F(CChangesetTest, Constructor)
{
    CIDManager *idmgr = new CIDManager(strVector);
    CRevision<IBitList*> *rev = new CRevision<IBitList*>();
    CChangeset *change = new CChangeset(idmgr, rev);
    EXPECT_EQ(prCount, change->getCodeElements().size());
    delete idmgr;
    delete rev;
    delete change;
}

TEST_F(CChangesetTest, AddRevisions)
{
    changeset->addRevision(1u);
    EXPECT_EQ(0u, changeset->getCodeElementNames(1u).size());

    changeset->addRevision(intVector);
    for(unsigned int i = 1; i <= revCount; i++) {
        EXPECT_EQ(0u, changeset->getCodeElementNames(i).size());
    }

    EXPECT_EQ(revCount, changeset->getRevisions().size());
}

TEST_F(CChangesetTest, RemoveRevisions)
{
    changeset->addRevision(intVector);
    unsigned int r = revCount/2;
    changeset->removeRevision(r);
    EXPECT_EQ(revCount-1, changeset->getRevisions().size());
    EXPECT_NE(r,changeset->getRevisions().at(r-1));
    EXPECT_EQ(r+1,changeset->getRevisions().at(r-1));

    changeset->removeRevision(intVector);
}

TEST_F(CChangesetTest, AddCodeElements)
{
    changeset->addCodeElement("codeElement1");
    EXPECT_EQ(1u, changeset->getCodeElements().size());
    changeset->addCodeElement(strVector);
    EXPECT_EQ(prCount, changeset->getCodeElements().size());
}

TEST_F(CChangesetTest, RemoveCodeElements)
{
    changeset->addRevision(intVector);
    changeset->addCodeElement(CIDManager(strVector));
    changeset->removeCodeElement("codeElement1");
    EXPECT_EQ(prCount-1, changeset->getCodeElements().size());
}

TEST_F(CChangesetTest, Toggle)
{
    changeset->addRevision(intVector);
    changeset->addCodeElement(strVector);
    for(unsigned int i = 1; i <= revCount; ++i) {
        EXPECT_EQ(0u, changeset->getCodeElementNames(i).size());
        changeset->toggle(i, "codeElement1");
        EXPECT_EQ(1u, changeset->getCodeElementNames(i).size());
    }
    EXPECT_EQ(revCount, changeset->getRevisions("codeElement1").size());
}

TEST_F(CChangesetTest, Exists)
{
    changeset->addCodeElement(strVector);
    changeset->addRevision(intVector);
    EXPECT_TRUE(changeset->exists(1));
    EXPECT_FALSE(changeset->exists(20));
}

TEST_F(CChangesetTest, SaveAndLoad)
{
    changeset->addRevision(intVector);
    changeset->addCodeElement(strVector);

    EXPECT_NO_THROW(changeset->save("sample/changesetTest.saved"));

    EXPECT_ANY_THROW(changeset->load("sample/idManagerTest.saved"));
    EXPECT_ANY_THROW(changeset->load("sample/ResultsMatrixSampleBit"));

    EXPECT_NO_THROW(changeset->load("sample/changesetTest.saved"));
    EXPECT_NO_THROW(changeset->save(String("sample/changesetTest.saved")));
    EXPECT_NO_THROW(changeset->load(String("sample/changesetTest.saved")));

    const IIDManager *loadedCodeElements = &(changeset->getCodeElements());
    unsigned int prCount = codeElements->size();

    EXPECT_EQ(prCount, loadedCodeElements->getIDList().size());
    for(unsigned int i = 0; i < prCount; ++i) {
        strcmp((*codeElements)[i].c_str(), (*loadedCodeElements)[i].c_str());
    }
}

TEST_F(CChangesetTest, BackwardCompatibility)
{
    EXPECT_NO_THROW(changeset->load("sample/ChangesetSampleBit"));
    EXPECT_EQ(30u, (changeset->getCodeElements()).size());
    EXPECT_EQ(6u, (changeset->getRevisions()).size());
}

TEST_F(CChangesetTest, AddOrSetChange)
{
    EXPECT_NO_THROW(changeset->addOrSetChange(1, "codeElement1", true));
    EXPECT_EQ(1u, changeset->getCodeElements().size());
    EXPECT_EQ(1u, changeset->getRevisions().size());
    EXPECT_TRUE(changeset->isChanged(1, "codeElement1"));
}

TEST_F(CChangesetTest, SetChange)
{
    changeset->addRevision(1);
    changeset->addCodeElement("codeElement1");
    changeset->setChange(1, "codeElement1",true);
    EXPECT_ANY_THROW(changeset->setChange(2, "ezer", true));
    EXPECT_TRUE(changeset->isChanged(1, "codeElement1"));
}

TEST_F(CChangesetTest, IndexOperator)
{
    changeset->addRevision(intVector);
    changeset->addCodeElement(strVector);
    IBitList& bitList = (*changeset)[1];
    EXPECT_EQ(prCount, bitList.size());
    bitList = changeset->at(2);
    EXPECT_EQ(prCount, bitList.size());
}
