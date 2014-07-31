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

void CoverageEfficiencyTestSuiteMetricPlugin::calculate(rapidjson::Document &results)
{
    /*
    std::ofstream out;
    out.open((output + "/coverage.efficiency.metric.csv").c_str());
    out << "# cluster id;number of testcases in cluster;number of code elements in cluster;coverage-efficiency" << std::endl;
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
        IndexType nrOfCodeElements = it->second.getCodeElements().size();

        double faultDetection = results[it->first.c_str()]["fault-detection"].GetDouble();
        double coverageEfficiency = (faultDetection * nrOfCodeElements) / nrOfTestCases;

        //out << it->first << ";" << nrOfTestCases << ";" << nrOfCodeElements << ";" << coverageEfficiency << std::endl;

        rapidjson::Value::MemberIterator metricIt = results[it->first.c_str()].FindMember("coverage-efficiency");
        if (metricIt == results[it->first.c_str()].MemberEnd()) {
            rapidjson::Value v;
            v.SetDouble(coverageEfficiency);
            results[it->first.c_str()].AddMember("coverage-efficiency", v, results.GetAllocator());
        } else
            metricIt->value.SetDouble(coverageEfficiency);
    }

    //out.close();
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new CoverageEfficiencyTestSuiteMetricPlugin());
}

} /* namespace soda */
