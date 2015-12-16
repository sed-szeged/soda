/*
 * Copyright (C): 2013-2015 Department of Software Engineering, University of Szeged
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

CKernel kernel;

class CoverageReaderPluginsTest : public testing::Test
{
protected:
    CCoverageMatrix* coverageMatrix;
    ICoverageReaderPlugin* plugin;
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

TEST_F(CoverageReaderPluginsTest, OneTestPerFileCoverageReaderPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("one-test-per-file"));
    EXPECT_EQ("one-test-per-file", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CoverageReaderPluginsTest, OneTestPerFileCoverageReaderPluginUnknownPath)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("one-test-per-file"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/this_dir_does_not_exists"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_THROW(coverageMatrix = plugin->read(vm), CException);
}

TEST_F(CoverageReaderPluginsTest, OneTestPerFileCoverageReaderPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("one-test-per-file"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/CoverageMatrixOneTestPerFileSampleDir"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_NO_THROW(coverageMatrix = plugin->read(vm));

    EXPECT_EQ(3u, coverageMatrix->getNumOfTestcases());
    EXPECT_EQ(824u, coverageMatrix->getNumOfCodeElements());
}

TEST_F(CoverageReaderPluginsTest, GcovCoverageReaderPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("gcov"));
    EXPECT_EQ("gcov", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(CoverageReaderPluginsTest, GcovCoverageReaderPlugin)
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

TEST_F(CoverageReaderPluginsTest, GcovCoverageReaderPluginFilter)
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

TEST_F(CoverageReaderPluginsTest, EmmaJavaCoverageReaderPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("emma-java"));
    EXPECT_EQ("emma-java", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

/*TEST_F(CoverageReaderPluginsTest, EmmaJavaCoverageReaderPluginUnknownPath)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("emma-java"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/this_dir_does_not_exists"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_THROW(coverageMatrix = plugin->read(vm), CException);
}*/

