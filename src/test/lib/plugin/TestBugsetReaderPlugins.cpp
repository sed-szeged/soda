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
#include "engine/CKernel.h"
#include "engine/plugin/IBugsetReaderPlugin.h"
#include "exception/CException.h"

using namespace soda;

extern CKernel kernel;

class BugsetReaderPluginsTest : public testing::Test
{
protected:
    CBugset* bugset;
    IBugsetReaderPlugin* plugin;
    variables_map vm;

    virtual void SetUp() {
        plugin = NULL;
        bugset = NULL;
    }

    virtual void TearDown() {
        delete bugset;
    }
};

TEST_F(BugsetReaderPluginsTest, CsvFileBugsetReaderPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getBugsetReaderPluginManager().getPlugin("csv-file"));
    EXPECT_EQ("csv-file", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(BugsetReaderPluginsTest, CsvFileBugsetReaderPluginUnknownPath)
{
    EXPECT_NO_THROW(plugin = kernel.getBugsetReaderPluginManager().getPlugin("csv-file"));
    EXPECT_THROW(bugset = plugin->read("not_existing_path"), CException);
}

TEST_F(BugsetReaderPluginsTest, CsvFileBugsetReaderPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getBugsetReaderPluginManager().getPlugin("csv-file"));
    (bugset = plugin->read("sample/CsvFileBugsetSampleDir/"));

    EXPECT_EQ(23u, bugset->getRevisions().size());
    EXPECT_EQ(25u, bugset->getReports().size());
}
