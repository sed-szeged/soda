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
    CKernel kernel;

    virtual void SetUp() {
        // Create selection data.
        selection.loadCoverage("sample/MetricPluginSampleDir/SoDA.extended.cov.SoDA");
        selection.loadResults("sample/MetricPluginSampleDir/SoDA.res.SoDA");
        selection.globalize();
        ITestSuiteClusterPlugin *clusterAlg = kernel.getTestSuiteClusterPluginManager().getPlugin("label-test-codeelements");
        rapidjson::Document doc;
        doc.SetObject();
        doc.AddMember("cluster-test-list", "sample/MetricPluginSampleDir/test-cluster.txt", doc.GetAllocator());
        doc.AddMember("cluster-code-elements-list", "sample/MetricPluginSampleDir/method-cluster.txt", doc.GetAllocator());
        clusterAlg->init(doc);
        clusterAlg->execute(selection, clusterList);
        results.SetObject();
        plugin = NULL;
    }

    virtual void TearDown() {
    }
};

TEST_F(CTestSuiteMetricPluginsTest, FaultDetectionMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("coverage"));
    EXPECT_EQ("coverage", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuiteMetricPluginsTest, FaultDetection)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("coverage"));
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 1));
    EXPECT_NO_THROW(plugin->calculate(results));

    EXPECT_DOUBLE_EQ(0.80952380952380953, results["cluster - cluster"]["coverage"].GetDouble());
}

TEST_F(CTestSuiteMetricPluginsTest, CoverageEfficiencyMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("coverage-efficiency"));
    EXPECT_EQ("coverage-efficiency", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuiteMetricPluginsTest, CoverageEfficiency)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("coverage-efficiency"));
    StringVector dependencies = plugin->getDependency();
    for (StringVector::iterator it = dependencies.begin(); it != dependencies.end(); it++) {
        ITestSuiteMetricPlugin *dep = kernel.getTestSuiteMetricPluginManager().getPlugin(*it);
        dep->init(&selection, &clusterList, 1);
        dep->calculate(results);
    }
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 1));
    EXPECT_NO_THROW(plugin->calculate(results));

    EXPECT_DOUBLE_EQ(3.3999999999999999, results["cluster - cluster"]["coverage-efficiency"].GetDouble());
}

TEST_F(CTestSuiteMetricPluginsTest, FaultLocalizationMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("fault-localization"));
    EXPECT_EQ("fault-localization", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuiteMetricPluginsTest, FaultLocalization)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("fault-localization"));
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 1));
    EXPECT_NO_THROW(plugin->calculate(results));

    EXPECT_DOUBLE_EQ(0.13333333333333333, results["cluster - cluster"]["fault-localization"].GetDouble());
}

TEST_F(CTestSuiteMetricPluginsTest, FMeasureMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("f-measure"));
    EXPECT_EQ("f-measure", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuiteMetricPluginsTest, FMeasure)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("f-measure"));
    StringVector dep = plugin->getDependency();
    for (StringVector::iterator it = dep.begin(); it != dep.end(); it++) {
        ITestSuiteMetricPlugin *depPlugin = kernel.getTestSuiteMetricPluginManager().getPlugin(*it);
        StringVector dep2 = depPlugin->getDependency();
        for (StringVector::iterator it2 = dep2.begin(); it2 != dep2.end(); it2++) {
            ITestSuiteMetricPlugin *dep2Plugin = kernel.getTestSuiteMetricPluginManager().getPlugin(*it2);
            dep2Plugin->init(&selection, &clusterList, 1);
            dep2Plugin->calculate(results);
        }
        depPlugin->init(&selection, &clusterList, 1);
        depPlugin->calculate(results);
    }
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 1));
    EXPECT_NO_THROW(plugin->calculate(results));

    EXPECT_DOUBLE_EQ(0.26432676155954388, results["cluster - cluster"]["f-measure"].GetDouble());
}

TEST_F(CTestSuiteMetricPluginsTest, TpceMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("tpce"));
    EXPECT_EQ("tpce", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuiteMetricPluginsTest, Tpce)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("tpce"));
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 1));
    EXPECT_NO_THROW(plugin->calculate(results));

    EXPECT_DOUBLE_EQ(0.23809523809523808, results["cluster - cluster"]["tpce"].GetDouble());
}

TEST_F(CTestSuiteMetricPluginsTest, SpecializationMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("specialization"));
    EXPECT_EQ("specialization", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuiteMetricPluginsTest, Specialization)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("specialization"));
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 1));
    EXPECT_NO_THROW(plugin->calculate(results));

    EXPECT_DOUBLE_EQ(0.5, results["cluster - cluster"]["specialization"].GetDouble());
}

TEST_F(CTestSuiteMetricPluginsTest, UniquenessMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("uniqueness"));
    EXPECT_EQ("uniqueness", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuiteMetricPluginsTest, Uniqueness)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("uniqueness"));
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 1));
    EXPECT_NO_THROW(plugin->calculate(results));

    EXPECT_DOUBLE_EQ(0.58823529411764708, results["cluster - cluster"]["uniqueness"].GetDouble());
}

TEST_F(CTestSuiteMetricPluginsTest, PartitionMetricMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("partition-metric"));
    EXPECT_EQ("partition-metric", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuiteMetricPluginsTest, PartitionMetric)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("partition-metric"));
    StringVector dependencies = plugin->getDependency();
    for (StringVector::iterator it = dependencies.begin(); it != dependencies.end(); it++) {
        ITestSuiteMetricPlugin *dep = kernel.getTestSuiteMetricPluginManager().getPlugin(*it);
        dep->init(&selection, &clusterList, 1);
        dep->calculate(results);
    }
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 1));
    EXPECT_NO_THROW(plugin->calculate(results));

    EXPECT_DOUBLE_EQ(0.8666666666666667, results["cluster - cluster"]["partition-metric"].GetDouble());
}

TEST_F(CTestSuiteMetricPluginsTest, PartitionEfficiencyMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("partition-efficiency"));
    EXPECT_EQ("partition-efficiency", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuiteMetricPluginsTest, PartitionEfficiency)
{
    plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("fault-localization");
    plugin->init(&selection, &clusterList, 1);
    plugin->calculate(results);
    plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("partition-metric");
    plugin->init(&selection, &clusterList, 1);
    plugin->calculate(results);
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteMetricPluginManager().getPlugin("partition-efficiency"));
    EXPECT_NO_THROW(plugin->init(&selection, &clusterList, 1));
    EXPECT_NO_THROW(plugin->calculate(results));

    EXPECT_DOUBLE_EQ(0.17333333333333334, results["cluster - cluster"]["partition-efficiency"].GetDouble());
}
