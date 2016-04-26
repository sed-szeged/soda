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
    ClusterMap::iterator it;
    for (it = m_clusterList->begin(); it != m_clusterList->end(); it++) {

        if (!results.HasMember(it->first.c_str())) {
            results.AddMember(rapidjson::Value(it->first.c_str(), results.GetAllocator()), rapidjson::Value(rapidjson::kObjectType), results.GetAllocator());
        }

        CPartitionAlgorithm algorithm;
        algorithm.compute(*m_data, it->second);

        // Prepare directory for the output.
        //std::stringstream ss;
        //ss << output << "/" << it->first;
        //boost::filesystem::path dir(ss.str().c_str());
        //boost::filesystem::create_directory(dir);

        std::cerr << "[INFO] Calculating statisitcs: " << it->first << std::endl;
        partitionStatistics(algorithm, it->second, it->first, results);
        partitionStatisticsFiltered(algorithm, it->second, it->first, results);
        std::cerr << "[INFO] Calculating statisitcs: " << it->first << " DONE." << std::endl;
    }

}

void FaultLocalizationMetricPlugin::partitionStatistics(CPartitionAlgorithm &algorithm, CClusterDefinition &cluster, const std::string& clusterId, rapidjson::Document &result)
{
    CPartitionAlgorithm::PartitionInfo &partitionInfo = algorithm.getPartitionInfo();
    CPartitionAlgorithm::PartitionData &partitions = algorithm.getPartitions();

    IndexType nrOfPartitions = partitions.size();
    IndexType nrOfCodeElementsInPartition = partitionInfo.size();
    IndexType nrOfTestcases = cluster.getTestCases().size();
    IndexType minSize = nrOfCodeElementsInPartition;
    IndexType maxSize = 0;

    double avgSize = 0.0;
    double flMetric = 0;
    double area = 0;
    //std::map<IndexType, IndexType> distribution;

    for (CPartitionAlgorithm::PartitionData::iterator partIt = partitions.begin(); partIt != partitions.end(); partIt++) {
        IndexType size = partIt->second.size();
        //distribution[size]++;
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

    rapidjson::Value partDataArray(rapidjson::kArrayType);
    for (auto &partition : partitions) {
        rapidjson::Value partData(rapidjson::kObjectType);
        IndexType nrOfCoveredTests = getPartitionCoverage(partition.second, cluster.getTestCases());
        IndexType partSize = partition.second.size();

        area += nrOfCoveredTests * partSize;

        partData.AddMember("partition-cover", nrOfCoveredTests, result.GetAllocator());
        partData.AddMember("partition-size", partSize, result.GetAllocator());
        rapidjson::Value ceIds(rapidjson::kArrayType);
        for (auto ceIdx : partition.second) {
            ceIds.PushBack(ceIdx, result.GetAllocator());
        }
        partData.AddMember("partition-elements", ceIds, result.GetAllocator());
        partDataArray.PushBack(partData, result.GetAllocator());
    }

    result[clusterId.c_str()].AddMember("partition-data", partDataArray, result.GetAllocator());

    area /= nrOfCodeElementsInPartition * nrOfTestcases;

    if(nrOfCodeElementsInPartition == 0){
        flMetric = 0;
    } else if(nrOfCodeElementsInPartition == 1){
        flMetric /= nrOfCodeElementsInPartition * (nrOfCodeElementsInPartition);
    } else {
        flMetric /= nrOfCodeElementsInPartition * (nrOfCodeElementsInPartition - 1);
    }

    double regularity = 0;
    if (nrOfPartitions >= 1 && nrOfCodeElementsInPartition > 1) {
        regularity = double(nrOfPartitions - 1) / (nrOfCodeElementsInPartition - 1);
    }

    rapidjson::Value::MemberIterator metricIt = result[clusterId.c_str()].FindMember("fault-localization");
    if (metricIt == result[clusterId.c_str()].MemberEnd()) {
        result[clusterId.c_str()].AddMember("fault-localization", flMetric, result.GetAllocator());
    } else
        metricIt->value.SetDouble(flMetric);

    rapidjson::Value partitionStatObject(rapidjson::kObjectType);
    partitionStatObject.AddMember("CP_K", nrOfPartitions, result.GetAllocator());
    partitionStatObject.AddMember("CP_Min", minSize, result.GetAllocator());
    partitionStatObject.AddMember("CP_Max", maxSize, result.GetAllocator());
    partitionStatObject.AddMember("CP_Avg", avgSize, result.GetAllocator());
    partitionStatObject.AddMember("CP_AvgP", 1.0 / nrOfPartitions, result.GetAllocator());
    partitionStatObject.AddMember("CP_REG", regularity, result.GetAllocator());
    partitionStatObject.AddMember("CP_AREA", area, result.GetAllocator());
    result[clusterId.c_str()].AddMember("partition-statistics", partitionStatObject, result.GetAllocator());
}

void FaultLocalizationMetricPlugin::partitionStatisticsFiltered(CPartitionAlgorithm &algorithm, CClusterDefinition &cluster, const std::string& clusterId, rapidjson::Document &result)
{

    CPartitionAlgorithm::PartitionInfo &partitionInfo = algorithm.getPartitionInfo();
    CPartitionAlgorithm::PartitionData &partitions = algorithm.getPartitions();


    IndexType nrOfCodeElementsInPartition = partitionInfo.size();
    IndexType nrOfTestcases = cluster.getTestCases().size();
    IndexType minSize = nrOfCodeElementsInPartition;
    IndexType maxSize = 0;

    double avgSize = 0.0;
    double flMetric = 0;
    double area = 0;
    //std::map<IndexType, IndexType> distribution;

    // TODO Merge these for loops into one
    for (auto &partition : partitions) {
        IndexType nrOfCoveredTests = getPartitionCoverage(partition.second, cluster.getTestCases());
        if (nrOfCoveredTests == 0) {
            nrOfCodeElementsInPartition -= partition.second.size();
            partitions.erase(partition.first);

            break;
        }
    }

    IndexType nrOfPartitions = partitions.size();

    for (CPartitionAlgorithm::PartitionData::iterator partIt = partitions.begin(); partIt != partitions.end(); partIt++) {
        IndexType size = partIt->second.size();
        //distribution[size]++;
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

    rapidjson::Value partDataArray(rapidjson::kArrayType);
    for (auto &partition : partitions) {
        rapidjson::Value partData(rapidjson::kObjectType);
        IndexType nrOfCoveredTests = getPartitionCoverage(partition.second, cluster.getTestCases());
        IndexType partSize = partition.second.size();

        area += nrOfCoveredTests * partSize;

        partData.AddMember("partition-cover'", nrOfCoveredTests, result.GetAllocator());
        partData.AddMember("partition-size'", partSize, result.GetAllocator());
        rapidjson::Value ceIds(rapidjson::kArrayType);
        for (auto ceIdx : partition.second) {
            ceIds.PushBack(ceIdx, result.GetAllocator());
        }
        partData.AddMember("partition-elements'", ceIds, result.GetAllocator());
        partDataArray.PushBack(partData, result.GetAllocator());
    }

    result[clusterId.c_str()].AddMember("partition-data'", partDataArray, result.GetAllocator());

    area /= nrOfCodeElementsInPartition * nrOfTestcases;

    if(nrOfCodeElementsInPartition == 0){
        flMetric = 0;
    } else if(nrOfCodeElementsInPartition == 1){
        flMetric /= nrOfCodeElementsInPartition * (nrOfCodeElementsInPartition);
    } else {
        flMetric /= nrOfCodeElementsInPartition * (nrOfCodeElementsInPartition - 1);
    }

    double regularity = 0;
    if (nrOfPartitions >= 1 && nrOfCodeElementsInPartition > 1) {
        regularity = double(nrOfPartitions - 1) / (nrOfCodeElementsInPartition - 1);
    }

    rapidjson::Value::MemberIterator metricIt = result[clusterId.c_str()].FindMember("fault-localization'");
    if (metricIt == result[clusterId.c_str()].MemberEnd()) {
        result[clusterId.c_str()].AddMember("fault-localization'", flMetric, result.GetAllocator());
    } else
        metricIt->value.SetDouble(flMetric);

    rapidjson::Value partitionStatObject(rapidjson::kObjectType);
    partitionStatObject.AddMember("CP_K'", nrOfPartitions, result.GetAllocator());
    partitionStatObject.AddMember("CP_Min'", minSize, result.GetAllocator());
    partitionStatObject.AddMember("CP_Max'", maxSize, result.GetAllocator());
    partitionStatObject.AddMember("CP_Avg'", avgSize, result.GetAllocator());
    partitionStatObject.AddMember("CP_AvgP'", 1.0 / nrOfPartitions, result.GetAllocator());
    partitionStatObject.AddMember("CP_REG'", regularity, result.GetAllocator());
    // TODO update area
    partitionStatObject.AddMember("CP_AREA'", area, result.GetAllocator());
    result[clusterId.c_str()].AddMember("partition-statistics'", partitionStatObject, result.GetAllocator());

}



IndexType FaultLocalizationMetricPlugin::getPartitionCoverage(std::set<IndexType> const &codeElements, IntVector const &testCases) {
    IndexType count = 0;
    for (auto tcIdx : testCases) {
        auto &row = m_data->getCoverage()->getBitMatrix().getRow(tcIdx);
        if (row.count() < codeElements.size()) {
            continue;
        }
        bool covered = true;
        for (auto ce : codeElements) {
            if (!row.at(ce)) {
                covered = false;
                break;
            }
        }
        if (covered) {
            ++count;
        }
    }
    return count;
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new FaultLocalizationMetricPlugin());
}

} /* namespace soda */
