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
#include "boost/lexical_cast.hpp"
#include "io/CJsonReader.h"
#include "engine/CKernel.h"
#include "engine/plugin/ITestSuiteReductionPlugin.h"
#include "data/CSelectionData.h"

using namespace soda;

class CTestSuiteReductionPluginsTest : public testing::Test
{
protected:
    rapidjson::Document results;
    CSelectionData data;
    ITestSuiteReductionPlugin *plugin;
    CKernel kernel;
    io::CJsonReader params;
    std::ofstream outStream;

    virtual void SetUp() {
        // Create selection data.
        std::srand(std::time(0));
        for (int i = 0; i < 100; ++i) {
            for (int j = 0; j < 100; ++j) {
                data.getCoverage()->addOrSetRelation("test-" + boost::lexical_cast<String>(i), "ce-" + boost::lexical_cast<String>(j), (j <= i) ? true : false);
            }
        }
        data.getCoverage()->setRelation(98,99,true);
        boost::property_tree::ptree pt, reductionSizes, size;
        pt.put("iteration", "4");
        size.put("", 1);
        reductionSizes.push_back(std::make_pair("", size));
        pt.add_child("reduction-sizes", reductionSizes);
        pt.put("program-name", "test");
        pt.put("output-dir", "sample/");
        params = io::CJsonReader(pt);
        plugin = NULL;
        outStream.open("sample/reduction.txt");
    }

    virtual void TearDown() {
    }
};

TEST_F(CTestSuiteReductionPluginsTest, CoverageReductionPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteReductionPluginManager().getPlugin("coverage"));
    EXPECT_EQ("coverage", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuiteReductionPluginsTest, CoverageReductionPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteReductionPluginManager().getPlugin("coverage"));
    EXPECT_NO_THROW(plugin->init(&data, params));
    EXPECT_NO_THROW(plugin->reduction(outStream));

    CCoverageMatrix coverage;
    EXPECT_NO_THROW(coverage.load("sample/test-GENITER.cov.006.SoDA"));
    EXPECT_EQ(63u, coverage.getNumOfTestcases());
    StringVector res = coverage.getTestcases().getValueList();
    EXPECT_EQ("test-99", res[0]);
    EXPECT_EQ("test-97", res[1]);
    EXPECT_EQ("test-98", res[2]);
    EXPECT_EQ("test-93", res[3]);

    EXPECT_NO_THROW(coverage.load("sample/test-GENITER.cov.004.SoDA"));
    EXPECT_EQ(15u, coverage.getNumOfTestcases());

    EXPECT_NO_THROW(coverage.load("sample/test-GENITER.cov.003.SoDA"));
    EXPECT_EQ(7u, coverage.getNumOfTestcases());
}

TEST_F(CTestSuiteReductionPluginsTest, AdditionalCoverageReductionPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteReductionPluginManager().getPlugin("additional-coverage"));
    EXPECT_EQ("additional-coverage", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuiteReductionPluginsTest, AdditionalCoverageReductionPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteReductionPluginManager().getPlugin("additional-coverage"));
    EXPECT_NO_THROW(plugin->init(&data, params));
    EXPECT_NO_THROW(plugin->reduction(outStream));

    CCoverageMatrix coverage;
    EXPECT_NO_THROW(coverage.load("sample/test-ADD-GEN-ITER.cov.006.SoDA"));
    EXPECT_EQ(63u, coverage.getNumOfTestcases());
    StringVector res = coverage.getTestcases().getValueList();
    EXPECT_EQ("test-99", res[0]);
    EXPECT_EQ("test-26", res[1]);
    EXPECT_EQ("test-27", res[2]);
    EXPECT_EQ("test-28", res[3]);

    EXPECT_NO_THROW(coverage.load("sample/test-ADD-GEN-ITER.cov.004.SoDA"));
    EXPECT_EQ(15u, coverage.getNumOfTestcases());

    EXPECT_NO_THROW(coverage.load("sample/test-ADD-GEN-ITER.cov.003.SoDA"));
    EXPECT_EQ(7u, coverage.getNumOfTestcases());
}

TEST_F(CTestSuiteReductionPluginsTest, DuplationReductionPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteReductionPluginManager().getPlugin("duplation"));
    EXPECT_EQ("duplation", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuiteReductionPluginsTest, DuplationReductionPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteReductionPluginManager().getPlugin("duplation"));
    EXPECT_NO_THROW(plugin->init(&data, params));
    EXPECT_NO_THROW(plugin->reduction(outStream));

    CCoverageMatrix coverage;
    EXPECT_NO_THROW(coverage.load("sample/test-DUPLATION.cov.003.SoDA"));
    StringVector res = coverage.getTestcases().getValueList();
    EXPECT_EQ(7u, coverage.getNumOfTestcases());
    EXPECT_EQ("test-49", res[0]);
    EXPECT_EQ("test-24", res[1]);
    EXPECT_EQ("test-74", res[2]);
    EXPECT_EQ("test-11", res[3]);
}

TEST_F(CTestSuiteReductionPluginsTest, RandomReductionPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteReductionPluginManager().getPlugin("random"));
    EXPECT_EQ("random", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuiteReductionPluginsTest, RandomReductionPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuiteReductionPluginManager().getPlugin("random"));
    EXPECT_NO_THROW(plugin->init(&data, params));
    EXPECT_NO_THROW(plugin->reduction(outStream));

    CCoverageMatrix coverage;
    EXPECT_NO_THROW(coverage.load("sample/test-RAND-ITER.cov.003.SoDA"));
    EXPECT_EQ(7u, coverage.getNumOfTestcases());
}
