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
    CBitMatrix*  vectorBitMatrix;
    CIDManager* idManager;

    virtual void SetUp() {
         idManager = new CIDManager();
         idManager->add(0, "Eye");
         idManager->add(2, "Eyes");
         vectorBitMatrix  = new CBitMatrix(4, 4);
         RMatrix = new CRelationMatrix(vectorBitMatrix, idManager);
    }

    virtual void TearDown() {
        delete RMatrix;
        RMatrix = 0;
        delete idManager;
        idManager = 0;
        delete vectorBitMatrix;
        vectorBitMatrix = 0;
    }
};

TEST_F(CRelationMatrixTest, SetAndGet)
{
    EXPECT_NO_THROW(RMatrix->setRelation(0, 2, true));
    EXPECT_NO_THROW(RMatrix->setRelation(1, 0, true));
    EXPECT_NO_THROW(RMatrix->setRelation(2, 3, true));
    EXPECT_NO_THROW(RMatrix->setRelation(3, 3, true));
    EXPECT_NO_THROW(RMatrix->getIdManager());
    EXPECT_NO_THROW(RMatrix->getRelationBitMatrix());
    EXPECT_EQ(4u, RMatrix->getSize());
    EXPECT_EQ(true, RMatrix->getRelation(0, 2));
    EXPECT_EQ(true, RMatrix->getRelation("Eye", "Eyes"));
    EXPECT_NO_THROW(RMatrix->getIdManager());
    EXPECT_NO_THROW(RMatrix->getRelationBitMatrix());
}
