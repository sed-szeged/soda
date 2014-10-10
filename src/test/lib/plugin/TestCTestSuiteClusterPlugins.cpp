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
#include "rapidjson/document.h"
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
    ClusterMap clusterList;
    CKernel kernel;
    rapidjson::Document doc;

    virtual void SetUp() {
        plugin = NULL;
        clusterList.clear();
        std::srand(std::time(0));
        for (int i = 0; i < 100; ++i) {
            for (int j = 0; j < 100; ++j) {
                data.getCoverage()->addOrSetRelation("test-" + std::to_string(i), "ce-" + std::to_string(j), (j <= i) ? true : false);
            }
        }
        doc.SetObject();
        rapidjson::Value val(rapidjson::kArrayType);
        for (int i = 10; i <= 50; i += 10)
            val.PushBack(i, doc.GetAllocator());
        doc.AddMember("cluster-sizes", val, doc.GetAllocator());
        doc.AddMember("cluster-test-list", "sample/ClusterPluginSampleDir/label-test-codeelement-test.txt", doc.GetAllocator());
        doc.AddMember("cluster-code-elements-list", "sample/ClusterPluginSampleDir/label-test-codeelement-method.txt", doc.GetAllocator());
    }

    virtual void TearDown() {
    }
};

TEST_F(CTestSuiteClusterPluginsTest, TestSuiteOneClusterPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("one-cluster"));
    EXPECT_NO_THROW(plugin->init(doc));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(1u, clusterList.size());
    EXPECT_EQ(data.getCoverage()->getNumOfCodeElements(), clusterList["full"].getCodeElements().size());
    EXPECT_EQ(data.getCoverage()->getNumOfTestcases(), clusterList["full"].getTestCases().size());
}

TEST_F(CTestSuiteClusterPluginsTest, TestSuiteCoverageClusterPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("coverage"));
    EXPECT_NO_THROW(plugin->init(doc));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(5u, clusterList.size());
    EXPECT_EQ(10u, clusterList["coverage-10"].getTestCases().size());
    EXPECT_EQ(99u, clusterList["coverage-10"].getTestCases()[0]);
    EXPECT_EQ(50u, clusterList["coverage-50"].getTestCases()[49]);
}

TEST_F(CTestSuiteClusterPluginsTest, TestSuiteDuplationClusterPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("duplation"));
    EXPECT_NO_THROW(plugin->init(doc));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(5u, clusterList.size());
    EXPECT_EQ(10u, clusterList["duplation-10"].getTestCases().size());
    EXPECT_EQ(49u, clusterList["duplation-10"].getTestCases()[0]);
    EXPECT_EQ(40u, clusterList["duplation-50"].getTestCases()[49]);
}

TEST_F(CTestSuiteClusterPluginsTest, TestSuiteRandomClusterPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("random"));
    EXPECT_NO_THROW(plugin->init(doc));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(5u, clusterList.size());
    EXPECT_EQ(10u, clusterList["random-10"].getTestCases().size());
    EXPECT_EQ(100u, clusterList["random-10"].getCodeElements().size());
    EXPECT_EQ(50u, clusterList["random-50"].getTestCases().size());
}

TEST_F(CTestSuiteClusterPluginsTest, TestSuiteLabelTestCodeElementClusterPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("label-test-codeelements"));
    EXPECT_NO_THROW(plugin->init(doc));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(4u, clusterList.size());
    EXPECT_EQ(3u, clusterList["sample - sample"].getTestCases().size());
    EXPECT_EQ(9u, clusterList["sample - sample"].getCodeElements().size());
    EXPECT_EQ(3u, clusterList["sample2 - sample"].getTestCases().size());
    EXPECT_EQ(6u, clusterList["sample - sample2"].getCodeElements().size());
}
