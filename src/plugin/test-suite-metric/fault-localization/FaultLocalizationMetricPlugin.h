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
#ifndef FAULTLOCALIZATIONMETRICPLUGIN_H
#define FAULTLOCALIZATIONMETRICPLUGIN_H

#include "CPartitionAlgorithm.h"
#include "engine/CKernel.h"

namespace soda {

class FaultLocalizationMetricPlugin : public ITestSuiteMetricPlugin
{
public:
    FaultLocalizationMetricPlugin();
    virtual ~FaultLocalizationMetricPlugin();

    /**
     * @brief Returns the name of the plugin.
     * @return
     */
    std::string getName();
    /**
     * @brief Returns the description of the plugin.
     * @return
     */
    std::string getDescription();

    /**
     * @brief Returns the list of plugins names that the plugin depens on.
     * @return List of dependencies.
     */
    std::vector<std::string> getDependency();

    /**
     * @brief Initialize the plugin.
     * @param data The test suite data.
     * @param clusterList The clusters of test cases and code elements.
     * @param revisionList The revisions to consider.
     */
    void init(CSelectionData *data, std::map<std::string, CClusterDefinition> *clusterList, IndexType revision);


    /**
     * @brief Calculates the metrics of a test suite.
     * @param results Stores the results of the metric plugin for each cluster in JSON format.
     */
    void calculate(rapidjson::Document &results);

private:
    void writePartitions(CPartitionAlgorithm &algorithm, rapidjson::Document &results);
    void partitionStatistics(CPartitionAlgorithm &algorithm, CClusterDefinition &cluster, const std::string& clusterId, rapidjson::Document &results);

private:
    CSelectionData *m_data;
    std::map<std::string, CClusterDefinition> *m_clusterList;
    IndexType m_revision;
};

} /* namespace soda */

#endif /* FAULTLOCALIZATIONMETRICPLUGIN_H */
