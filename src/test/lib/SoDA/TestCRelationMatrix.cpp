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

#include "data/CRelationMatrix.h"
#include "data/CBitMatrix.h"
#include "exception/CException.h"
#include "gtest/gtest.h"
#include "io/CBinaryIO.h"
#include "data/CIDManager.h"


using namespace soda;

class CRelationMatrixTest : public testing::Test
{
protected:
    CRelationMatrix* RMatrix;
    CBitMatrix*  bitMatrix;
    CIDManager* idManager;

    virtual void SetUp() {
         idManager = new CIDManager();
         idManager->add(0, "Eye");
         idManager->add(1, "codeElement1");
         idManager->add(2, "Eyes");
         idManager->add(3, "codeElement2");
         bitMatrix  = new CBitMatrix(4, 4);
         RMatrix = new CRelationMatrix(bitMatrix, idManager);
    }

    virtual void TearDown() {
        delete RMatrix;
        RMatrix = 0;
        delete idManager;
        idManager = 0;
        delete bitMatrix;
        bitMatrix = 0;
    }
};

TEST(CRelationMatrix, DefaultContructor)
{
    CRelationMatrix relMatrix;
    EXPECT_TRUE(relMatrix.getIdManager().size() == 0);
    EXPECT_TRUE(relMatrix.getSize() == 0);

    CRelationMatrix *relMatrix2 = new CRelationMatrix();
    EXPECT_TRUE(relMatrix2->getIdManager().size() == 0);
    EXPECT_TRUE(relMatrix2->getSize() == 0);
    delete relMatrix2;
    relMatrix2 = 0;
}

TEST_F(CRelationMatrixTest, SetAndGet)
{
    EXPECT_NO_THROW(RMatrix->getIdManager());
    EXPECT_NO_THROW(RMatrix->getRelationBitMatrix());
    EXPECT_EQ(4u, RMatrix->getSize());

    EXPECT_NO_THROW(RMatrix->setRelation(0, 2, true));
    EXPECT_NO_THROW(RMatrix->setRelation(1, 0, true));
    EXPECT_NO_THROW(RMatrix->setRelation(2, 3, true));
    EXPECT_NO_THROW(RMatrix->setRelation(2, 2, true));
    EXPECT_NO_THROW(RMatrix->setRelation(3, 3, true));
    EXPECT_EQ(true, RMatrix->getRelation(0, 2));
    EXPECT_EQ(true, RMatrix->getRelation("Eye", "Eyes"));

    EXPECT_EQ(1u, (*RMatrix)[0].count());
    EXPECT_EQ(1u, (*RMatrix)["codeElement1"].count());
    EXPECT_EQ(2u, (*RMatrix)[2].count());
    EXPECT_EQ(1u, (*RMatrix)["codeElement2"].count());

}

TEST_F(CRelationMatrixTest, ImpactSet)
{
    EXPECT_NO_THROW(RMatrix->setRelation(0, 1, true));
    EXPECT_NO_THROW(RMatrix->setRelation(0, 2, true));

    EXPECT_EQ(2u, RMatrix->getImpactSet("Eye").size());
    EXPECT_EQ(1u, RMatrix->getImpactSet("Eye").count("codeElement1"));
    EXPECT_EQ(1u, RMatrix->getImpactSet("Eye").count("Eyes"));

    std::vector<IndexType> impactSetSizes = RMatrix->computeImpactSetSizes();
    EXPECT_EQ(RMatrix->getSize() + 1, impactSetSizes.size());
    EXPECT_EQ(1u, impactSetSizes[2]);
    EXPECT_EQ(3u, impactSetSizes[4]);

    std::vector<int> detMsg = RMatrix->computeDetailedMsg();
    EXPECT_EQ(5u, detMsg.size());
    EXPECT_EQ(1u, detMsg[2]);
    EXPECT_EQ(3u, detMsg[4]);
}

TEST_F(CRelationMatrixTest, SaveAndLoad)
{
    EXPECT_NO_THROW(RMatrix->setRelation(0, 2, true));
    EXPECT_NO_THROW(RMatrix->setRelation(1, 0, true));
    EXPECT_NO_THROW(RMatrix->setRelation(2, 3, true));
    EXPECT_NO_THROW(RMatrix->setRelation(2, 2, true));
    EXPECT_NO_THROW(RMatrix->setRelation(3, 3, true));

    CRelationMatrix otherRelMatrix;
    EXPECT_NO_THROW(RMatrix->save("sample/relationMatrixTest.saved"));
    EXPECT_THROW(otherRelMatrix.load("sample/ioTest.saved"), CException);
    EXPECT_NO_THROW(otherRelMatrix.load("sample/relationMatrixTest.saved"));
    EXPECT_NO_THROW(RMatrix->save(String("sample/relationMatrixTest.saved")));
    EXPECT_NO_THROW(otherRelMatrix.load(String("sample/relationMatrixTest.saved")));

    EXPECT_TRUE(RMatrix->getRelationBitMatrix() == otherRelMatrix.getRelationBitMatrix());
    EXPECT_TRUE(RMatrix->getIdManager().size() == otherRelMatrix.getIdManager().size());
}
