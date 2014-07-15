/*
 *
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

#include <iostream>

#include <fstream>
#include <sstream>

#include "boost/filesystem.hpp"

#include "FaultLocalizationMetricPlugin.h"

namespace soda {

FaultLocalizationMetricPlugin::FaultLocalizationMetricPlugin() :
    m_data(NULL),
    m_clusterList(NULL),
    m_revision(0)
{

}

FaultLocalizationMetricPlugin::~FaultLocalizationMetricPlugin()
{

}

std::string FaultLocalizationMetricPlugin::getName()
{
    return "fault-localization";
}

std::string FaultLocalizationMetricPlugin::getDescription()
{
    return "Calculating fault localization metric and other statistics of the test suite.";
}

void FaultLocalizationMetricPlugin::init(CSelectionData *data, std::map<std::string, CClusterDefinition> *clusterList, IndexType revision)
{
    m_data = data;
    m_clusterList = clusterList;
    m_revision = revision;
}

std::vector<std::string> FaultLocalizationMetricPlugin::getDependency()
{
    return std::vector<std::string>();
}

void FaultLocalizationMetricPlugin::calculate(rapidjson::Document &results)
{
    std::map<std::string, CClusterDefinition>::iterator it;
    for (it = m_clusterList->begin(); it != m_clusterList->end(); it++) {

        if (!results.HasMember(it->first.c_str())) {
            rapidjson::Value key;
            key.SetString(it->first.c_str(), results.GetAllocator());
            rapidjson::Value cluster;
            cluster.SetObject();
            results.AddMember(key, cluster, results.GetAllocator());
        }

        CPartitionAlgorithm algorithm;
        algorithm.compute(*m_data, it->second, m_revision);

        // Prepare directory for the output.
        //std::stringstream ss;
        //ss << output << "/" << it->first;
        //boost::filesystem::path dir(ss.str().c_str());
        //boost::filesystem::create_directory(dir);

        //writePartitions(algorithm, ss.str());

        std::cerr << "[INFO] Calculating statisitcs: " << it->first << std::endl;
        partitionStatistics(algorithm, it->second, it->first, results);
        std::cerr << "[INFO] Calculating statisitcs: " << it->first << " DONE." << std::endl;
    }

}

void FaultLocalizationMetricPlugin::writePartitions(CPartitionAlgorithm &algorithm, rapidjson::Document &results)
{
    /*std::ofstream out;

    out.open((output + "/partitions.csv").c_str());

    CPartitionAlgorithm::PartitionInfo &partitionInfo = algorithm.getPartitionInfo();
    CPartitionAlgorithm::PartitionData &partitions = algorithm.getPartitions();

    for (CPartitionAlgorithm::PartitionInfo::iterator it = partitionInfo.begin(); it != partitionInfo.end(); it++) {
        if ((*it).partitionId) {
            out << (*it).cid << "|";
            for (std::set<IndexType>::iterator it2 = partitions[(*it).partitionId].begin(); it2 != partitions[(*it).partitionId].end(); it2++) {
                out << *it2 << ";";
            }
            out << std::endl;
        }
    }

    out.close();*/
}

void FaultLocalizationMetricPlugin::partitionStatistics(CPartitionAlgorithm &algorithm, CClusterDefinition &cluster, const std::string& clusterId, rapidjson::Document &result)
{
    /*
    std::ofstream partitionStatistics;
    std::ofstream partitionDistribution;

    partitionStatistics.open((output + "/partitions.statistics.csv").c_str());
    partitionDistribution.open((output + "/partitions.distribution.csv").c_str());

    partitionStatistics << "#revision;number of testcases;number of methods; number of classes;minimum size;maximum size; AVG size; FL (absolute); FL (normalized)" << std::endl;
    partitionDistribution  << "#revision; size of partition; number of partitions" << std::endl;
    */

    CPartitionAlgorithm::PartitionInfo &partitionInfo = algorithm.getPartitionInfo();
    CPartitionAlgorithm::PartitionData &partitions = algorithm.getPartitions();

    IndexType nrOfPartitions = partitions.size();
    IndexType nrOfCodeElementsInPartition = partitionInfo.size();
    //IndexType nrOfTestcases = cluster.getTestCases().size();
    IndexType minSize = nrOfCodeElementsInPartition;
    IndexType maxSize = 0;

    double avgSize = 0.0;
    double flMetric = 0;

    std::map<IndexType, IndexType> distribution;

    for (CPartitionAlgorithm::PartitionData::iterator partIt = partitions.begin(); partIt != partitions.end(); partIt++) {
        IndexType size = partIt->second.size();
        distribution[size]++;
        if (size < minSize) {
            minSize = size;
        }
        if (size > maxSize) {
            maxSize = size;
        }
        avgSize += size;

        flMetric += size * (size - 1);
    }
    avgSize /= nrOfPartitions;

    //double flMetricAbs = flMetric;
    flMetric /= nrOfCodeElementsInPartition * (nrOfCodeElementsInPartition - 1);

    rapidjson::Value v;
    v.SetDouble(flMetric);
    result[clusterId.c_str()].AddMember("fault-localization", v, result.GetAllocator());

    //partitionStatistics << m_revision << ";" << nrOfTestcases << ";" << nrOfCodeElementsInPartition << ";" << nrOfPartitions << ";" << minSize << ";" << maxSize << ";" << avgSize << ";" << flMetricAbs << ";" << flMetric << std::endl;

    /*for (std::map<IndexType, IndexType>::iterator it = distribution.begin(); it != distribution.end(); it++) {
        partitionDistribution << m_revision << ";" << it->first << ";" <<  it->second << std::endl;
    }*/

    //partitionStatistics.close();
    //partitionDistribution.close();
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new FaultLocalizationMetricPlugin());
}

} /* namespace soda */
