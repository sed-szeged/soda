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
#ifndef CTESTSUITESCORE_H
#define CTESTSUITESCORE_H

#include "engine/CKernel.h"

namespace soda {

class CTestSuiteScore
{
public:
    /**
     * @brief Calculates the fault localization score.
     * @param cluster The cluster inder measurement.
     * @param value The fault localization technique value of a code element.
     * @param distribution The distribution of the values of the fault localization technique in this cluster.
     * @return The fault localization score.
     */
    static double flScore(CClusterDefinition &cluster, double value, IFaultLocalizationTechniquePlugin::FLDistribution &distribution);

    /**
     * @brief Calculates the fault detection score of a cluster.
     * @param data The input data.
     * @param cluster The cluster.
     * @param revision The revision we are working with.
     * @param totalNrOfFailedTestcases The total number of failing testcases in the whole test suite (without any reduction or selection and clusterization).
     * @return The fault detection score.
     */
    static double fdScore(CSelectionData &data, CClusterDefinition &cluster, IndexType revision, IndexType totalNrOfFailedTestcases);
};

} /* namespace soda */

#endif /* CTESTSUITESCORE_H */
