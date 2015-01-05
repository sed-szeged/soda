/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors:
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
#include "engine/CKernel.h"
#include "engine/plugin/ITestSuitePrioritizationPlugin.h"
#include "data/CSelectionData.h"

using namespace soda;

class CTestSuitePrioritizationPluginsTest : public testing::Test
{
protected:
    CSelectionData *data;
    ITestSuitePrioritizationPlugin *plugin;
    IntVector result;
    CKernel kernel;

    virtual void SetUp() {
        // Create selection data.
        data = new CSelectionData();
        data->loadChangeset("sample/ChangesetOneRevisionPerFileTestSelectionBit");
        data->loadCoverage("sample/CoverageMatrixOneTestPerFileTestSelectionBit");
        data->loadResults("sample/ResultsMatrixDejaGNUOneRevisionPerFileTestSelectionBit");
        plugin = NULL;
    }

    virtual void TearDown() {
        delete data;
    }
};

TEST_F(CTestSuitePrioritizationPluginsTest, GeneralIgnorePrioritizationPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("general-ignore"));

    EXPECT_EQ("general-ignore", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuitePrioritizationPluginsTest, GeneralIgnorePrioritizationPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("general-ignore"));
    EXPECT_NO_THROW(plugin->reset(1));
    EXPECT_NO_THROW(plugin->init(data));
    EXPECT_NO_THROW(plugin->fillSelection(result, 100));

    EXPECT_EQ(3u, result[0]);
    EXPECT_EQ(4u, result[1]);
    EXPECT_EQ(2u, result[2]);
    EXPECT_EQ(1u, result[3]);
    EXPECT_EQ(0u, result[4]);
}

TEST_F(CTestSuitePrioritizationPluginsTest, AdditionalGeneralIgnorePrioritizationPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("additional-general-ignore"));

    EXPECT_EQ("additional-general-ignore", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuitePrioritizationPluginsTest, AdditionalGeneralIgnorePrioritizationPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("additional-general-ignore"));
    EXPECT_NO_THROW(plugin->reset(1));
    EXPECT_NO_THROW(plugin->init(data));
    EXPECT_NO_THROW(plugin->fillSelection(result, 100));

    EXPECT_EQ(3u, result[0]);
    EXPECT_EQ(1u, result[1]);
    EXPECT_EQ(4u, result[2]);
    EXPECT_EQ(0u, result[3]);
    EXPECT_EQ(2u, result[4]);
}

TEST_F(CTestSuitePrioritizationPluginsTest, RandomIgnorePrioritizationPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("random-ignore"));

    EXPECT_EQ("random-ignore", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CTestSuitePrioritizationPluginsTest, RandomIgnorePrioritizationPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("random-ignore"));
    EXPECT_NO_THROW(plugin->reset(1));
    EXPECT_NO_THROW(plugin->init(data));
    EXPECT_NO_THROW(plugin->fillSelection(result, 100));
    EXPECT_EQ(5u, result.size());
}
