/*
 * Copyright (C): 2013-2015 Department of Software Engineering, University of Szeged
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

class TestSuiteClusterPluginsTest : public testing::Test
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

        val.PushBack(1000, doc.GetAllocator());
        doc.AddMember("cluster-sizes", val, doc.GetAllocator());
        doc.AddMember("cluster-test-list", "sample/ClusterPluginSampleDir/label-test-codeelement-test.txt", doc.GetAllocator());
        doc.AddMember("cluster-code-elements-list", "sample/ClusterPluginSampleDir/label-test-codeelement-method.txt", doc.GetAllocator());

        doc.AddMember("test_clusters_dump", "sample/ClusterPluginSampleDir/testDump.txt", doc.GetAllocator());
        doc.AddMember("codeelement_clusters_dump", "sample/ClusterPluginSampleDir/codeDump.txt", doc.GetAllocator());
    }

    virtual void TearDown() {
    }
};




TEST_F(TestSuiteClusterPluginsTest, TestSuiteHammingClusterPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("hamming"));
    EXPECT_EQ("hamming", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(TestSuiteClusterPluginsTest, TestSuiteHamming01ClusterPlugin)
{
    doc.AddMember("hamming_dist_row(%)", 50, doc.GetAllocator());
    doc.AddMember("hamming_dist_cols(%)", 50, doc.GetAllocator());
    doc.AddMember("0cluster(%)", 100, doc.GetAllocator());

    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("hamming"));
    EXPECT_NO_THROW(plugin->init(doc));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(3, clusterList.size());   // two cluster + (empty) nullcluster
    EXPECT_EQ(0, clusterList["0"].getCodeElements().size());
    EXPECT_EQ(0, clusterList["0"].getTestCases().size());
    EXPECT_EQ(50, clusterList["1"].getCodeElements().size());
    EXPECT_EQ(50, clusterList["2"].getTestCases().size());

}

TEST_F(TestSuiteClusterPluginsTest, TestSuiteHamming02ClusterPlugin)
{
    doc.AddMember("hamming_dist_row(%)", 20, doc.GetAllocator());
    doc.AddMember("hamming_dist_cols(%)", 50, doc.GetAllocator());
    doc.AddMember("0cluster(%)", 90, doc.GetAllocator());

    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("hamming"));
    EXPECT_NO_THROW(plugin->init(doc));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(6, clusterList.size()); // five cluster + nullcluster
    EXPECT_EQ(10, clusterList["0"].getCodeElements().size());
    EXPECT_EQ(50, clusterList["1"].getCodeElements().size());
    EXPECT_EQ(20, clusterList["2"].getTestCases().size());
}


TEST_F(TestSuiteClusterPluginsTest, TestSuiteOchiaiDiceJaccardClusterPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("ochiai-dice-jaccard"));
    EXPECT_EQ("ochiai-dice-jaccard", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}


TEST_F(TestSuiteClusterPluginsTest, TestSuiteOchiaiClusterPlugin)
{
    doc.AddMember("alg.index", 0, doc.GetAllocator());
    doc.AddMember("limit", -1.0, doc.GetAllocator());
    doc.AddMember("0cluster(%)", 100, doc.GetAllocator());
    doc.AddMember("cluster-number", 2, doc.GetAllocator());

    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("ochiai-dice-jaccard"));
    EXPECT_NO_THROW(plugin->init(doc));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(3, clusterList.size());
    EXPECT_EQ(0, clusterList["0"].getCodeElements().size());
    EXPECT_EQ(0, clusterList["0"].getTestCases().size());
    EXPECT_EQ(69, clusterList["1"].getCodeElements().size());
    EXPECT_EQ(31, clusterList["1"].getTestCases().size());
    EXPECT_EQ(31, clusterList["2"].getCodeElements().size());
    EXPECT_EQ(69, clusterList["2"].getTestCases().size());
}


TEST_F(TestSuiteClusterPluginsTest, TestSuiteDiceClusterPlugin)
{
    doc.AddMember("alg.index", 1, doc.GetAllocator());
    doc.AddMember("limit", -1.0, doc.GetAllocator());
    doc.AddMember("0cluster(%)", 95, doc.GetAllocator());
    doc.AddMember("cluster-number", 3, doc.GetAllocator());

    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("ochiai-dice-jaccard"));
    EXPECT_NO_THROW(plugin->init(doc));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(4, clusterList.size());
    EXPECT_EQ(5, clusterList["0"].getCodeElements().size());
    EXPECT_EQ(17, clusterList["1"].getTestCases().size());
    EXPECT_EQ(49, clusterList["2"].getCodeElements().size());
    EXPECT_EQ(49, clusterList["3"].getTestCases().size());
}


TEST_F(TestSuiteClusterPluginsTest, TestSuiteJaccardClusterPlugin)
{
    doc.AddMember("alg.index", 2, doc.GetAllocator());
    doc.AddMember("limit", -1.0, doc.GetAllocator());
    doc.AddMember("0cluster(%)", 90, doc.GetAllocator());
    doc.AddMember("cluster-number", 4, doc.GetAllocator());

    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("ochiai-dice-jaccard"));
    EXPECT_NO_THROW(plugin->init(doc));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(10, clusterList["0"].getCodeElements().size());

    EXPECT_LE(clusterList["1"].getCodeElements().size(), clusterList["4"].getTestCases().size());
    EXPECT_GE(clusterList["1"].getCodeElements().size(), clusterList["4"].getTestCases().size());

    EXPECT_LE(clusterList["2"].getCodeElements().size(), clusterList["3"].getTestCases().size());
    EXPECT_GE(clusterList["2"].getCodeElements().size(), clusterList["3"].getTestCases().size());

}



TEST_F(TestSuiteClusterPluginsTest, TestSuiteOneClusterPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("one-cluster"));
    EXPECT_NO_THROW(plugin->init(doc));

    EXPECT_EQ("one-cluster", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
    EXPECT_EQ(0, plugin->getRequiredParameters().size());
}

TEST_F(TestSuiteClusterPluginsTest, TestSuiteOneClusterPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("one-cluster"));
    EXPECT_NO_THROW(plugin->init(doc));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(1u, clusterList.size());
    EXPECT_EQ(data.getCoverage()->getNumOfCodeElements(), clusterList["full"].getCodeElements().size());
    EXPECT_EQ(data.getCoverage()->getNumOfTestcases(), clusterList["full"].getTestCases().size());
}


TEST_F(TestSuiteClusterPluginsTest, TestSuiteCoverageClusterPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("coverage"));
    EXPECT_NO_THROW(plugin->init(doc));

    EXPECT_EQ("coverage", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
    EXPECT_EQ(1, plugin->getRequiredParameters().count("cluster-sizes"));
}

TEST_F(TestSuiteClusterPluginsTest, TestSuiteCoverageClusterPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("coverage"));
    EXPECT_NO_THROW(plugin->init(doc));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(5u, clusterList.size());
    EXPECT_EQ(10u, clusterList["coverage-10"].getTestCases().size());
    EXPECT_EQ(99u, clusterList["coverage-10"].getTestCases()[0]);
    EXPECT_EQ(50u, clusterList["coverage-50"].getTestCases()[49]);
}

TEST_F(TestSuiteClusterPluginsTest, TestSuiteDuplationClusterPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("duplation"));
    EXPECT_NO_THROW(plugin->init(doc));

    EXPECT_EQ("duplation", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
    EXPECT_EQ(1, plugin->getRequiredParameters().count("cluster-sizes"));
}

TEST_F(TestSuiteClusterPluginsTest, TestSuiteDuplationClusterPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("duplation"));
    EXPECT_NO_THROW(plugin->init(doc));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(5u, clusterList.size());
    EXPECT_EQ(10u, clusterList["duplation-10"].getTestCases().size());
    EXPECT_EQ(49u, clusterList["duplation-10"].getTestCases()[0]);
    EXPECT_EQ(40u, clusterList["duplation-50"].getTestCases()[49]);
}

TEST_F(TestSuiteClusterPluginsTest, TestSuiteRandomClusterPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("random"));
    EXPECT_NO_THROW(plugin->init(doc));

    EXPECT_EQ("random", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
    EXPECT_EQ(1, plugin->getRequiredParameters().count("cluster-sizes"));
}

TEST_F(TestSuiteClusterPluginsTest, TestSuiteRandomClusterPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("random"));
    EXPECT_NO_THROW(plugin->init(doc));
    EXPECT_NO_THROW(plugin->execute(data, clusterList));

    EXPECT_EQ(5u, clusterList.size());
    EXPECT_EQ(10u, clusterList["random-10"].getTestCases().size());
    EXPECT_EQ(100u, clusterList["random-10"].getCodeElements().size());
    EXPECT_EQ(50u, clusterList["random-50"].getTestCases().size());
}

TEST_F(TestSuiteClusterPluginsTest, TestSuiteLabelTestCodeElementClusterPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteClusterPluginManager().getPlugin("label-test-codeelements"));
    EXPECT_NO_THROW(plugin->init(doc));

    EXPECT_EQ("label-test-codeelements", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
    EXPECT_EQ(1, plugin->getRequiredParameters().count("cluster-test-list"));
    EXPECT_EQ(1, plugin->getRequiredParameters().count("cluster-code-elements-list"));
}

TEST_F(TestSuiteClusterPluginsTest, TestSuiteLabelTestCodeElementClusterPlugin)
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
