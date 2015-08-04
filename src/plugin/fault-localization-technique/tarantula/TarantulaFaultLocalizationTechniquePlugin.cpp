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
#include "TarantulaFaultLocalizationTechniquePlugin.h"

namespace soda {

TarantulaFaultLocalizationTechniquePlugin::TarantulaFaultLocalizationTechniquePlugin() :
    m_distribution(new FLDistribution()),
    m_data(NULL),
    m_revision(0)
{
}

TarantulaFaultLocalizationTechniquePlugin::~TarantulaFaultLocalizationTechniquePlugin()
{
    delete m_distribution;
}

std::string TarantulaFaultLocalizationTechniquePlugin::getName()
{
    return "tarantula";
}

std::string TarantulaFaultLocalizationTechniquePlugin::getDescription()
{
    return "Calculates the tarantula value for each code element.";
}

void TarantulaFaultLocalizationTechniquePlugin::init(CSelectionData *data, ClusterMap *clusters, IndexType revision)
{
    m_data = data;
    clusterList = clusters;
    m_revision = revision;
}

TarantulaFaultLocalizationTechniquePlugin::FLDistribution& TarantulaFaultLocalizationTechniquePlugin::getDistribution()
{
    return *m_distribution;
}

void TarantulaFaultLocalizationTechniquePlugin::calculate(rapidjson::Document &res)
{
    (std::cerr << "[INFO] Tarantula ... ").flush();
    m_distribution->clear();

    CCoverageMatrix *coverageMatrix = m_data->getCoverage();

    std::map<std::string, CClusterDefinition>::iterator it;
    for (it = clusterList->begin(); it != clusterList->end(); it++) {

        std::map<IndexType, IndexType> tcMap;
        IntVector testCaseIds = it->second.getTestCases();
        IntVector codeElementIds = it->second.getCodeElements();

        // FIXME: Remove code duplication.
        bool hasCluster = res.HasMember(it->first.c_str());
        rapidjson::Value *clusterObj = nullptr;
        // group for cluster data
        if (!hasCluster) {
            res.AddMember(rapidjson::Value(it->first.c_str(), res.GetAllocator()), rapidjson::Value(rapidjson::kObjectType), res.GetAllocator());
            clusterObj = &res[it->first.c_str()];
        }
        else {
            clusterObj = &res[it->first.c_str()];
        }

        if (!hasCluster) {
            for (IndexType i = 0; i < testCaseIds.size(); i++) {
                tcMap[testCaseIds[i]] = m_data->translateTestcaseIdFromCoverageToResults(testCaseIds[i]);
            }
        }

        for (IndexType i = 0; i < codeElementIds.size(); i++) {
            IndexType cid = codeElementIds[i];
            std::string ceIdStr = std::to_string(cid);

            // holds the metric values for one code element
            rapidjson::Value *ceMetrics;
            if (!clusterObj->HasMember(ceIdStr.c_str())) {
                clusterObj->AddMember(rapidjson::Value(ceIdStr.c_str(), res.GetAllocator()), rapidjson::Value(rapidjson::kObjectType), res.GetAllocator());
                ceMetrics = &(*clusterObj)[ceIdStr.c_str()];
            }
            else {
                ceMetrics = &(*clusterObj)[ceIdStr.c_str()];
            }

            IndexType failedCovered = 0;
            IndexType passedCovered = 0;
            IndexType failedNotCovered = 0;
            IndexType passedNotCovered = 0;

            if (hasCluster) {
                failedCovered = (*ceMetrics)["ef"].GetUint64();
                passedCovered = (*ceMetrics)["ep"].GetUint64();
                failedNotCovered = (*ceMetrics)["nf"].GetUint64();
                passedNotCovered = (*ceMetrics)["np"].GetUint64();
            }
            else {
                double efperefep = 0;
                double nfpernfnp = 0;

                for (IndexType j = 0; j < testCaseIds.size(); j++) {
                    IndexType tcid = testCaseIds[j];
                    IndexType tcidInResults = tcMap[tcid];

                    if (m_data->getResults()->getExecutionBitList(m_revision).at(tcidInResults)) {
                        bool isPassed = m_data->getResults()->getPassedBitList(m_revision).at(tcidInResults);
                        bool isCovered = coverageMatrix->getBitMatrix().get(tcid, cid);
                        if (isCovered) {
                            if (isPassed) {
                                passedCovered++;
                            }
                            else {
                                failedCovered++;
                            }
                        }
                        else {
                            if (isPassed) {
                                passedNotCovered++;
                            }
                            else {
                                failedNotCovered++;
                            }
                        }
                    }
                }

                if ((failedCovered + passedCovered) > 0) {
                    efperefep = (double)failedCovered / (failedCovered + passedCovered);
                }
                if ((failedNotCovered + passedNotCovered) > 0) {
                    nfpernfnp = (double)failedNotCovered / (failedNotCovered + passedNotCovered);
                }

                // ef; ep; nf; np; ef/(ef+ep); nf/(nf+np);
                ceMetrics->AddMember("ef", failedCovered, res.GetAllocator());
                ceMetrics->AddMember("ep", passedCovered, res.GetAllocator());
                ceMetrics->AddMember("nf", failedNotCovered, res.GetAllocator());
                ceMetrics->AddMember("np", passedNotCovered, res.GetAllocator());
                ceMetrics->AddMember("ef/(ef+ep)", efperefep, res.GetAllocator());
                ceMetrics->AddMember("nf/(nf+np)", nfpernfnp, res.GetAllocator());
            }

            double tarantula = 0;
            IndexType nrOfFailedTestcases = failedCovered + failedNotCovered;
            IndexType nrOfPassedTestcases = passedCovered + passedNotCovered;
            if (nrOfFailedTestcases > 0 && nrOfPassedTestcases > 0) {
                double denominator = (((double)failedCovered / nrOfFailedTestcases) + ((double)passedCovered / nrOfPassedTestcases));
                if (denominator > 0) {
                    tarantula = ((double)failedCovered / nrOfFailedTestcases) / denominator;
                }
            }

            ceMetrics->AddMember("tarantula", tarantula, res.GetAllocator());

            (*m_distribution)[tarantula]++;
        }
    }

    (std::cerr << "done." << std::endl).flush();
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getFaultLocalizationTechniquePluginManager().addPlugin(new TarantulaFaultLocalizationTechniquePlugin());
}

} /* namespace soda */
