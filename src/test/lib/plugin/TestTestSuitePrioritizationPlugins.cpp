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

#include "gtest/gtest.h"
#include "engine/CKernel.h"
#include "engine/plugin/ITestSuitePrioritizationPlugin.h"
#include "data/CSelectionData.h"

using namespace soda;

class TestSuitePrioritizationPluginsTest : public testing::Test
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

TEST_F(TestSuitePrioritizationPluginsTest, GeneralIgnorePrioritizationPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("general-ignore"));

    EXPECT_EQ("general-ignore", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(TestSuitePrioritizationPluginsTest, GeneralIgnorePrioritizationPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("general-ignore"));
    EXPECT_NO_THROW(plugin->reset(1));
    EXPECT_NO_THROW(plugin->init(data, &kernel));
    EXPECT_NO_THROW(plugin->fillSelection(result, 100));

    EXPECT_EQ(3u, result[0]);
    EXPECT_EQ(4u, result[1]);
    EXPECT_EQ(2u, result[2]);
    EXPECT_EQ(1u, result[3]);
    EXPECT_EQ(0u, result[4]);
}

TEST_F(TestSuitePrioritizationPluginsTest, AdditionalGeneralIgnorePrioritizationPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("additional-general-ignore"));

    EXPECT_EQ("additional-general-ignore", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(TestSuitePrioritizationPluginsTest, AdditionalGeneralIgnorePrioritizationPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("additional-general-ignore"));
    EXPECT_NO_THROW(plugin->reset(1));
    EXPECT_NO_THROW(plugin->init(data, &kernel));
    EXPECT_NO_THROW(plugin->fillSelection(result, 100));

    EXPECT_EQ(3u, result[0]);
    EXPECT_EQ(1u, result[1]);
    EXPECT_EQ(4u, result[2]);
    EXPECT_EQ(0u, result[3]);
    EXPECT_EQ(2u, result[4]);
}

TEST_F(TestSuitePrioritizationPluginsTest, AdditionalWithResetsPrioritizationPlugin)
{
    CSelectionData csdata;
    csdata.loadCoverage("sample/prioritization.reset.cov.SoDA");

    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("additional-with-resets"));
    EXPECT_NO_THROW(plugin->init(&csdata, &kernel));
    EXPECT_NO_THROW(plugin->fillSelection(result, csdata.getCoverage()->getNumOfTestcases()));

    EXPECT_EQ(0u, result[0]);
    EXPECT_EQ(1u, result[1]);
    EXPECT_EQ(4u, result[2]);
    EXPECT_EQ(2u, result[3]);
    EXPECT_EQ(3u, result[4]);
    EXPECT_EQ(5u, result[5]);
}

TEST_F(TestSuitePrioritizationPluginsTest, RandomIgnorePrioritizationPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("random-ignore"));

    EXPECT_EQ("random-ignore", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(TestSuitePrioritizationPluginsTest, RandomIgnorePrioritizationPlugin)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("random-ignore"));
    EXPECT_NO_THROW(plugin->reset(1));
    EXPECT_NO_THROW(plugin->init(data, &kernel));
    EXPECT_NO_THROW(plugin->fillSelection(result, 100));
    EXPECT_EQ(5u, result.size());
}

TEST_F(TestSuitePrioritizationPluginsTest, RaptorPrioritizationPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("raptor"));

    EXPECT_EQ("raptor", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(TestSuitePrioritizationPluginsTest, RaptorPrioritizationPluginNext)
{
    CSelectionData selectionData;
    selectionData.loadCoverage("sample/raptor.cov.SoDA");

    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("raptor"));
    EXPECT_NO_THROW(plugin->init(&selectionData, &kernel));
    EXPECT_EQ(plugin->next(), 4);
    EXPECT_EQ(plugin->next(), 7);
    EXPECT_EQ(plugin->next(), 5);
    EXPECT_EQ(plugin->next(), 1);
    EXPECT_EQ(plugin->next(), 6);
    EXPECT_EQ(plugin->next(), 3);
    EXPECT_EQ(plugin->next(), 2);
    EXPECT_EQ(plugin->next(), 0);

}

