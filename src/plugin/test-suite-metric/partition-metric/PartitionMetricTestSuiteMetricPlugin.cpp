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

void PartitionMetricTestSuiteMetricPlugin::calculate(rapidjson::Document &results)
{
    /*
    std::ofstream partitionMetricStream;
    partitionMetricStream.open((output + "/partition.metric.csv").c_str());
    partitionMetricStream << "# cluster id;number of testcases in cluster;number of code elements in cluster;partition-metric" << std::endl;
    */

    std::map<std::string, CClusterDefinition>::iterator it;
    for (it = m_clusterList->begin(); it != m_clusterList->end(); it++) {

        if (!results.HasMember(it->first.c_str())) {
            rapidjson::Value key;
            key.SetString(it->first.c_str(), results.GetAllocator());
            rapidjson::Value cluster;
            cluster.SetObject();
            results.AddMember(key, cluster, results.GetAllocator());
        }

        double faultLocalization = results[it->first.c_str()]["fault-localization"].GetDouble();
        double partitionMetric = 1.0 - faultLocalization;

        //IndexType nrOfTestCases = it->second.getTestCases().size();
        //IndexType nrOfCodeElements = it->second.getCodeElements().size();

        //partitionMetricStream << it->first << ";" << nrOfTestCases << ";" << nrOfCodeElements << ";" << partitionMetric << std::endl;

        rapidjson::Value::MemberIterator metricIt = results[it->first.c_str()].FindMember("partition-metric");
        if (metricIt == results[it->first.c_str()].MemberEnd()) {
            rapidjson::Value v;
            v.SetDouble(partitionMetric);
            results[it->first.c_str()].AddMember("partition-metric", v, results.GetAllocator());
        } else
            metricIt->value.SetDouble(partitionMetric);
    }

    //partitionMetricStream.close();
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new PartitionMetricTestSuiteMetricPlugin());
}

} /* namespace soda */
