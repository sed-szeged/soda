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

#include <cmath>

#include "OchiaiFaultLocalizationTechniquePlugin.h"

namespace soda {

OchiaiFaultLocalizationTechniquePlugin::OchiaiFaultLocalizationTechniquePlugin() :
    m_distribution(new FLDistribution()),
    m_data(NULL),
    clusterList(NULL),
    m_revision(0)
{
}

OchiaiFaultLocalizationTechniquePlugin::~OchiaiFaultLocalizationTechniquePlugin()
{
    delete m_distribution;
}

String OchiaiFaultLocalizationTechniquePlugin::getName()
{
    return "ochiai";
}

String OchiaiFaultLocalizationTechniquePlugin::getDescription()
{
    return "Calculates the ochiai value for each code element.";
}

StringVector OchiaiFaultLocalizationTechniquePlugin::getDependency()
{
    return { "common" };
}

void OchiaiFaultLocalizationTechniquePlugin::init(CSelectionData *data, ClusterMap *clusters, IndexType revision)
{
    m_data = data;
    clusterList = clusters;
    m_revision = revision;
}

OchiaiFaultLocalizationTechniquePlugin::FLDistribution& OchiaiFaultLocalizationTechniquePlugin::getDistribution()
{
    return *m_distribution;
}

void OchiaiFaultLocalizationTechniquePlugin::calculate(rapidjson::Document &res)
{
    m_distribution->clear();

    CCoverageMatrix *coverageMatrix = m_data->getCoverage();

    ClusterMap::iterator it;
    for (it = clusterList->begin(); it != clusterList->end(); it++) {
        IntVector codeElementIds = (*it).second.getCodeElements();

        // group for cluster data
        if (!res.HasMember(it->first.c_str())) {
            res.AddMember(rapidjson::Value(it->first.c_str(), res.GetAllocator()), rapidjson::Value(rapidjson::kObjectType), res.GetAllocator());
        }

        for (IndexType i = 0; i < codeElementIds.size(); i++) {
            IndexType cid = codeElementIds[i];
            String ceIdStr = std::to_string(cid);

            // holds the metric values for one code element
            if (!res[it->first.c_str()].HasMember(ceIdStr.c_str())) {
                res[it->first.c_str()].AddMember(rapidjson::Value(ceIdStr.c_str(), res.GetAllocator()), rapidjson::Value(rapidjson::kObjectType), res.GetAllocator());
            }
            rapidjson::Value &ceMetrics = res[it->first.c_str()][ceIdStr.c_str()];

            IndexType failedCovered = ceMetrics["ef"].GetUint64();
            IndexType passedCovered = ceMetrics["ep"].GetUint64();
            IndexType failedNotCovered = ceMetrics["nf"].GetUint64();
            IndexType passedNotCovered = ceMetrics["np"].GetUint64();

            double ochiai = 0;
            IndexType nrOfFailedTestcases = failedCovered + failedNotCovered;
            IndexType covered = failedCovered + passedCovered;
            if (nrOfFailedTestcases > 0 && covered > 0) {
                double denominator = std::sqrt(nrOfFailedTestcases * covered);
                if (denominator > 0) {
                    ochiai = (double)failedCovered / denominator;
                }
            }

            ceMetrics.AddMember("ochiai", ochiai, res.GetAllocator());
            (*m_distribution)[ochiai]++;
        }
    }
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getFaultLocalizationTechniquePluginManager().addPlugin(new OchiaiFaultLocalizationTechniquePlugin());
}

} /* namespace soda */
