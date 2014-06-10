#include "CoverageEfficiencyTestSuiteMetricPlugin.h"

namespace soda {

CoverageEfficiencyTestSuiteMetricPlugin::CoverageEfficiencyTestSuiteMetricPlugin() :
    m_data(NULL),
    m_clusterList(NULL)
{
}

CoverageEfficiencyTestSuiteMetricPlugin::~CoverageEfficiencyTestSuiteMetricPlugin()
{
}

std::string CoverageEfficiencyTestSuiteMetricPlugin::getName()
{
    return "coverage-efficiency";
}

std::string CoverageEfficiencyTestSuiteMetricPlugin::getDescription()
{
    return "Returns the coverage efficiency of test suite.";
}

void CoverageEfficiencyTestSuiteMetricPlugin::init(CSelectionData *data, std::map<std::string, CClusterDefinition> *clusterList, IndexType revision)
{
    m_data = data;
    m_clusterList = clusterList;
}

std::vector<std::string> CoverageEfficiencyTestSuiteMetricPlugin::getDependency()
{
    std::vector<std::string> dependencies;
    dependencies.push_back("fault-detection");
    return dependencies;
}

void CoverageEfficiencyTestSuiteMetricPlugin::calculate(const std::string &output, std::map<std::string, MetricResults> &results)
{
    std::ofstream out;
    out.open((output + "/coverage.efficiency.metric.csv").c_str());
    out << "# cluster id;number of testcases in cluster;number of code elements in cluster;coverage-efficiency" << std::endl;

    std::map<std::string, MetricResults>::iterator it;
    for (it = results.begin(); it != results.end(); it++) {
        MetricResults result = it->second;

        IndexType nrOfTestCases = (*m_clusterList)[it->first].getTestCases().size();
        IndexType nrOfCodeElements = (*m_clusterList)[it->first].getCodeElements().size();

        double faultDetection = result["fault-detection"];
        double coverageEfficiency = (faultDetection * nrOfCodeElements) / nrOfTestCases;

        out << it->first << ";" << nrOfTestCases << ";" << nrOfCodeElements << ";" << coverageEfficiency << std::endl;

        results[it->first]["coverage-efficiency"] = coverageEfficiency;
    }

    out.close();
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new CoverageEfficiencyTestSuiteMetricPlugin());
}

} /* namespace soda */
