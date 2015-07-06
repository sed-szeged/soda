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
    m_values(new FLValues()),
    m_distribution(new FLDistribution()),
    m_data(NULL),
    m_revision(0)
{
}

OchiaiFaultLocalizationTechniquePlugin::~OchiaiFaultLocalizationTechniquePlugin()
{
    delete m_values;
    delete m_distribution;
}

std::string OchiaiFaultLocalizationTechniquePlugin::getName()
{
    return "ochiai";
}

std::string OchiaiFaultLocalizationTechniquePlugin::getDescription()
{
    return "Calculates the ochiai value for each code element.";
}

void OchiaiFaultLocalizationTechniquePlugin::init(CSelectionData *data, IndexType revision)
{
    m_data = data;
    m_revision = revision;
}

OchiaiFaultLocalizationTechniquePlugin::FLValues& OchiaiFaultLocalizationTechniquePlugin::getValues()
{
    return *m_values;
}

OchiaiFaultLocalizationTechniquePlugin::FLDistribution& OchiaiFaultLocalizationTechniquePlugin::getDistribution()
{
    return *m_distribution;
}

void OchiaiFaultLocalizationTechniquePlugin::calculate(CClusterDefinition &cluster, const std::string &output)
{
    (std::cerr << "[INFO] Ochiai ... ").flush();

    delete m_values;
    m_values = new FLValues();
    m_values->SetObject();
    m_distribution->clear();

    bool writeDetails = !output.empty();
    std::ofstream ochiaiStreamDetailed;
    if (writeDetails) {
        ochiaiStreamDetailed.open((output + "/ochiai.details.csv").c_str());
        ochiaiStreamDetailed << "#revision; code element; ef; ep; nf; np; ochiai" << std::endl;
    }

    CCoverageMatrix *coverageMatrix = m_data->getCoverage();

    std::map<IndexType, IndexType> tcMap;
    IntVector testCaseIds = cluster.getTestCases();
    IntVector codeElementIds = cluster.getCodeElements();

    for (IndexType i = 0; i < testCaseIds.size(); i++) {
        tcMap[testCaseIds[i]] = m_data->translateTestcaseIdFromCoverageToResults(testCaseIds[i]);
    }

    for (IndexType i = 0; i < codeElementIds.size(); i++) {
        IndexType cid = codeElementIds[i];
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
                    if (!isPassed) {
                        failedCovered++;
                    } else {
                        passedCovered++;
                    }
                } else {
                    if (!isPassed) {
                        failedNotCovered++;
                    } else {
                        passedNotCovered++;
                    }
                }
            }
        }
        double ochiai = 0;
        IndexType nrOfFailedTestcases = failedCovered + failedNotCovered;
        IndexType covered = failedCovered + passedCovered;
        if (nrOfFailedTestcases > 0 && covered > 0) {
            double denominator = std::sqrt(nrOfFailedTestcases * covered);
            if (denominator > 0) {
                ochiai = (double)failedCovered / denominator;
            }
        }

        {
            rapidjson::Value key;
            key.SetString(static_cast<std::ostringstream*>( &(std::ostringstream() << cid) )->str().c_str(), m_values->GetAllocator());
            rapidjson::Value val;
            val.SetDouble(ochiai);
            m_values->AddMember(key, val, m_values->GetAllocator());
        }

        (*m_distribution)[ochiai]++;

        if (writeDetails)
            ochiaiStreamDetailed << m_revision << ";" << cid << ";" << failedCovered << ";" << passedCovered << ";" << failedNotCovered << ";" << passedNotCovered << ";" << ochiai << std::endl;
    }

    if (writeDetails)
        ochiaiStreamDetailed.close();

    (std::cerr << "done." << std::endl).flush();
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getFaultLocalizationTechniquePluginManager().addPlugin(new OchiaiFaultLocalizationTechniquePlugin());
}

} /* namespace soda */
