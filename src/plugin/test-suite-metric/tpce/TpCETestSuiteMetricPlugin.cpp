#include "TpCETestSuiteMetricPlugin.h"

namespace soda {

TpCETestSuiteMetricPlugin::TpCETestSuiteMetricPlugin() :
    m_data(NULL),
    m_clusterList(NULL)
{
}

TpCETestSuiteMetricPlugin::~TpCETestSuiteMetricPlugin()
{
}

std::string TpCETestSuiteMetricPlugin::getName()
{
    return "tpce";
}

std::string TpCETestSuiteMetricPlugin::getDescription()
{
    return "The ratio of test cases to code elements.";
}

void TpCETestSuiteMetricPlugin::init(CSelectionData *data, std::map<std::string, CClusterDefinition> *clusterList, IndexType revision)
{
    m_data = data;
    m_clusterList = clusterList;
}

std::vector<std::string> TpCETestSuiteMetricPlugin::getDependency()
{
    return std::vector<std::string>();
}

void TpCETestSuiteMetricPlugin::calculate(const std::string &output, std::map<std::string, MetricResults> &results)
{
    std::ofstream out;
    out.open((output + "/tpce.metric.csv").c_str());
    out << "# cluster id;number of testcases in cluster;number of code elements;tpce" << std::endl;


    std::map<std::string, CClusterDefinition>::iterator it;
    for (it = m_clusterList->begin(); it != m_clusterList->end(); it++) {
        IndexType nrOfCodeElements = it->second.getCodeElements().size();
        IndexType nrOfTestcases = it->second.getTestCases().size();

        double tpce = (double)nrOfTestcases / nrOfCodeElements;

        out << it->first << ";" << nrOfTestcases << ";" << nrOfCodeElements << ";" << tpce << std::endl;

        results[it->first]["tpce"] = tpce;
    }

    out.close();
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new TpCETestSuiteMetricPlugin());
}

} /* namespace soda */
