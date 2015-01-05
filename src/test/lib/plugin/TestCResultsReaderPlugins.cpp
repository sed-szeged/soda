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
#include "engine/plugin/IResultsReaderPlugin.h"
#include "exception/CException.h"

using namespace soda;

class CResultsReaderPluginsTest : public testing::Test
{
protected:
    CResultsMatrix* resultsMatrix;
    IResultsReaderPlugin *plugin;
    CKernel kernel;

    virtual void SetUp() {
        plugin = NULL;
        resultsMatrix = NULL;
    }

    virtual void TearDown() {
        delete resultsMatrix;
        resultsMatrix = 0;
    }
};

TEST_F(CResultsReaderPluginsTest, DejaGNUOneRevisionPerFileResultsReaderPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getResultsReaderPluginManager().getPlugin("dejagnu-one-revision-per-file"));
    EXPECT_EQ("dejagnu-one-revision-per-file", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CResultsReaderPluginsTest, DejaGNUOneRevisionPerFileResultsReaderPluginUnknownPath)
{
    EXPECT_NO_THROW(plugin = kernel.getResultsReaderPluginManager().getPlugin("dejagnu-one-revision-per-file"));
    EXPECT_THROW(plugin->read("sample/this_dir_does_not_exists"), CException);
}

TEST_F(CResultsReaderPluginsTest, DejaGNUOneRevisionPerFileResultsReaderPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getResultsReaderPluginManager().getPlugin("dejagnu-one-revision-per-file"));
    EXPECT_NO_THROW(resultsMatrix = plugin->read("sample/ResultsMatrixDejaGNUOneRevisionPerFileSampleDir"));

    EXPECT_EQ(2u, resultsMatrix->getNumOfRevisions());
    EXPECT_EQ(28u, resultsMatrix->getNumOfTestcases());
}