TEST_F(CoverageReaderPluginsTest, EmmaJavaCoverageReaderPluginPackage)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("emma-java"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/EmmaCoverageSampleDir"), ""))));
    EXPECT_NO_THROW(vm.insert(std::make_pair("granularity", variable_value(String("package"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_NO_THROW(coverageMatrix = plugin->read(vm));

    EXPECT_EQ(1u, coverageMatrix->getNumOfTestcases());
    EXPECT_EQ(8u, coverageMatrix->getNumOfCodeElements());

    EXPECT_TRUE(coverageMatrix->getRelation("sample_test", "com.sun.tools.javac.v8.resources"));
}

TEST_F(CoverageReaderPluginsTest, EmmaJavaCoverageReaderPluginSrc)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("emma-java"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/EmmaCoverageSampleDir"), ""))));
    EXPECT_NO_THROW(vm.insert(std::make_pair("granularity", variable_value(String("src"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_NO_THROW(coverageMatrix = plugin->read(vm));

    EXPECT_EQ(1u, coverageMatrix->getNumOfTestcases());
    EXPECT_EQ(62u, coverageMatrix->getNumOfCodeElements());

    EXPECT_FALSE(coverageMatrix->getRelation("sample_test", "com.sun.tools.javac.v8.resources.compiler_ja.java"));
    EXPECT_TRUE(coverageMatrix->getRelation("sample_test", "com.sun.tools.javac.v8.resources.compiler.java"));
}

TEST_F(CoverageReaderPluginsTest, EmmaJavaCoverageReaderPluginClass)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("emma-java"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/EmmaCoverageSampleDir"), ""))));
    EXPECT_NO_THROW(vm.insert(std::make_pair("granularity", variable_value(String("class"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_NO_THROW(coverageMatrix = plugin->read(vm));

    EXPECT_EQ(1u, coverageMatrix->getNumOfTestcases());
    EXPECT_EQ(185u, coverageMatrix->getNumOfCodeElements());

    EXPECT_FALSE(coverageMatrix->getRelation("sample_test", "com.sun.tools.javac.v8.resources.compiler_ja"));
    EXPECT_TRUE(coverageMatrix->getRelation("sample_test", "com.sun.tools.javac.v8.resources.compiler"));
}

TEST_F(CoverageReaderPluginsTest, EmmaJavaCoverageReaderPluginMethod)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("emma-java"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/EmmaCoverageSampleDir"), ""))));
    EXPECT_NO_THROW(vm.insert(std::make_pair("granularity", variable_value(String("method"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_NO_THROW(coverageMatrix = plugin->read(vm));

    EXPECT_EQ(1u, coverageMatrix->getNumOfTestcases());
    EXPECT_EQ(2061u, coverageMatrix->getNumOfCodeElements());

    EXPECT_FALSE(coverageMatrix->getRelation("sample_test", "com.sun.tools.javac.v8.resources.javac_ja.getContents (): Object [][]"));
    EXPECT_TRUE(coverageMatrix->getRelation("sample_test", "com.sun.tools.javac.v8.resources.compiler.compiler (): void"));
}

TEST_F(CoverageReaderPluginsTest, JacocoJavaCoverageReaderPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("jacoco-java"));
    EXPECT_EQ("jacoco-java", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

/*TEST_F(CoverageReaderPluginsTest, JacocoJavaCoverageReaderPluginUnknownPath)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("jacoco-java"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/this_dir_does_not_exists"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_THROW(coverageMatrix = plugin->read(vm), CException);
}*/

TEST_F(CoverageReaderPluginsTest, JacocoJavaCoverageReaderPluginPackage)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("jacoco-java"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/JacocoCoverageSampleDir"), ""))));
    EXPECT_NO_THROW(vm.insert(std::make_pair("granularity", variable_value(String("package"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_NO_THROW(coverageMatrix = plugin->read(vm));

    EXPECT_EQ(1u, coverageMatrix->getNumOfTestcases());
    EXPECT_EQ(27u, coverageMatrix->getNumOfCodeElements());

    EXPECT_FALSE(coverageMatrix->getRelation("testWasRun(junit.tests.framework.TestCaseTest).testWasRun on testWasRun(junit.tests.framework.TestCaseTest)", "org/junit/internal"));
    EXPECT_TRUE(coverageMatrix->getRelation("testWasRun(junit.tests.framework.TestCaseTest).testWasRun on testWasRun(junit.tests.framework.TestCaseTest)", "org/junit/runner"));
}

TEST_F(CoverageReaderPluginsTest, JacocoJavaCoverageReaderPluginSrc)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("jacoco-java"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/JacocoCoverageSampleDir"), ""))));
    EXPECT_NO_THROW(vm.insert(std::make_pair("granularity", variable_value(String("src"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_NO_THROW(coverageMatrix = plugin->read(vm));

    EXPECT_EQ(1u, coverageMatrix->getNumOfTestcases());
    EXPECT_EQ(123u, coverageMatrix->getNumOfCodeElements());

    EXPECT_FALSE(coverageMatrix->getRelation("testWasRun(junit.tests.framework.TestCaseTest).testWasRun on testWasRun(junit.tests.framework.TestCaseTest)", "org/junit/internal/ArrayComparisonFailure.java"));
    EXPECT_TRUE(coverageMatrix->getRelation("testWasRun(junit.tests.framework.TestCaseTest).testWasRun on testWasRun(junit.tests.framework.TestCaseTest)", "org/junit/runner/Description.java"));
}

TEST_F(CoverageReaderPluginsTest, JacocoJavaCoverageReaderPluginClass)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("jacoco-java"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/JacocoCoverageSampleDir"), ""))));
    EXPECT_NO_THROW(vm.insert(std::make_pair("granularity", variable_value(String("class"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_NO_THROW(coverageMatrix = plugin->read(vm));

    EXPECT_EQ(1u, coverageMatrix->getNumOfTestcases());
    EXPECT_EQ(182u, coverageMatrix->getNumOfCodeElements());

    EXPECT_FALSE(coverageMatrix->getRelation("testWasRun(junit.tests.framework.TestCaseTest).testWasRun on testWasRun(junit.tests.framework.TestCaseTest)", "org/junit/internal/ArrayComparisonFailure"));
    EXPECT_TRUE(coverageMatrix->getRelation("testWasRun(junit.tests.framework.TestCaseTest).testWasRun on testWasRun(junit.tests.framework.TestCaseTest)", "org/junit/runner/Description"));
}

TEST_F(CoverageReaderPluginsTest, JacocoJavaCoverageReaderPluginMethod)
{
    EXPECT_NO_THROW(plugin = kernel.getCoverageReaderPluginManager().getPlugin("jacoco-java"));
    EXPECT_NO_THROW(vm.insert(std::make_pair("path", variable_value(String("sample/JacocoCoverageSampleDir"), ""))));
    EXPECT_NO_THROW(vm.insert(std::make_pair("granularity", variable_value(String("method"), ""))));
    EXPECT_NO_THROW(notify(vm));

    EXPECT_NO_THROW(coverageMatrix = plugin->read(vm));

    EXPECT_EQ(1u, coverageMatrix->getNumOfTestcases());
    EXPECT_EQ(1043u, coverageMatrix->getNumOfCodeElements());

    EXPECT_FALSE(coverageMatrix->getRelation("testWasRun(junit.tests.framework.TestCaseTest).testWasRun on testWasRun(junit.tests.framework.TestCaseTest)", "org/junit/runner/Description/createSuiteDescription(Ljava/lang/String,[Ljava/lang/annotation/Annotation,)Lorg/junit/runner/Description,"));
    EXPECT_TRUE(coverageMatrix->getRelation("testWasRun(junit.tests.framework.TestCaseTest).testWasRun on testWasRun(junit.tests.framework.TestCaseTest)", "org/junit/runner/Description/createTestDescription(Ljava/lang/Class,Ljava/lang/String,)Lorg/junit/runner/Description,"));
}


