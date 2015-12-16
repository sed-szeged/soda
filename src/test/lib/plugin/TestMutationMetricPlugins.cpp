/*
* Copyright (C): 2015 Department of Software Engineering, University of Szeged
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

using namespace soda;

class TestSuiteMetricPluginsTest : public testing::Test
{
protected:
    rapidjson::Document results;
    CSelectionData selection;
    IMutationMetricPlugin *plugin;
    CKernel kernel;

    virtual void SetUp() {
        // Create selection data.
        selection.loadResults("sample/MetricPluginSampleDir/oryx.50.returns.res.SoDA");
        plugin = nullptr;
    }

    virtual void TearDown() {
    }
};

TEST_F(TestSuiteMetricPluginsTest, ResultsScoreMetaInfo) {
    EXPECT_NO_THROW(plugin = kernel.getMutationMetricPluginManager().getPlugin("results-score"));
    EXPECT_EQ("results-score", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(TestSuiteMetricPluginsTest, ResultsScore) {
    EXPECT_NO_THROW(plugin = kernel.getMutationMetricPluginManager().getPlugin("results-score"));
    EXPECT_NO_THROW(plugin->init(&selection, results, nullptr));
    EXPECT_NO_THROW(plugin->calculate(results));
    EXPECT_DOUBLE_EQ(0.83999999999999997, results["mutation-metrics"]["fail-results-score"].GetDouble());
    EXPECT_DOUBLE_EQ(0, results["mutation-metrics"]["pass-results-score"].GetDouble());
}

TEST_F(TestSuiteMetricPluginsTest, MutationCategoryMetaInfO) {
    EXPECT_NO_THROW(plugin = kernel.getMutationMetricPluginManager().getPlugin("mutation-category"));
    EXPECT_EQ("mutation-category", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

/*TEST_F(TestSuiteMetricPluginsTest, MutationCategory) {
    EXPECT_NO_THROW(plugin = kernel.getMutationMetricPluginManager().getPlugin("results-score"));
    EXPECT_NO_THROW(plugin->init(&selection, results, nullptr));
    EXPECT_NO_THROW(plugin->calculate(results));
    EXPECT_DOUBLE_EQ(0.83999999999999997, results["mutation-metrics"]["fail-results-score"].GetDouble());
    EXPECT_DOUBLE_EQ(0, results["mutation-metrics"]["pass-results-score"].GetDouble());
}*/
