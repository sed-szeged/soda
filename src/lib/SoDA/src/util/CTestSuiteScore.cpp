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

#include "util/CTestSuiteScore.h"

namespace soda {

double CTestSuiteScore::flScore(CClusterDefinition &cluster, double value, IFaultLocalizationTechniquePlugin::FLDistribution &distribution)
{
    double tau = 0;

    IndexType nrOfCodeElements = cluster.getCodeElements().size();

    IFaultLocalizationTechniquePlugin::FLDistribution::const_reverse_iterator it;

    for (it = distribution.rbegin(); it != distribution.rend(); it++) {
        if (it->first != value) {
            tau += it->second;
        } else {
            tau += tau + it->second - 1;
            break;
        }
    }
    tau /= 2;

    return 1 - (tau / (nrOfCodeElements - 1));
}

double CTestSuiteScore::fdScore(CSelectionData &data, CClusterDefinition &cluster, IndexType revision, IndexType totalNrOfFailedTestcases)
{
    IntVector testCaseIds = cluster.getTestCases();
    IndexType nOfTestcases = testCaseIds.size();

    double nrOfFailedAndCovered = 0.0;
    for (IndexType i = 0; i < nOfTestcases; i++) {
        IndexType tcid = testCaseIds[i];
        IndexType tcidInResults = data.translateTestcaseIdFromCoverageToResults(tcid);
        if (data.getResults()->getExecutionBitList(revision).at(tcidInResults) &&
                !data.getResults()->getPassedBitList(revision).at(tcidInResults)) {
            nrOfFailedAndCovered++;
        }
    }

    if (totalNrOfFailedTestcases == 0) {
        totalNrOfFailedTestcases = 1;
    }

    return nrOfFailedAndCovered / totalNrOfFailedTestcases;

}

} /* namespace soda */
