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

class MutationMetricPluginsTest : public testing::Test
{
protected:
    rapidjson::Document results;
    CSelectionData selection;
    IMutationMetricPlugin *plugin;
    CKernel kernel;

    virtual void SetUp() {
        results.AddMember("mutation-coverage", "sample/MutationMetricPluginSample/oryx-mutation-point.cov.SoDA", results.GetAllocator());
        results.AddMember("mutation-map", "sample/MutationMetricPluginSample/oryx-mutants-if.list.csv", results.GetAllocator());
        // Create selection data.
        selection.loadCoverage("sample/MutationMetricPluginSample/oryx-mutation-method.cov.SoDA");
        selection.loadResults("sample/MutationMetricPluginSample/oryx-mutation-if.results.SoDA");
        plugin = nullptr;
    }

    virtual void TearDown() {
    }
};

TEST_F(MutationMetricPluginsTest, ResultsScoreMetaInfo) {
    EXPECT_NO_THROW(plugin = kernel.getMutationMetricPluginManager().getPlugin("results-score"));
    EXPECT_EQ("results-score", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(MutationMetricPluginsTest, ResultsScore) {
    EXPECT_NO_THROW(plugin = kernel.getMutationMetricPluginManager().getPlugin("results-score"));
    EXPECT_NO_THROW(plugin->init(&selection, results, nullptr));
    EXPECT_NO_THROW(plugin->calculate(results));
    EXPECT_DOUBLE_EQ(0.21568627450980393, results["mutation-metrics"]["fail-results-score"].GetDouble());
    EXPECT_DOUBLE_EQ(0, results["mutation-metrics"]["pass-results-score"].GetDouble());
}

TEST_F(MutationMetricPluginsTest, MutationCategoryMetaInfo) {
    EXPECT_NO_THROW(plugin = kernel.getMutationMetricPluginManager().getPlugin("mutation-category"));
    EXPECT_EQ("mutation-category", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(MutationMetricPluginsTest, MutationCategory) {
    EXPECT_NO_THROW(plugin = kernel.getMutationMetricPluginManager().getPlugin("mutation-category"));
    EXPECT_NO_THROW(plugin->init(&selection, results, nullptr));
    EXPECT_NO_THROW(plugin->calculate(results));
    EXPECT_EQ(51, results["mutation-metrics"]["nrOfMutations"].GetInt());
    EXPECT_EQ(38, results["mutation-metrics"]["type-1"].GetInt());
    EXPECT_EQ(1, results["mutation-metrics"]["type-2"].GetInt());
    EXPECT_EQ(1, results["mutation-metrics"]["type-3"].GetInt());
    EXPECT_EQ(0, results["mutation-metrics"]["type-4A"].GetInt());
    EXPECT_EQ(11, results["mutation-metrics"]["type-4B"].GetInt());
}
