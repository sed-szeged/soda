/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Havas <havasd@inf.u-szeged.hu>
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

#include "CommonFaultLocalizationTechniquePlugin.h"

namespace soda {

CommonFaultLocalizationTechniquePlugin::CommonFaultLocalizationTechniquePlugin() :
    m_data(NULL),
    clusterList(NULL),
    m_flScore(NULL),
    m_revision(0)
{
}

CommonFaultLocalizationTechniquePlugin::~CommonFaultLocalizationTechniquePlugin()
{
}

String CommonFaultLocalizationTechniquePlugin::getName()
{
    return "common";
}

String CommonFaultLocalizationTechniquePlugin::getDescription()
{
    return "Calculates the common values for each code element.";
}

StringVector CommonFaultLocalizationTechniquePlugin::getDependency()
{
    return StringVector();
}

void CommonFaultLocalizationTechniquePlugin::init(CSelectionData *data, ClusterMap *clusters, IndexType revision, IntVector failedCodeElements)
{
    m_data = data;
    clusterList = clusters;
    m_revision = revision;
}

CommonFaultLocalizationTechniquePlugin::FLDistribution& CommonFaultLocalizationTechniquePlugin::getDistribution()
{
    return distribution;
}

CommonFaultLocalizationTechniquePlugin::FLScore& CommonFaultLocalizationTechniquePlugin::getFlScore()
{
    return *m_flScore;
}

void CommonFaultLocalizationTechniquePlugin::calculate(rapidjson::Document &res)
{
    auto& coverage = m_data->getCoverage()->getBitMatrix();
    auto results = m_data->getResults();
    auto& exec = results->getExecutionBitList(m_revision);
    auto& pass = results->getPassedBitList(m_revision);

    for (auto it = clusterList->begin(); it != clusterList->end(); it++) {
        auto cluster_name = it->first.c_str();
        auto& cluster_def = it->second;

        auto& testCaseIds = cluster_def.getTestCases();
        auto& codeElementIds = cluster_def.getCodeElements();

        // group for cluster data
        if (!res.HasMember(cluster_name)) {
            res.AddMember(rapidjson::Value(cluster_name, res.GetAllocator()), rapidjson::Value(rapidjson::kObjectType), res.GetAllocator());
        }

        auto& cluster_res = res[cluster_name];

        std::map<IndexType, IndexType> tcMap;

        for (IndexType i = 0; i < testCaseIds.size(); i++) {
            tcMap[testCaseIds[i]] = m_data->translateTestcaseIdFromCoverageToResults(testCaseIds[i]);
        }

        /*for (IndexType i = 0; i < codeElementIds.size(); i++) {
            IndexType cid = codeElementIds[i];
            auto ceIdStr = std::to_string(cid).c_str();

            if (!cluster_res.HasMember(ceIdStr)) {
                cluster_res.AddMember(rapidjson::Value(ceIdStr, res.GetAllocator()), rapidjson::Value(rapidjson::kObjectType), res.GetAllocator());
            }
        }*/

        #pragma omp parallel for schedule(dynamic)
        for (IndexType i = 0; i < codeElementIds.size(); i++) {
            IndexType cid = codeElementIds[i];
            auto ceIdStr = std::to_string(cid).c_str();

            IndexType failedCovered = 0;
            IndexType passedCovered = 0;
            IndexType failedNotCovered = 0;
            IndexType passedNotCovered = 0;
            for (IndexType j = 0; j < testCaseIds.size(); j++) {
                IndexType tcid = testCaseIds[j];
                IndexType tcidInResults = tcMap[tcid];

                if (exec.at(tcidInResults)) {
                    bool isPassed = pass.at(tcidInResults);
                    bool isCovered = coverage[tcid][cid];
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

            double efperefep = 0;
            if ((failedCovered + passedCovered) > 0) {
                efperefep = (double)failedCovered / (failedCovered + passedCovered);
            }
            double nfpernfnp = 0;
            if ((failedNotCovered + passedNotCovered) > 0) {
                nfpernfnp = (double)failedNotCovered / (failedNotCovered + passedNotCovered);
            }
            double dstar = 0;
            {
                IndexType nrOfFailedTestcases = failedCovered + failedNotCovered;
                IndexType denominator = passedCovered + (nrOfFailedTestcases - failedCovered);
                if (denominator > 0) {
                    dstar = std::pow((double)failedCovered, 2) / denominator;
                }
            }
            double ochiai = 0;
            {
                IndexType nrOfFailedTestcases = failedCovered + failedNotCovered;
                IndexType covered = failedCovered + passedCovered;
                if (nrOfFailedTestcases > 0 && covered > 0) {
                    double denominator = std::sqrt(nrOfFailedTestcases * covered);
                    if (denominator > 0) {
                        ochiai = (double)failedCovered / denominator;
                    }
                }
            }
            double tarantula = 0;
            {
                IndexType nrOfFailedTestcases = failedCovered + failedNotCovered;
                IndexType nrOfPassedTestcases = passedCovered + passedNotCovered;
                if (nrOfFailedTestcases > 0 && nrOfPassedTestcases > 0) {
                    double denominator = (((double)failedCovered / nrOfFailedTestcases) + ((double)passedCovered / nrOfPassedTestcases));
                    if (denominator > 0) {
                        tarantula = ((double)failedCovered / nrOfFailedTestcases) / denominator;
                    }
                }
            }

            // holds the metric values for one code element
            if (!cluster_res.HasMember(ceIdStr)) {
                #pragma omp critical(json)
                {
                    cluster_res.AddMember(rapidjson::Value(ceIdStr, res.GetAllocator()), rapidjson::Value(rapidjson::kObjectType), res.GetAllocator());
                }
            }

            auto& ceMetrics = cluster_res[ceIdStr];

            // ef; ep; nf; np; ef/(ef+ep); nf/(nf+np);
            ceMetrics.AddMember("ef", failedCovered, res.GetAllocator());
            ceMetrics.AddMember("ep", passedCovered, res.GetAllocator());
            ceMetrics.AddMember("nf", failedNotCovered, res.GetAllocator());
            ceMetrics.AddMember("np", passedNotCovered, res.GetAllocator());
            ceMetrics.AddMember("ef/(ef+ep)", efperefep, res.GetAllocator());
            ceMetrics.AddMember("nf/(nf+np)", nfpernfnp, res.GetAllocator());

            ceMetrics.AddMember("dstar", dstar, res.GetAllocator());
            ceMetrics.AddMember("ochiai", ochiai, res.GetAllocator());
            ceMetrics.AddMember("tarantula", tarantula, res.GetAllocator());
        }
    }
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getFaultLocalizationTechniquePluginManager().addPlugin(new CommonFaultLocalizationTechniquePlugin());
}

} /* namespace soda */
