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
    m_values(new FLValues()),
    m_distribution(new FLDistribution()),
    m_data(NULL),
    m_revision(0)
{
}

TarantulaFaultLocalizationTechniquePlugin::~TarantulaFaultLocalizationTechniquePlugin()
{
    delete m_values;
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

void TarantulaFaultLocalizationTechniquePlugin::init(CSelectionData *data, IndexType revision)
{
    m_data = data;
    m_revision = revision;
}

TarantulaFaultLocalizationTechniquePlugin::FLValues& TarantulaFaultLocalizationTechniquePlugin::getValues()
{
    return *m_values;
}

TarantulaFaultLocalizationTechniquePlugin::FLDistribution& TarantulaFaultLocalizationTechniquePlugin::getDistribution()
{
    return *m_distribution;
}

void TarantulaFaultLocalizationTechniquePlugin::calculate(CClusterDefinition &cluster, const std::string &output)
{
    (std::cerr << "[INFO] Tarantula ... ").flush();

    m_values->clear();
    m_distribution->clear();

    std::ofstream tarantulaStream;
    std::ofstream tarantulaStreamDetailed;

    tarantulaStream.open((output + "/tarantula.csv").c_str());
    tarantulaStreamDetailed.open((output + "/tarantula.details.csv").c_str());

    tarantulaStream << "#revision; code element; tarantula" << std::endl;
    tarantulaStreamDetailed << "#revision; code element; ef; np; nf; np; tarantula" << std::endl;

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
                    if (isPassed) {
                        passedCovered++;
                    } else {
                        failedCovered++;
                    }
                } else {
                    if (isPassed) {
                        passedNotCovered++;
                    } else {
                        failedNotCovered++;
                    }
                }
            }
        }
        double tarantula = 0;
        IndexType nrOfFailedTestcases = failedCovered + failedNotCovered;
        IndexType nrOfPassedTestcases = passedCovered + passedNotCovered;
        if (nrOfFailedTestcases > 0 && nrOfPassedTestcases > 0) {
            double denominator = (((double)failedCovered/nrOfFailedTestcases) + ((double)passedCovered/nrOfPassedTestcases));
            if (denominator > 0) {
                tarantula = ((double)failedCovered/nrOfFailedTestcases) / denominator;
            }
        }

        (*m_values)[cid] = tarantula;
        (*m_distribution)[tarantula]++;

        tarantulaStream << m_revision << ";" << cid << ";" << tarantula << std::endl;
        tarantulaStreamDetailed << m_revision << ";" << cid << ";" << failedCovered << ";" << passedCovered << ";" << failedNotCovered << ";" << passedNotCovered << ";" << tarantula << std::endl;
    }

    tarantulaStream.close();
    tarantulaStreamDetailed.close();

    (std::cerr << "done." << std::endl).flush();
}

} /* namespace soda */
