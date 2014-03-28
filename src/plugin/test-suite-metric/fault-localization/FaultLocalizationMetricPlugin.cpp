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
#include <fstream>
#include <sstream>

#include "boost/filesystem.hpp"

#include "FaultLocalizationMetricPlugin.h"

namespace soda {

FaultLocalizationMetricPlugin::FaultLocalizationMetricPlugin()
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

void FaultLocalizationMetricPlugin::calculate(CSelectionData &data, std::vector<CClusterDefinition> &clusterList, const std::vector<IndexType> &revisionList, const std::string &output)
{
    for (IndexType i = 0; i < clusterList.size(); i++) {
        CPartitionAlgorithm algorithm;
        algorithm.compute(data, clusterList[i], revisionList);

        // Prepare directory for the output.
        std::stringstream ss;
        ss << output << "/" << i;
        boost::filesystem::path dir(ss.str().c_str());
        boost::filesystem::create_directory(dir);

        writePartitions(algorithm, ss.str());

        partitionStatistics(data, algorithm, ss.str());
    }

}

void FaultLocalizationMetricPlugin::writePartitions(CPartitionAlgorithm &algorithm, const std::string &output)
{
    std::ofstream out;

    out.open((output + "/partitions.csv").c_str());

    CPartitionAlgorithm::PartitionInfo &partitionInfo = algorithm.getPartitionInfo();
    CPartitionAlgorithm::Partition &partitions = algorithm.getPartitions();

    for (CPartitionAlgorithm::PartitionInfo::iterator it = partitionInfo.begin(); it != partitionInfo.end(); it++) {
        IndexType revision = it->first;
        out << "Revision: " << revision << std::endl;

        for (IndexType i = 0; i < it->second.size(); i++) {
            if ((it->second)[i].partitionId) {
                out << (it->second)[i].cid << "|";
                for (std::set<IndexType>::iterator it2 = partitions[revision][(it->second)[i].partitionId].begin(); it2 != partitions[revision][(it->second)[i].partitionId].end(); it2++) {
                    out << *it2 << ";";
                }
                out << std::endl;
            }
        }
    }

    out.close();
}

void FaultLocalizationMetricPlugin::partitionStatistics(CSelectionData &data, CPartitionAlgorithm &algorithm, const std::string &output)
{
    std::ofstream partitionStatistics;
    std::ofstream partitionGlobalStatistics;
    std::ofstream partitionDistribution;

    partitionStatistics.open((output + "/partitions.statistics.csv").c_str());
    partitionGlobalStatistics.open((output + "/partitions.global.statistics.csv").c_str());
    partitionDistribution.open((output + "/partitions.distribution.csv").c_str());

    partitionStatistics << "#revision;number of testcases;number of methods; number of classes;minimum size;maximum size; AVG size; FL (absolute); FL (normalized)" << std::endl;
    partitionGlobalStatistics << "#number of revisions;minimum class size;maximum class size; AVG size" << std::endl;
    partitionDistribution  << "#revision; size of partition; number of partitions" << std::endl;

    CPartitionAlgorithm::PartitionInfo &partitionInfo = algorithm.getPartitionInfo();
    CPartitionAlgorithm::Partition &partitions = algorithm.getPartitions();

    IndexType nrOfCodeElements = data.getCoverage()->getNumOfCodeElements();

    IndexType globalMinPartitionSize = nrOfCodeElements;
    IndexType globalMaxPartitionSize = 0;
    double globalAvgSize = 0.0;

    for (CPartitionAlgorithm::Partition::iterator revIt = partitions.begin(); revIt != partitions.end(); revIt++) {

        IndexType nrOfPartitions = revIt->second.size();
        IndexType nrOfCodeElementsInPartition = partitionInfo[revIt->first].size();
        IndexType nrOfTestcases = data.getResults()->getExecutionBitList(revIt->first).count();
        IndexType minSize = nrOfCodeElementsInPartition;
        IndexType maxSize = 0;

        double avgSize = 0.0;
        double flMetric = 0;

        std::map<IndexType, IndexType> distribution;

        for (CPartitionAlgorithm::PartitionData::iterator partIt = revIt->second.begin(); partIt != revIt->second.end(); partIt++) {
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

        if (minSize < globalMinPartitionSize) {
            globalMinPartitionSize = minSize;
        }
        if (maxSize > globalMaxPartitionSize) {
            globalMaxPartitionSize = maxSize;
        }
        globalAvgSize += avgSize;

        partitionStatistics << revIt->first << ";" << nrOfTestcases << ";" << nrOfCodeElementsInPartition << ";" << nrOfPartitions << ";" << minSize << ";" << maxSize << ";" << avgSize << ";" << flMetricAbs << ";" << flMetric << std::endl;

        for (std::map<IndexType, IndexType>::iterator it = distribution.begin(); it != distribution.end(); it++) {
            partitionDistribution << revIt->first << ";" << it->first << ";" <<  it->second << std::endl;
        }
    }

    globalAvgSize /= partitions.size();

    partitionGlobalStatistics << partitions.size() << ";" << globalMinPartitionSize << ";" << globalMaxPartitionSize << ";" << globalAvgSize << ";" << std::endl;

    partitionStatistics.close();
    partitionGlobalStatistics.close();
    partitionDistribution.close();
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new FaultLocalizationMetricPlugin());
}

} /* namespace soda */
