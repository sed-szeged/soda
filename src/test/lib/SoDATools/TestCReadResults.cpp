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
#include "datareader/CReadResults.h"

using namespace soda;
using namespace sodatools;

class CReadResultsTest : public testing::Test
{
protected:
    CReadResults*   resultsReader;
    CResultsMatrix* resultsMatrix;

    virtual void SetUp() {
        resultsMatrix = new CResultsMatrix();
        resultsReader = new CReadResults(resultsMatrix, true, true, false);
    }

    virtual void TearDown() {
        delete resultsMatrix;
        resultsMatrix = 0;
        delete resultsReader;
        resultsReader = 0;
    }
};

TEST_F(CReadResultsTest, readOneTestPerFileResultsFromDirectoryStructureTest)
{
    EXPECT_NO_THROW(
        resultsReader->readOneTestPerFileResultsFromDirectoryStructure("sample/ResultsMatrixOneTestPerFileSampleDir");
        EXPECT_EQ(2u, resultsMatrix->getNumOfRevisions());
        EXPECT_EQ(28u, resultsMatrix->getNumOfTestcases());
    );
}
