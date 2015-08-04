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

extern CKernel kernel;

class CFaultLocalizationTechniquePluginsTest : public testing::Test
{
protected:

    std::map<std::string, CClusterDefinition> clusterList;
    CSelectionData selection;
    IFaultLocalizationTechniquePlugin *plugin;
    rapidjson::Document results;

    virtual void SetUp() {
        // Create selection data.
        selection.loadCoverage("sample/CoverageMatrixOneTestPerFileTestSelectionBit");
        selection.loadResults("sample/ResultsMatrixDejaGNUOneRevisionPerFileTestSelectionBit");
        selection.getResults()->setResult(12345, 0, CResultsMatrix::trtFailed);
        selection.getResults()->setResult(12345, 2, CResultsMatrix::trtFailed);
        selection.getResults()->setResult(12345, 1, CResultsMatrix::trtFailed);
        ITestSuiteClusterPlugin *clusterAlg = kernel.getTestSuiteClusterPluginManager().getPlugin("one-cluster");
        clusterAlg->execute(selection, clusterList);
        plugin = NULL;
    }

    virtual void TearDown() {
    }
};

TEST_F(CFaultLocalizationTechniquePluginsTest, OchiaiMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getFaultLocalizationTechniquePluginManager().getPlugin("ochiai"));
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 12345));

    EXPECT_EQ("ochiai", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CFaultLocalizationTechniquePluginsTest, Ochiai)
{
    EXPECT_NO_THROW(plugin = kernel.getFaultLocalizationTechniquePluginManager().getPlugin("ochiai"));
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 12345));
    EXPECT_NO_THROW(plugin->calculate(results));

    EXPECT_EQ(100u, results["full"].MemberCount());
    EXPECT_DOUBLE_EQ(0.57735026918962584, results["full"]["0"].GetDouble());
    EXPECT_DOUBLE_EQ(0.57735026918962584, results["full"]["1"].GetDouble());
    EXPECT_DOUBLE_EQ(0.40824829046386307, results["full"]["2"].GetDouble());
    EXPECT_DOUBLE_EQ(0.67171717171717171, CTestSuiteScore::flScore(clusterList["full"], results["full"]["0"].GetDouble(), plugin->getDistribution()));
}

TEST_F(CFaultLocalizationTechniquePluginsTest, TarantulaMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getFaultLocalizationTechniquePluginManager().getPlugin("tarantula"));
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 12345));

    EXPECT_EQ("tarantula", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CFaultLocalizationTechniquePluginsTest, Tarantula)
{
    EXPECT_NO_THROW(plugin = kernel.getFaultLocalizationTechniquePluginManager().getPlugin("tarantula"));
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 12345));
    EXPECT_NO_THROW(plugin->calculate(results));

    EXPECT_EQ(100u, results["full"].MemberCount());
    EXPECT_DOUBLE_EQ(1, results["full"]["0"].GetDouble());
    EXPECT_DOUBLE_EQ(1, results["full"]["1"].GetDouble());
    EXPECT_DOUBLE_EQ(0.25, results["full"]["2"].GetDouble());
    EXPECT_DOUBLE_EQ(0.78787878787878785, CTestSuiteScore::flScore(clusterList["full"], results["full"]["0"].GetDouble(), plugin->getDistribution()));
}
