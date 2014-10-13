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
#include "util/CTestSuiteScore.h"
#include "data/CClusterDefinition.h"
#include "data/CSelectionData.h"
#include "engine/plugin/IFaultLocalizationTechniquePlugin.h"
#include "engine/plugin/ITestSuiteClusterPlugin.h"
#include "engine/CKernel.h"

using namespace soda;

class CFaultLocalizationTechniquePluginsTest : public testing::Test
{
protected:

    std::map<std::string, CClusterDefinition> clusterList;
    CSelectionData selection;
    IFaultLocalizationTechniquePlugin *plugin;
    ITestSuiteClusterPlugin *clusterAlg;
    CKernel kernel;

    virtual void SetUp() {
        // Create selection data.
        selection.loadCoverage("sample/CoverageMatrixOneTestPerFileTestSelectionBit");
        selection.loadResults("sample/ResultsMatrixDejaGNUOneRevisionPerFileTestSelectionBit");
        selection.getResults()->setResult(12345, 0, CResultsMatrix::trtFailed);
        selection.getResults()->setResult(12345, 2, CResultsMatrix::trtFailed);
        selection.getResults()->setResult(12345, 1, CResultsMatrix::trtFailed);
        clusterAlg = kernel.getTestSuiteClusterPluginManager().getPlugin("one-cluster");
        clusterAlg->execute(selection, clusterList);
        plugin = NULL;
    }

    virtual void TearDown() {
    }
};

TEST_F(CFaultLocalizationTechniquePluginsTest, Ochiai)
{
    EXPECT_NO_THROW(plugin = kernel.getFaultLocalizationTechniquePluginManager().getPlugin("ochiai"));
    EXPECT_NO_THROW(plugin->init(&selection, 12345));
    EXPECT_NO_THROW(plugin->calculate(clusterList["full"], "sample"));

    EXPECT_EQ(100u, plugin->getValues().MemberCount());
    EXPECT_DOUBLE_EQ(0.57735026918962584, plugin->getValues()["0"].GetDouble());
    EXPECT_DOUBLE_EQ(0.57735026918962584, plugin->getValues()["1"].GetDouble());
    EXPECT_DOUBLE_EQ(0.40824829046386307, plugin->getValues()["2"].GetDouble());
    EXPECT_DOUBLE_EQ(0.67171717171717171, CTestSuiteScore::flScore(clusterList["full"], plugin->getValues()["0"].GetDouble(), plugin->getDistribution()));
}

TEST_F(CFaultLocalizationTechniquePluginsTest, Tarantula)
{
    EXPECT_NO_THROW(plugin = kernel.getFaultLocalizationTechniquePluginManager().getPlugin("tarantula"));
    EXPECT_NO_THROW(plugin->init(&selection, 12345));
    EXPECT_NO_THROW(plugin->calculate(clusterList["full"], "sample"));

    EXPECT_EQ(100u, plugin->getValues().MemberCount());
    EXPECT_DOUBLE_EQ(1, plugin->getValues()["0"].GetDouble());
    EXPECT_DOUBLE_EQ(1, plugin->getValues()["1"].GetDouble());
    EXPECT_DOUBLE_EQ(0.25, plugin->getValues()["2"].GetDouble());
    EXPECT_DOUBLE_EQ(0.78787878787878785, CTestSuiteScore::flScore(clusterList["full"], plugin->getValues()["0"].GetDouble(), plugin->getDistribution()));
}
