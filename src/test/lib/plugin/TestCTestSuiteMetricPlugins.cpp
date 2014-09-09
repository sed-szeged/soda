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
#include "data/CSelectionData.h"
#include "data/CClusterDefinition.h"
#include "engine/CKernel.h"
#include "engine/plugin/ITestSuiteMetricPlugin.h"
#include "engine/plugin/ITestSuiteClusterPlugin.h"

using namespace soda;

class CTestSuiteMetricPluginsTest : public testing::Test
{
protected:
    std::map<std::string, CClusterDefinition> clusterList;
    rapidjson::Document results;
    CSelectionData selection;
    ITestSuiteMetricPlugin *plugin;
    ITestSuiteClusterPlugin *clusterAlg;
    CKernel kernel;

    virtual void SetUp() {
        // Create selection data.
        selection.loadCoverage("sample/CoverageMatrixOneTestPerFileTestSelectionBit");
        selection.loadResults("sample/ResultsMatrixDejaGNUOneRevisionPerFileTestSelectionBit");
        selection.getCoverage()->setRelation(0, 0, false);
        clusterAlg = kernel.getTestSuiteClusterPluginManager().getPlugin("one-cluster");
        clusterAlg->execute(selection, clusterList);
        plugin = NULL;
    }

    virtual void TearDown() {
    }
};

TEST_F(CTestSuiteMetricPluginsTest, FaultDetection)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("fault-detection"));
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 12345));
    EXPECT_NO_THROW(plugin->calculate(results));

    EXPECT_DOUBLE_EQ(0.99, results["full"]["fault-detection"].GetDouble());
}

TEST_F(CTestSuiteMetricPluginsTest, FaultLocalization)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("fault-localization"));
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 12345));
    EXPECT_NO_THROW(plugin->calculate(results));

    EXPECT_DOUBLE_EQ(0.15252525252525254, results["full"]["fault-localization"].GetDouble());
}

TEST_F(CTestSuiteMetricPluginsTest, FMeasure)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("f-measure"));
    StringVector dependencies = plugin->getDependency();
    for (StringVector::iterator it = dependencies.begin(); it != dependencies.end(); it++) {
        ITestSuiteMetricPlugin *dep = kernel.getTestSuiteMetricPluginManager().getPlugin(*it);
        dep->init(&selection, &clusterList, 12345);
        dep->calculate(results);
    }
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 12345));
    EXPECT_NO_THROW(plugin->calculate(results));

    EXPECT_DOUBLE_EQ(0.26432676155954388, results["full"]["f-measure"].GetDouble());
}
