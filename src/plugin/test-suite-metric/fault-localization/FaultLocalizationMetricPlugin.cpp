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

void FaultLocalizationMetricPlugin::init(CSelectionData *data, std::vector<CClusterDefinition> *clusterList, IndexType revision)
{
    m_data = data;
    m_clusterList = clusterList;
    m_revision = revision;
}

std::vector<std::string> FaultLocalizationMetricPlugin::getDependency()
{
    return std::vector<std::string>();
}

void FaultLocalizationMetricPlugin::calculate(const std::string &output, std::vector<MetricResults> &results)
{
    for (IndexType i = 0; i < m_clusterList->size(); i++) {
        CPartitionAlgorithm algorithm;
        algorithm.compute(*m_data, (*m_clusterList)[i], m_revision);

        // Prepare directory for the output.
        std::stringstream ss;
        ss << output << "/" << i;
        boost::filesystem::path dir(ss.str().c_str());
        boost::filesystem::create_directory(dir);

        //writePartitions(algorithm, ss.str());

        std::cerr << "[INFO] Calculating statisitcs: " << i << std::endl;
        partitionStatistics(algorithm, (*m_clusterList)[i], ss.str(), results[i]);
        std::cerr << "[INFO] Calculating statisitcs: " << i << " DONE." << std::endl;
    }

}

void FaultLocalizationMetricPlugin::writePartitions(CPartitionAlgorithm &algorithm, const std::string &output)
{
    std::ofstream out;

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

    out.close();
}

void FaultLocalizationMetricPlugin::partitionStatistics(CPartitionAlgorithm &algorithm, CClusterDefinition &cluster, const std::string &output, MetricResults &result)
{
    std::ofstream partitionStatistics;
    std::ofstream partitionDistribution;

    partitionStatistics.open((output + "/partitions.statistics.csv").c_str());
    partitionDistribution.open((output + "/partitions.distribution.csv").c_str());

    partitionStatistics << "#revision;number of testcases;number of methods; number of classes;minimum size;maximum size; AVG size; FL (absolute); FL (normalized)" << std::endl;
    partitionDistribution  << "#revision; size of partition; number of partitions" << std::endl;

    CPartitionAlgorithm::PartitionInfo &partitionInfo = algorithm.getPartitionInfo();
    CPartitionAlgorithm::PartitionData &partitions = algorithm.getPartitions();

    IndexType nrOfPartitions = partitions.size();
    IndexType nrOfCodeElementsInPartition = partitionInfo.size();
    IndexType nrOfTestcases = cluster.getTestCases().size();
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

    double flMetricAbs = flMetric;
    flMetric /= nrOfCodeElementsInPartition * (nrOfCodeElementsInPartition - 1);

    result["fault-localization"] = flMetric;

    partitionStatistics << m_revision << ";" << nrOfTestcases << ";" << nrOfCodeElementsInPartition << ";" << nrOfPartitions << ";" << minSize << ";" << maxSize << ";" << avgSize << ";" << flMetricAbs << ";" << flMetric << std::endl;

    for (std::map<IndexType, IndexType>::iterator it = distribution.begin(); it != distribution.end(); it++) {
        partitionDistribution << m_revision << ";" << it->first << ";" <<  it->second << std::endl;
    }

    partitionStatistics.close();
    partitionDistribution.close();
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new FaultLocalizationMetricPlugin());
}

} /* namespace soda */
