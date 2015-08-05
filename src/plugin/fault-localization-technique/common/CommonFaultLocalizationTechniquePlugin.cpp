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

void CommonFaultLocalizationTechniquePlugin::init(CSelectionData *data, ClusterMap *clusters, IndexType revision)
{
    m_data = data;
    clusterList = clusters;
    m_revision = revision;
}

CommonFaultLocalizationTechniquePlugin::FLDistribution& CommonFaultLocalizationTechniquePlugin::getDistribution()
{
    return distribution;
}

void CommonFaultLocalizationTechniquePlugin::calculate(rapidjson::Document &res)
{
    CCoverageMatrix *coverageMatrix = m_data->getCoverage();

    ClusterMap::iterator it;
    for (it = clusterList->begin(); it != clusterList->end(); it++) {
        std::map<IndexType, IndexType> tcMap;
        IntVector testCaseIds = it->second.getTestCases();
        IntVector codeElementIds = it->second.getCodeElements();

        // group for cluster data
        if (!res.HasMember(it->first.c_str())) {
            res.AddMember(rapidjson::Value(it->first.c_str(), res.GetAllocator()), rapidjson::Value(rapidjson::kObjectType), res.GetAllocator());
        }

        for (IndexType i = 0; i < testCaseIds.size(); i++) {
            tcMap[testCaseIds[i]] = m_data->translateTestcaseIdFromCoverageToResults(testCaseIds[i]);
        }

        for (IndexType i = 0; i < codeElementIds.size(); i++) {
            IndexType cid = codeElementIds[i];
            String ceIdStr = std::to_string(cid);

            // holds the metric values for one code element
            if (!res[it->first.c_str()].HasMember(ceIdStr.c_str())) {
                res[it->first.c_str()].AddMember(rapidjson::Value(ceIdStr.c_str(), res.GetAllocator()), rapidjson::Value(rapidjson::kObjectType), res.GetAllocator());
            }
            rapidjson::Value &ceMetrics = res[it->first.c_str()][ceIdStr.c_str()];

            IndexType failedCovered = 0;
            IndexType passedCovered = 0;
            IndexType failedNotCovered = 0;
            IndexType passedNotCovered = 0;
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

            double efperefep = 0;
            if ((failedCovered + passedCovered) > 0) {
                efperefep = (double)failedCovered / (failedCovered + passedCovered);
            }
            double nfpernfnp = 0;
            if ((failedNotCovered + passedNotCovered) > 0) {
                nfpernfnp = (double)failedNotCovered / (failedNotCovered + passedNotCovered);
            }

            // ef; ep; nf; np; ef/(ef+ep); nf/(nf+np);
            ceMetrics.AddMember("ef", failedCovered, res.GetAllocator());
            ceMetrics.AddMember("ep", passedCovered, res.GetAllocator());
            ceMetrics.AddMember("nf", failedNotCovered, res.GetAllocator());
            ceMetrics.AddMember("np", passedNotCovered, res.GetAllocator());
            ceMetrics.AddMember("ef/(ef+ep)", efperefep, res.GetAllocator());
            ceMetrics.AddMember("nf/(nf+np)", nfpernfnp, res.GetAllocator());
        }
    }
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getFaultLocalizationTechniquePluginManager().addPlugin(new CommonFaultLocalizationTechniquePlugin());
}

} /* namespace soda */
