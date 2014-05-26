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

void PartitionEfficiencyTestSuiteMetricPlugin::calculate(const std::string &output, std::map<std::string, MetricResults> &results)
{
    std::ofstream out;
    out.open((output + "/effpart.metric.csv").c_str());
    out << "# cluster id;number of testcases in cluster;number of code elements in cluster;partition-metric;partition-efficiency" << std::endl;

    std::map<std::string, MetricResults>::iterator it;
    for (it = results.begin(); it != results.end(); it++) {
        MetricResults result = it->second;

        IndexType nrOfTestCases = (*m_clusterList)[it->first].getTestCases().size();
        IndexType nrOfCodeElements = (*m_clusterList)[it->first].getCodeElements().size();

        double partitionMetric = result["partition-metric"];
        double partitionEfficiency = partitionMetric / nrOfTestCases;

        out << it->first << ";" << nrOfTestCases << ";" << nrOfCodeElements << ";" << partitionMetric << ";" << partitionEfficiency << std::endl;

        results[it->first]["partition-efficiency"] = partitionEfficiency;
    }

    out.close();
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new PartitionEfficiencyTestSuiteMetricPlugin());
}

} /* namespace soda */
