/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: Tamás Gergely <gertom@inf.u-szeged.hu>
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
#include "data/CSelectionData.h"
#include "exception/CException.h"

using namespace soda;

class CSelectionDataTest : public testing::Test
{
protected:
    CSelectionData* data;

    virtual void SetUp() {
        data = new CSelectionData();
    }

    virtual void TearDown() {
        delete data;
        data = 0;
    }
};

TEST_F(CSelectionDataTest, loadSelection)
{
    EXPECT_NO_THROW(data->loadChangeset("sample/ChangesetSampleBit"));
    EXPECT_NO_THROW(data->loadCoverage("sample/CoverageMatrixSampleBit"));
    EXPECT_NO_THROW(data->loadResults("sample/ResultsMatrixSampleBit"));

    EXPECT_EQ(30u,  data->getChangeset()->getCodeElements().size());
    EXPECT_EQ(6u,   data->getChangeset()->getRevisions().size());
    EXPECT_EQ(824u, data->getCoverage()->getCodeElements().size());
    EXPECT_EQ(3u,   data->getCoverage()->getTestcases().size());
    EXPECT_EQ(2u,   data->getResults()->getNumOfRevisions());
    EXPECT_EQ(28u,  data->getResults()->getNumOfTestcases());

    EXPECT_EQ(850u, data->getCodeElements()->size());
    EXPECT_EQ(31u,  data->getTestcases()->size());
}

TEST_F(CSelectionDataTest, translations)
{
    EXPECT_NO_THROW(data->loadChangeset("sample/SelectionDataChangeset"));
    EXPECT_NO_THROW(data->loadCoverage("sample/SelectionDataCoverage"));
    EXPECT_NO_THROW(data->loadResults("sample/SelectionDataResults"));

    data->globalize();

    for(IndexType i=0; i<data->getChangeset()->getCodeElements().size(); i++) {
        EXPECT_EQ(i, data->translateCodeElementIdFromCoverageToChangeset(
                        data->translateCodeElementIdFromChangesetToCoverage(i)));
    }
    for(IndexType i=0; i<data->getCoverage()->getCodeElements().size(); i++) {
        EXPECT_EQ(i, data->translateCodeElementIdFromChangesetToCoverage(
                        data->translateCodeElementIdFromCoverageToChangeset(i)));
    }
    for(IndexType i=0; i<data->getCoverage()->getTestcases().size(); i++) {
        EXPECT_EQ(i, data->translateTestcaseIdFromResultsToCoverage(
                        data->translateTestcaseIdFromCoverageToResults(i)));
    }
    for(IndexType i=0; i<data->getResults()->getTestcases().size(); i++) {
        EXPECT_EQ(i, data->translateTestcaseIdFromCoverageToResults (
                        data->translateTestcaseIdFromResultsToCoverage(i)));
    }
}
