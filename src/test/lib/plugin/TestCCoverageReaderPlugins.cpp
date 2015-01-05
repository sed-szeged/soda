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

#include <boost/program_options.hpp>
#include "gtest/gtest.h"
#include "engine/CKernel.h"
#include "engine/plugin/ICoverageReaderPlugin.h"
#include "exception/CException.h"

using namespace boost::program_options;
using namespace soda;

class CCoverageReaderPluginsTest : public testing::Test
{
protected:
    CCoverageMatrix* coverageMatrix;
    ICoverageReaderPlugin* plugin;
    CKernel kernel;
    variables_map vm;

    virtual void SetUp() {
        plugin = NULL;
        coverageMatrix = NULL;
    }

    virtual void TearDown() {
        delete coverageMatrix;
        coverageMatrix = 0;
    }
};

TEST_F(CCoverageReaderPluginsTest, OneTestPerFileCoverageReaderPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("one-test-per-file"));
    EXPECT_EQ("one-test-per-file", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CCoverageReaderPluginsTest, OneTestPerFileCoverageReaderPluginUnknownPath)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("one-test-per-file"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/this_dir_does_not_exists"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_THROW(coverageMatrix = plugin->read(vm), CException);
}

TEST_F(CCoverageReaderPluginsTest, OneTestPerFileCoverageReaderPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("one-test-per-file"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/CoverageMatrixOneTestPerFileSampleDir"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_NO_THROW(coverageMatrix = plugin->read(vm));

    EXPECT_EQ(3u, coverageMatrix->getNumOfTestcases());
    EXPECT_EQ(824u, coverageMatrix->getNumOfCodeElements());
}

TEST_F(CCoverageReaderPluginsTest, GcovCoverageReaderPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("gcov"));
    EXPECT_EQ("gcov", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CCoverageReaderPluginsTest, GcovCoverageReaderPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("gcov"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/CoverageMatrixGcovSampleDir"), ""))));
    EXPECT_NO_THROW(vm.insert(std::make_pair("cut-source-path", variable_value(String("github/soda"), ""))));
    EXPECT_NO_THROW(vm.insert(std::make_pair("filter-input-files", variable_value(String(""), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_NO_THROW(coverageMatrix = plugin->read(vm));

    EXPECT_EQ(1u, coverageMatrix->getNumOfTestcases());
    EXPECT_EQ("full", coverageMatrix->getTestcases().getValue(0));
    EXPECT_EQ("/src/lib/SoDA/src/io/CBinaryIO.cpp:28", coverageMatrix->getCodeElements().getValue(0));
    EXPECT_NO_THROW(coverageMatrix->getCodeElements().getID("/src/lib/SoDA/src/io/filtered.cpp:28"));
    EXPECT_EQ(542, coverageMatrix->getNumOfCodeElements());
}

TEST_F(CCoverageReaderPluginsTest, GcovCoverageReaderPluginFilter)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("gcov"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/CoverageMatrixGcovSampleDir"), ""))));
    EXPECT_NO_THROW(vm.insert(std::make_pair("cut-source-path", variable_value(String("github/soda"), ""))));
    EXPECT_NO_THROW(vm.insert(std::make_pair("filter-input-files", variable_value(String(".*filtered.cpp.*"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_NO_THROW(coverageMatrix = plugin->read(vm));

    EXPECT_EQ(1u, coverageMatrix->getNumOfTestcases());
    EXPECT_EQ("full", coverageMatrix->getTestcases().getValue(0));
    EXPECT_THROW(coverageMatrix->getCodeElements().getID("/src/lib/SoDA/src/io/filtered.cpp:28"), std::out_of_range);
    EXPECT_EQ(328, coverageMatrix->getNumOfCodeElements());
}
