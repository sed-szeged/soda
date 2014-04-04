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
#include "data/CClusterDefinition.h"
#include "data/CSelectionData.h"
#include "engine/plugin/ITestSuiteClusterPlugin.h"
#include "engine/CKernel.h"

using namespace soda;

class CTestSuiteClusterPluginsTest : public testing::Test
{
protected:
    CSelectionData data;
    ITestSuiteClusterPlugin *plugin;
    std::vector<CClusterDefinition> clusterList;
    CKernel kernel;

    virtual void SetUp() {
        plugin = NULL;
        clusterList.clear();
        data.loadCoverage("sample/CoverageMatrixOneTestPerFileTestSelectionBit");
    }

    virtual void TearDown() {
    }
};

TEST_F(CTestSuiteClusterPluginsTest, TestSuiteOneClusterPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("one-cluster"));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(1u, clusterList.size());
    EXPECT_EQ(data.getCoverage()->getNumOfCodeElements(), clusterList[0].getCodeElements().size());
    EXPECT_EQ(data.getCoverage()->getNumOfTestcases(), clusterList[0].getTestCases().size());
}
