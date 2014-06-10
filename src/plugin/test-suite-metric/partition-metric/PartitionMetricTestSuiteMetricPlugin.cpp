/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Tengeri <dtengeri@inf.u-szeged.hu>
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

#include "PartitionMetricTestSuiteMetricPlugin.h"

namespace soda {

PartitionMetricTestSuiteMetricPlugin::PartitionMetricTestSuiteMetricPlugin() :
    m_data(NULL),
    m_clusterList(NULL)
{
}

PartitionMetricTestSuiteMetricPlugin::~PartitionMetricTestSuiteMetricPlugin()
{
}

std::string PartitionMetricTestSuiteMetricPlugin::getName()
{
    return "partition-metric";
}

std::string PartitionMetricTestSuiteMetricPlugin::getDescription()
{
    return "Calculates the partition metroc of the test suite.";
}

void PartitionMetricTestSuiteMetricPlugin::init(CSelectionData *data, std::map<std::string, CClusterDefinition> *clusterList, IndexType revision)
{
    m_data = data;
    m_clusterList = clusterList;
}

std::vector<std::string> PartitionMetricTestSuiteMetricPlugin::getDependency()
{
    std::vector<std::string> dependencies;
    dependencies.push_back("fault-localization");
    return dependencies;
}

void PartitionMetricTestSuiteMetricPlugin::calculate(const std::string &output, std::map<std::string, MetricResults> &results)
{
    std::ofstream partitionMetricStream;
    partitionMetricStream.open((output + "/partition.metric.csv").c_str());
    partitionMetricStream << "# cluster id;number of testcases in cluster;number of code elements in cluster;partition-metric" << std::endl;

    std::map<std::string, MetricResults>::iterator it;
    for (it = results.begin(); it != results.end(); it++) {
        MetricResults result = it->second;
        double faultLocalization = result["fault-localization"];
        double partitionMetric = 1.0 - faultLocalization;

        IndexType nrOfTestCases = (*m_clusterList)[it->first].getTestCases().size();
        IndexType nrOfCodeElements = (*m_clusterList)[it->first].getCodeElements().size();

        partitionMetricStream << it->first << ";" << nrOfTestCases << ";" << nrOfCodeElements << ";" << partitionMetric << std::endl;

        results[it->first]["partition-metric"] = partitionMetric;
    }

    partitionMetricStream.close();
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new PartitionMetricTestSuiteMetricPlugin());
}

} /* namespace soda */
