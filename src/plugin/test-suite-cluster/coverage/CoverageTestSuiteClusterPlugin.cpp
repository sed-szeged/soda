#include <algorithm>
#include "CoverageTestSuiteClusterPlugin.h"

namespace soda {

bool operator<(CoverageTestSuiteClusterPlugin::qelement d1, CoverageTestSuiteClusterPlugin::qelement d2) {
    return d1.priorityValue > d2.priorityValue;
}

CoverageTestSuiteClusterPlugin::CoverageTestSuiteClusterPlugin() :
    m_sizes(std::vector<IndexType>())
{
}

CoverageTestSuiteClusterPlugin::~CoverageTestSuiteClusterPlugin()
{
}

std::string CoverageTestSuiteClusterPlugin::getName()
{
    return "coverage";
}

std::string CoverageTestSuiteClusterPlugin::getDescription()
{
    return "Generates test groups ordered by their coverage with different sizes given in the percentage of test cases.";
}

std::map<String, String> CoverageTestSuiteClusterPlugin::getRequiredParameters()
{
    std::map<String, String> requiredParams;
    requiredParams["cluster-sizes"] = TYPE_INTARRAY;
    return requiredParams;
}

void CoverageTestSuiteClusterPlugin::init(rapidjson::Document &doc)
{
    m_sizes.clear();
    for (rapidjson::Value::ConstValueIterator it = doc["cluster-sizes"].Begin(); it != doc["cluster-sizes"].End(); it++) {
        m_sizes.push_back(it->GetInt());
    }
}

void CoverageTestSuiteClusterPlugin::execute(CSelectionData &data, std::map<std::string, CClusterDefinition>& clusterList)
{
    // Sort the test cases based on their coverage.
    IndexType nofTestcases = data.getCoverage()->getNumOfTestcases();
    std::vector<qelement> priorityQueue;
    const IBitMatrix& coverageBitMatrix = data.getCoverage()->getBitMatrix();
    for(IndexType tcid = 0; tcid < nofTestcases; tcid++) {
        qelement d;
        d.testcaseId = tcid;
        d.priorityValue = coverageBitMatrix[tcid].count();
        priorityQueue.push_back(d);
    }

    std::sort(priorityQueue.begin(), priorityQueue.end());

    for (std::vector<IndexType>::iterator it = m_sizes.begin(); it != m_sizes.end(); it++) {
        IndexType size = std::floor(nofTestcases * ((double)*it / 100));
        if (size > nofTestcases) {
            // TODO Handle this error.
            std::cerr << "Invalid size." << std::endl;
            continue;
        }

        std::string clusterName = boost::lexical_cast<std::string>(*it);
        for (IndexType i = 0; i < size; i++) {
            clusterList[clusterName].addTestCase(priorityQueue[i].testcaseId);
        }
        clusterList[clusterName].addCodeElements(data.getCoverage()->getCodeElements().getIDList());
    }
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteClusterPluginManager().addPlugin(new CoverageTestSuiteClusterPlugin());
}

} /* namespace soda */
