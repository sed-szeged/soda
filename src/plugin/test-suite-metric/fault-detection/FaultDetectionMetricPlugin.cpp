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

#include <fstream>

#include "FaultDetectionMetricPlugin.h"

namespace soda {

FaultDetectionMetricPlugin::FaultDetectionMetricPlugin()
{

}

FaultDetectionMetricPlugin::~FaultDetectionMetricPlugin()
{

}

std::string FaultDetectionMetricPlugin::getName()
{
    return "fault-detection";
}

std::string FaultDetectionMetricPlugin::getDescription()
{
    return "Calculating fault detection metric of the test suite.";
}

void FaultDetectionMetricPlugin::calculate(CSelectionData &data, std::vector<CClusterDefinition> &clusterList, const std::vector<IndexType> &revisionList, const std::string &output)
{
    std::ofstream coverageStream;
    coverageStream.open((output + "/fault.detection.metric.csv").c_str());
    coverageStream << "# cluster id; number of methods; number of methods covered; coverage (%)" << std::endl;

    std::set<IndexType> coveredElementIds;

    CCoverageMatrix *coverage = data.getCoverage();
    for (IndexType i = 0; i < clusterList.size(); i++) {

        IndexType nrOfCodeElements = clusterList[i].getCodeElements().size();
        IndexType nrOfTestcases = clusterList[i].getTestCases().size();
        IndexType nrOfCoveredCodeElements = 0;

        coveredElementIds.clear();

        for (IndexType j = 0; j < nrOfCodeElements; j++) {
            coveredElementIds.insert(clusterList[i].getCodeElements().at(j));
        }

        for (IndexType j = 0; j < nrOfTestcases; j++) {
            IndexType tcid = clusterList[i].getTestCases().at(j);
            std::set<IndexType>::iterator it = coveredElementIds.begin();
            while (it != coveredElementIds.end()) {
                IndexType cid = *it;
                if (coverage->getBitMatrix().get(tcid, cid)) {
                    nrOfCoveredCodeElements++;
                    coveredElementIds.erase(it++);
                } else {
                    it++;
                }
            }
        }

        coverageStream << i << ";" << nrOfCodeElements << ";" << nrOfCoveredCodeElements << ";" << ((double)nrOfCoveredCodeElements / nrOfCodeElements) << std::endl;
    }
    coverageStream.close();
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new FaultDetectionMetricPlugin());
}

} /* namespace soda */
