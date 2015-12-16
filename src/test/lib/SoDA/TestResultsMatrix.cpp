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
#include "data/CResultsMatrix.h"
#include "data/CIDManager.h"
#include "data/CBitMatrix.h"
#include "data/CRevision.h"
#include "exception/CException.h"

using namespace soda;

class CResultsMatrixTest : public testing::Test
{
protected:
    CResultsMatrix *resMatrix;
    CRevision<IndexType> *revisions;
    CIDManager *testcases;
    CBitMatrix *exec;
    CBitMatrix *pass;


    virtual void SetUp() {
        testcases = new CIDManager();
        exec = new CBitMatrix();
        pass = new CBitMatrix();
        revisions = new CRevision<IndexType>();
        resMatrix = new CResultsMatrix(testcases, revisions, exec, pass);
        resMatrix->addOrSetResult(1, "testcase1", CResultsMatrix::trtNotExecuted);
        resMatrix->addOrSetResult(1, "testcase2", CResultsMatrix::trtPassed);
        resMatrix->addOrSetResult(1, "testcase3", CResultsMatrix::trtFailed);
        resMatrix->addOrSetResult(2, "testcase1", CResultsMatrix::trtPassed);
        resMatrix->addOrSetResult(2, "testcase2", CResultsMatrix::trtFailed);
        resMatrix->addOrSetResult(2, "testcase3", CResultsMatrix::trtNotExecuted);
    }

    virtual void TearDown() {
        delete revisions;
        revisions = 0;
        delete exec;
        exec = 0;
        delete pass;
        pass = 0;
        delete testcases;
        testcases = 0;
        delete resMatrix;
        resMatrix = 0;
    }
};

TEST_F(CResultsMatrixTest, Get)
{
    EXPECT_NO_THROW(resMatrix->getTestcases());
    EXPECT_NO_THROW(resMatrix->getRevisions());
    EXPECT_NO_THROW(resMatrix->getExecutionBitMatrix());
    EXPECT_NO_THROW(resMatrix->getPassedBitMatrix());

    EXPECT_EQ(2u, resMatrix->getNumOfRevisions());
    EXPECT_EQ(3u, resMatrix->getNumOfTestcases());

    EXPECT_TRUE(resMatrix->getExecutionBitList(1) == resMatrix->getExecutionBitMatrix()[0]);
    EXPECT_TRUE(resMatrix->getPassedBitList(2) == resMatrix->getPassedBitMatrix()[1]);
    EXPECT_TRUE(resMatrix->getRevisionNumbers() == resMatrix->getRevisions().getRevisionNumbers());
}

TEST_F(CResultsMatrixTest, BasicOperations)
{
    EXPECT_TRUE(resMatrix->isExecuted(1, "testcase2"));
    EXPECT_FALSE(resMatrix->isExecuted(1, "testcase1"));

    EXPECT_TRUE(resMatrix->isPassed(1, "testcase2"));
    EXPECT_FALSE(resMatrix->isPassed(1, "testcase1"));
    EXPECT_FALSE(resMatrix->isPassed(1, "testcase3"));

    EXPECT_EQ(CResultsMatrix::trtPassed, resMatrix->getResult(1, "testcase2"));
    EXPECT_NO_THROW(resMatrix->setResult(1, "testcase2", CResultsMatrix::trtNotExecuted));
    EXPECT_EQ(CResultsMatrix::trtNotExecuted, resMatrix->getResult(1, "testcase2"));

    EXPECT_EQ(CResultsMatrix::trtNotExecuted, resMatrix->getResult(1, 0));
    EXPECT_NO_THROW(resMatrix->setResult(1, 0, CResultsMatrix::trtPassed));
    EXPECT_EQ(CResultsMatrix::trtPassed, resMatrix->getResult(1, 0));

    EXPECT_THROW(resMatrix->setResult(3, "testcase1", CResultsMatrix::trtFailed), CException);
    EXPECT_THROW(resMatrix->setResult(1, "testcase5", CResultsMatrix::trtFailed), CException);
    EXPECT_THROW(resMatrix->setResult(3, "testcase5", CResultsMatrix::trtFailed), CException);
    EXPECT_THROW(resMatrix->setResult(3, 0, CResultsMatrix::trtFailed), CException);
    EXPECT_THROW(resMatrix->setResult(1, 5, CResultsMatrix::trtFailed), CException);
    EXPECT_THROW(resMatrix->setResult(3, 5, CResultsMatrix::trtFailed), CException);

    EXPECT_NO_THROW(resMatrix->addOrSetResult(1, "testcase4", CResultsMatrix::trtFailed));
    EXPECT_EQ(CResultsMatrix::trtFailed, resMatrix->getResult(1, "testcase4"));
    EXPECT_EQ(CResultsMatrix::trtNotExecuted, resMatrix->getResult(2, "testcase4"));

    EXPECT_NO_THROW(resMatrix->addOrSetResult(3, "testcase1", CResultsMatrix::trtPassed));
    EXPECT_EQ(CResultsMatrix::trtPassed, resMatrix->getResult(3, "testcase1"));

    EXPECT_NO_THROW(resMatrix->addOrSetResult(4, "testcase5", CResultsMatrix::trtPassed));
    EXPECT_EQ(CResultsMatrix::trtPassed, resMatrix->getResult(4, "testcase5"));
    EXPECT_EQ(CResultsMatrix::trtNotExecuted, resMatrix->getResult(4, 0));
}

TEST(CResultsMatrix, Resize)
{
    CResultsMatrix resMatrix;
    EXPECT_EQ(0u, resMatrix.getPassedBitMatrix().getNumOfCols());
    EXPECT_NO_THROW(resMatrix.addRevisionNumber(1));
    EXPECT_NO_THROW(resMatrix.addTestcaseName("testcase1"));
    EXPECT_NO_THROW(resMatrix.refitMatrixSize());
    EXPECT_EQ(1u, resMatrix.getPassedBitMatrix().getNumOfCols());
}

TEST(CResultsMatrix, SaveAndLoad)
{
    CResultsMatrix *resMatrix = new CResultsMatrix();
    EXPECT_NO_THROW(resMatrix->load(String("sample/ResultsMatrixSampleBit")));
    EXPECT_NO_THROW(resMatrix->save(String("sample/resultsMatrixTest.saved")));

    CResultsMatrix *loadedResMatrix = new CResultsMatrix();
    EXPECT_NO_THROW(loadedResMatrix->load("sample/resultsMatrixTest.saved"));
    EXPECT_NO_THROW(loadedResMatrix->save("sample/resultsMatrixTest.saved"));

    EXPECT_EQ(resMatrix->getExecutionBitMatrix(), loadedResMatrix->getExecutionBitMatrix());
    EXPECT_EQ(resMatrix->getPassedBitMatrix(), loadedResMatrix->getPassedBitMatrix());
    EXPECT_EQ(2u, loadedResMatrix->getNumOfRevisions());
    EXPECT_EQ(28u, loadedResMatrix->getNumOfTestcases());
    delete resMatrix;
    delete loadedResMatrix;
}
