#include "UniquenessTestSuiteMetricPlugin.h"

namespace soda {

UniquenessTestSuiteMetricPlugin::UniquenessTestSuiteMetricPlugin() :
    m_data(NULL),
    m_clusterList(NULL),
    m_revision(0)
{
}

UniquenessTestSuiteMetricPlugin::~UniquenessTestSuiteMetricPlugin()
{
}

std::string UniquenessTestSuiteMetricPlugin::getName()
{
    return "uniqueness";
}

std::string UniquenessTestSuiteMetricPlugin::getDescription()
{
    return "";
}

void UniquenessTestSuiteMetricPlugin::init(CSelectionData *data, std::map<std::string, CClusterDefinition> *clusterList, IndexType revision)
{
    m_data = data;
    m_clusterList = clusterList;
    m_revision = revision;
}

std::vector<std::string> UniquenessTestSuiteMetricPlugin::getDependency()
{
    return std::vector<std::string>();
}

void UniquenessTestSuiteMetricPlugin::calculate(rapidjson::Document &results)
{
    /*
    std::ofstream out;
    out.open((output + "/uniqueness.metric.csv").c_str());
    out << "# cluster id;number of testcases in cluster;number of code elements;uniqueness" << std::endl;
    */

    CCoverageMatrix *coverage = m_data->getCoverage();
    IndexType nrOfTestcases = coverage->getNumOfTestcases();

    std::map<IndexType, bool> tcMap;

    std::cout << std::endl;

    for (IndexType i = 0; i < nrOfTestcases; i++) {
        tcMap[i] = m_data->getResults()->getExecutionBitList(m_revision).at(i);
    }


    std::map<std::string, CClusterDefinition>::iterator it;
    for (it = m_clusterList->begin(); it != m_clusterList->end(); it++) {

        if (!results.HasMember(it->first.c_str())) {
            rapidjson::Value cluster;
            cluster.SetObject();
            results.AddMember(it->first.c_str(), results.GetAllocator(), cluster, results.GetAllocator());
        }

        std::cout << "Processing " << it->first << std::endl;
        IndexType nrOfCodeElementsInCluster = it->second.getCodeElements().size();
        IndexType nrOfTestcasesInCluster = it->second.getTestCases().size();

        std::set<IndexType> testcasesNotInCluster;
        std::set<IndexType> testcasesInCluster;
        for (IndexType i = 0; i < nrOfTestcasesInCluster; i++) {
            if (tcMap[it->second.getTestCases().at(i)]) {
                testcasesInCluster.insert(it->second.getTestCases().at(i));
            }
        }

        for (IndexType i = 0; i < nrOfTestcases; i++) {
            if (tcMap[i] && testcasesInCluster.find(i) == testcasesInCluster.end()) {
                testcasesNotInCluster.insert(i);
            }
        }

        IndexType nrOfCoveredElements = 0;
        IndexType nrOfCoveredUniq = 0;

        for (IndexType i = 0; i < nrOfCodeElementsInCluster; i++) {
            IndexType cid = it->second.getCodeElements().at(i);
            std::set<IndexType>::iterator tit;

            bool covered = false;
            for (tit = testcasesInCluster.begin(); tit != testcasesInCluster.end(); tit++) {
                IndexType tcid = *tit;
                if (coverage->getBitMatrix().get(tcid, cid)) {
                    covered = true;
                    break;
                }
            }
            if (covered) {
                nrOfCoveredElements++;

                bool uniq = true;
                for (tit = testcasesNotInCluster.begin(); tit != testcasesNotInCluster.end(); tit++) {
                    IndexType tcid = *tit;
                    if (coverage->getBitMatrix().get(tcid, cid)) {
                        uniq = false;
                        break;
                    }
                }
                if (uniq) {
                    nrOfCoveredUniq++;
                }
            }
        }
        double uniqueness = 0.0;
        if (nrOfCoveredElements > 0) {
            uniqueness = (double)nrOfCoveredUniq / nrOfCoveredElements;
        }

        rapidjson::Value v;
        v.SetDouble(uniqueness);
        results[it->first.c_str()].AddMember("uniqueness", results.GetAllocator(), v, results.GetAllocator());
        //out << it->first << ";" << nrOfTestcasesInCluster << ";" << nrOfCodeElementsInCluster << ";" << uniqueness  << std::endl;
    }
    //out.close();
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new UniquenessTestSuiteMetricPlugin());
}

} /* namespace soda */