TEST_F(TestSuitePrioritizationPluginsTest, RaptorPrioritizationPluginFillSelection)
{
    CSelectionData selectionData;
    selectionData.loadCoverage("sample/raptor.cov.SoDA");

    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("raptor"));
    EXPECT_NO_THROW(plugin->init(&selectionData, &kernel));
    EXPECT_NO_THROW(plugin->fillSelection(result, 100));
    EXPECT_EQ(8u, result.size());
    EXPECT_EQ(4u, result[0]);
    EXPECT_EQ(7u, result[1]);
    EXPECT_EQ(5u, result[2]);
    EXPECT_EQ(1u, result[3]);
    EXPECT_EQ(6u, result[4]);
    EXPECT_EQ(3u, result[5]);
    EXPECT_EQ(2u, result[6]);
    EXPECT_EQ(0u, result[7]);
}

TEST_F(TestSuitePrioritizationPluginsTest, DuplationPrioritizationPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("duplation"));

    EXPECT_EQ("duplation", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(TestSuitePrioritizationPluginsTest, DuplationPrioritizationPluginNext)
{
    CSelectionData selectionData;
    // Create selection data.
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            selectionData.getCoverage()->addOrSetRelation("test-" + boost::lexical_cast<String>(i), "ce-" + boost::lexical_cast<String>(j), (j <= i) ? true : false);
        }
    }
    selectionData.getCoverage()->setRelation(98,99,true);

    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("duplation"));
    EXPECT_NO_THROW(plugin->init(&selectionData, &kernel));

    EXPECT_EQ(plugin->next(), 49);
    EXPECT_EQ(plugin->next(), 74);
    EXPECT_EQ(plugin->next(), 24);
    EXPECT_EQ(plugin->next(), 86);
}

TEST_F(TestSuitePrioritizationPluginsTest, DuplationPrioritizationPluginNext2)
{
    CSelectionData selectionData;
    selectionData.getCoverage()->addTestcaseName("t1");
    selectionData.getCoverage()->addTestcaseName("t2");
    selectionData.getCoverage()->addTestcaseName("t3");
    selectionData.getCoverage()->addTestcaseName("t4");

    selectionData.getCoverage()->addCodeElementName("c1");
    selectionData.getCoverage()->addCodeElementName("c2");
    selectionData.getCoverage()->addCodeElementName("c3");
    selectionData.getCoverage()->addCodeElementName("c4");
    selectionData.getCoverage()->addCodeElementName("c5");

    selectionData.getCoverage()->refitMatrixSize();

    selectionData.getCoverage()->addOrSetRelation("t1", "c1", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c3", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c4", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c5", true);
    selectionData.getCoverage()->addOrSetRelation("t2", "c2", true);
    selectionData.getCoverage()->addOrSetRelation("t2", "c3", true);
    selectionData.getCoverage()->addOrSetRelation("t3", "c1", true);
    selectionData.getCoverage()->addOrSetRelation("t3", "c3", true);
    selectionData.getCoverage()->addOrSetRelation("t3", "c5", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c1", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c4", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c5", true);


    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("duplation"));
    EXPECT_NO_THROW(plugin->init(&selectionData, &kernel));

    EXPECT_EQ(plugin->next(), 1);
    EXPECT_EQ(plugin->next(), 2);
    EXPECT_EQ(plugin->next(), 0);
    EXPECT_EQ(plugin->next(), 3);
}

TEST_F(TestSuitePrioritizationPluginsTest, DuplationPrioritizationPluginNext3)
{
    CSelectionData selectionData;
    selectionData.getCoverage()->addTestcaseName("t1");
    selectionData.getCoverage()->addTestcaseName("t2");
    selectionData.getCoverage()->addTestcaseName("t3");
    selectionData.getCoverage()->addTestcaseName("t4");

    selectionData.getCoverage()->addCodeElementName("c1");
    selectionData.getCoverage()->addCodeElementName("c2");
    selectionData.getCoverage()->addCodeElementName("c3");
    selectionData.getCoverage()->addCodeElementName("c4");
    selectionData.getCoverage()->addCodeElementName("c5");
    selectionData.getCoverage()->addCodeElementName("c6");
    selectionData.getCoverage()->addCodeElementName("c7");
    selectionData.getCoverage()->addCodeElementName("c8");

    selectionData.getCoverage()->refitMatrixSize();

    selectionData.getCoverage()->addOrSetRelation("t1", "c1", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c2", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c3", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c4", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c5", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c6", true);

    selectionData.getCoverage()->addOrSetRelation("t2", "c1", true);
    selectionData.getCoverage()->addOrSetRelation("t2", "c2", true);
    selectionData.getCoverage()->addOrSetRelation("t2", "c3", true);
    selectionData.getCoverage()->addOrSetRelation("t2", "c4", true);
    selectionData.getCoverage()->addOrSetRelation("t2", "c7", true);
    selectionData.getCoverage()->addOrSetRelation("t2", "c8", true);

    selectionData.getCoverage()->addOrSetRelation("t3", "c1", true);
    selectionData.getCoverage()->addOrSetRelation("t3", "c2", true);
    selectionData.getCoverage()->addOrSetRelation("t3", "c3", true);
    selectionData.getCoverage()->addOrSetRelation("t3", "c4", true);
    selectionData.getCoverage()->addOrSetRelation("t3", "c8", true);

    selectionData.getCoverage()->addOrSetRelation("t4", "c1", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c2", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c3", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c4", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c5", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c6", true);



    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("duplation"));
    EXPECT_NO_THROW(plugin->init(&selectionData, &kernel));

    EXPECT_EQ(plugin->next(), 2);
    EXPECT_EQ(plugin->next(), 0);
    EXPECT_EQ(plugin->next(), 3);
    EXPECT_EQ(plugin->next(), 1);
}

TEST_F(TestSuitePrioritizationPluginsTest, DuplationPrimePrioritizationPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("duplation-prime"));

    EXPECT_EQ("duplation-prime", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(TestSuitePrioritizationPluginsTest, DuplationPrimePrioritizationPluginNext)
{
    CSelectionData selectionData;
    // Create selection data.
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            selectionData.getCoverage()->addOrSetRelation("test-" + boost::lexical_cast<String>(i), "ce-" + boost::lexical_cast<String>(j), (j <= i) ? true : false);
        }
    }
    selectionData.getCoverage()->setRelation(98,99,true);

    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("duplation-prime"));
    EXPECT_NO_THROW(plugin->init(&selectionData, &kernel));

    EXPECT_EQ(plugin->next(), 49);
    EXPECT_EQ(plugin->next(), 74);
    EXPECT_EQ(plugin->next(), 24);
    EXPECT_EQ(plugin->next(), 86);
}

