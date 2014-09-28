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

#include "PartitionEfficiencyTestSuiteMetricPlugin.h"

namespace soda {

PartitionEfficiencyTestSuiteMetricPlugin::PartitionEfficiencyTestSuiteMetricPlugin() :
    m_data(NULL),
    m_clusterList(NULL)
{
}

PartitionEfficiencyTestSuiteMetricPlugin::~PartitionEfficiencyTestSuiteMetricPlugin()
{
}

std::string PartitionEfficiencyTestSuiteMetricPlugin::getName()
{
    return "partition-efficiency";
}

std::string PartitionEfficiencyTestSuiteMetricPlugin::getDescription()
{
    return "The partition efficiency of a test suite.";
}

void PartitionEfficiencyTestSuiteMetricPlugin::init(CSelectionData *data, std::map<std::string, CClusterDefinition> *clusterList, IndexType revision)
{
    m_data = data;
    m_clusterList = clusterList;
}

std::vector<std::string> PartitionEfficiencyTestSuiteMetricPlugin::getDependency()
{
    std::vector<std::string> dependencies;
    dependencies.push_back("partition-metric");
    return dependencies;
}

void PartitionEfficiencyTestSuiteMetricPlugin::calculate(rapidjson::Document &results)
{
    /*
    std::ofstream out;
    out.open((output + "/effpart.metric.csv").c_str());
    out << "# cluster id;number of testcases in cluster;number of code elements in cluster;partition-metric;partition-efficiency" << std::endl;
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

        IndexType nrOfTestCases = it->second.getTestCases().size();
        //IndexType nrOfCodeElements = (*m_clusterList)[it->first].getCodeElements().size();

        double partitionMetric = results[it->first.c_str()]["partition-metric"].GetDouble();
        double partitionEfficiency = partitionMetric / nrOfTestCases;

        //out << it->first << ";" << nrOfTestCases << ";" << nrOfCodeElements << ";" << partitionMetric << ";" << partitionEfficiency << std::endl;

        rapidjson::Value::MemberIterator metricIt = results[it->first.c_str()].FindMember("partition-efficiency");
        if (metricIt == results[it->first.c_str()].MemberEnd()) {
            rapidjson::Value v;
            v.SetDouble(partitionEfficiency);
            results[it->first.c_str()].AddMember("partition-efficiency", v, results.GetAllocator());
        } else
            metricIt->value.SetDouble(partitionEfficiency);
    }

    //out.close();
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new PartitionEfficiencyTestSuiteMetricPlugin());
}

} /* namespace soda */
