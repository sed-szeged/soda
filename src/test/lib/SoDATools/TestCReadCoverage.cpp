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
#include "datareader/CReadCoverage.h"

using namespace soda;
using namespace sodatools;

class CReadCoverageTest : public testing::Test
{
protected:
    virtual void SetUp() {
        coverageMatrix = new CCoverageMatrix();
    }

    virtual void TearDown() {
        delete coverageMatrix;
        coverageMatrix=0;
    }

    CCoverageMatrix* coverageMatrix;
};

TEST_F(CReadCoverageTest, readOneTestPerFileCoverageFromDirectoryStructureTest)
{
    EXPECT_NO_THROW(
        CReadCoverage coverageReader(*coverageMatrix, false, true, true, false);
        coverageReader.readOneTestPerFileCoverageFromDirectoryStructure("sample/CoverageMatrixOneTestPerFileSampleDir");
        EXPECT_EQ(3u, coverageMatrix->getNumOfTestcases());
        EXPECT_EQ(824u, coverageMatrix->getNumOfCodeElements());
    );
}

TEST_F(CReadCoverageTest, readOneTestPerFileCoverageFromDirectoryStructureWithPassFailInfoTest)
{
    EXPECT_NO_THROW(
        CReadCoverage coverageReader(*coverageMatrix, true, true, true, false);
        coverageReader.readOneTestPerFileCoverageFromDirectoryStructure("sample/CoverageMatrixOneTestPerFilePassFailSampleDir");
        EXPECT_EQ(3u, coverageMatrix->getNumOfTestcases());
        EXPECT_EQ(824u, coverageMatrix->getNumOfCodeElements());
    );
}