TEST_F(TestSuitePrioritizationPluginsTest, DuplationPrimePrioritizationPluginNext2)
{
    CSelectionData selectionData;
    selectionData.getCoverage()->addTestcaseName("t1");
    selectionData.getCoverage()->addTestcaseName("t2");
    selectionData.getCoverage()->addTestcaseName("t3");
    selectionData.getCoverage()->addTestcaseName("t4");

    selectionData.getCoverage()->addCodeElementName("c1");
    selectionData.getCoverage()->addCodeElementName("c2");
    selectionData.getCoverage()->addCodeElementName("c3");
    selectionData.getCoverage()->addCodeElementName("c4");
    selectionData.getCoverage()->addCodeElementName("c5");

    selectionData.getCoverage()->refitMatrixSize();

    selectionData.getCoverage()->addOrSetRelation("t1", "c1", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c3", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c4", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c5", true);
    selectionData.getCoverage()->addOrSetRelation("t2", "c2", true);
    selectionData.getCoverage()->addOrSetRelation("t2", "c3", true);
    selectionData.getCoverage()->addOrSetRelation("t3", "c1", true);
    selectionData.getCoverage()->addOrSetRelation("t3", "c3", true);
    selectionData.getCoverage()->addOrSetRelation("t3", "c5", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c1", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c4", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c5", true);


    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("duplation-prime"));
    EXPECT_NO_THROW(plugin->init(&selectionData, &kernel));

    EXPECT_EQ(plugin->next(), 1);
    EXPECT_EQ(plugin->next(), 2);
    EXPECT_EQ(plugin->next(), 0);
    EXPECT_EQ(plugin->next(), 3);
}

TEST_F(TestSuitePrioritizationPluginsTest, DuplationPrimePrioritizationPluginNext3)
{
    CSelectionData selectionData;
    selectionData.getCoverage()->addTestcaseName("t1");
    selectionData.getCoverage()->addTestcaseName("t2");
    selectionData.getCoverage()->addTestcaseName("t3");
    selectionData.getCoverage()->addTestcaseName("t4");

    selectionData.getCoverage()->addCodeElementName("c1");
    selectionData.getCoverage()->addCodeElementName("c2");
    selectionData.getCoverage()->addCodeElementName("c3");
    selectionData.getCoverage()->addCodeElementName("c4");
    selectionData.getCoverage()->addCodeElementName("c5");
    selectionData.getCoverage()->addCodeElementName("c6");
    selectionData.getCoverage()->addCodeElementName("c7");
    selectionData.getCoverage()->addCodeElementName("c8");

    selectionData.getCoverage()->refitMatrixSize();

    selectionData.getCoverage()->addOrSetRelation("t1", "c1", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c2", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c3", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c4", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c5", true);
    selectionData.getCoverage()->addOrSetRelation("t1", "c6", true);

    selectionData.getCoverage()->addOrSetRelation("t2", "c1", true);
    selectionData.getCoverage()->addOrSetRelation("t2", "c2", true);
    selectionData.getCoverage()->addOrSetRelation("t2", "c3", true);
    selectionData.getCoverage()->addOrSetRelation("t2", "c4", true);
    selectionData.getCoverage()->addOrSetRelation("t2", "c7", true);
    selectionData.getCoverage()->addOrSetRelation("t2", "c8", true);

    selectionData.getCoverage()->addOrSetRelation("t3", "c1", true);
    selectionData.getCoverage()->addOrSetRelation("t3", "c2", true);
    selectionData.getCoverage()->addOrSetRelation("t3", "c3", true);
    selectionData.getCoverage()->addOrSetRelation("t3", "c4", true);
    selectionData.getCoverage()->addOrSetRelation("t3", "c8", true);

    selectionData.getCoverage()->addOrSetRelation("t4", "c1", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c2", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c3", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c4", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c5", true);
    selectionData.getCoverage()->addOrSetRelation("t4", "c6", true);



    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("duplation-prime"));
    EXPECT_NO_THROW(plugin->init(&selectionData, &kernel));

    EXPECT_EQ(plugin->next(), 2);
    EXPECT_EQ(plugin->next(), 0);
    EXPECT_EQ(plugin->next(), 1);
    EXPECT_EQ(plugin->next(), 3);
}

TEST_F(TestSuitePrioritizationPluginsTest, PartitionMetricPrioritizationPluginMetaInfo)
{
    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("partition-metric"));

    EXPECT_EQ("partition-metric", plugin->getName());
    EXPECT_TRUE(plugin->getDescription().length() > 0);
}

TEST_F(TestSuitePrioritizationPluginsTest, PartitionMetricPrioritizationPluginNext)
{
    CSelectionData selectionData;
    // Create selection data.
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            selectionData.getCoverage()->addOrSetRelation("test-" + boost::lexical_cast<String>(i), "ce-" + boost::lexical_cast<String>(j), (j <= i && j % 2 == 0) ? true : false);
        }
    }
    selectionData.getCoverage()->setRelation(98,99,true);

    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("partition-metric"));
    EXPECT_NO_THROW(plugin->init(&selectionData, &kernel));

    EXPECT_EQ(plugin->next(), 99);
    EXPECT_EQ(plugin->next(), 49);
    EXPECT_EQ(plugin->next(), 75);
    EXPECT_EQ(plugin->next(), 22);
}

TEST_F(TestSuitePrioritizationPluginsTest, PartitionWithResetsPrioritizationPluginNext)
{
    CSelectionData selectionData;
    // Create selection data.
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            selectionData.getCoverage()->addOrSetRelation("test-" + boost::lexical_cast<String>(i), "ce-" + boost::lexical_cast<String>(j), (j <= i && j % 2 == 0) ? true : false);
        }
    }
    selectionData.getCoverage()->setRelation(98,99,true);

    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("partition-with-resets"));
    EXPECT_NO_THROW(plugin->init(&selectionData, &kernel));

    EXPECT_EQ(plugin->next(), 99);
    EXPECT_EQ(plugin->next(), 49);
    EXPECT_EQ(plugin->next(), 75);
    EXPECT_EQ(plugin->next(), 22);
    plugin->fillSelection(result, 60);
    EXPECT_EQ(plugin->next(), 29);
}

/*TEST_F(TestSuitePrioritizationPluginsTest, DuplationPrioritizationPluginFillSelection)
{
    CSelectionData selectionData;
    selectionData.loadCoverage("sample/raptor.cov.SoDA");

    EXPECT_NO_THROW(plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin("raptor"));
    EXPECT_NO_THROW(plugin->init(&selectionData, &kernel));
    EXPECT_NO_THROW(plugin->fillSelection(result, 100));
    EXPECT_EQ(8u, result.size());
    EXPECT_EQ(4u, result[0]);
    EXPECT_EQ(7u, result[1]);
    EXPECT_EQ(5u, result[2]);
    EXPECT_EQ(1u, result[3]);
    EXPECT_EQ(6u, result[4]);
    EXPECT_EQ(3u, result[5]);
    EXPECT_EQ(2u, result[6]);
    EXPECT_EQ(0u, result[7]);
}*/


