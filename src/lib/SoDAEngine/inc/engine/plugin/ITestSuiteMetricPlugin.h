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

#ifndef IMETRICPLUGIN_H
#define IMETRICPLUGIN_H

#include <map>
#include <string>
#include <vector>

#include "data/CClusterDefinition.h"
#include "data/CSelectionData.h"
#include "data/SoDALibDefs.h"

#include "rapidjson/document.h"

namespace soda {

/**
 * @brief Interface of the metric plugins.
 */
class ITestSuiteMetricPlugin
{
public:

    /**
     * @brief Virtual destructor.
     */
    virtual ~ITestSuiteMetricPlugin() {}

    /**
     * @brief Returns the name of the plugin.
     * @return
     */
    virtual String getName() = 0;

    /**
     * @brief Returns the description of the plugin.
     * @return
     */
    virtual String getDescription() = 0;

    /**
     * @brief Initialize the plugin.
     * @param data The test suite data.
     * @param clusterList The clusters of test cases and code elements.
     * @param revisionList The revisions to consider.
     */
    virtual void init(CSelectionData *data, std::map<String, CClusterDefinition> *clusterList, IndexType revision) = 0;

    /**
     * @brief Returns the list of plugins names that the plugin depens on.
     * @return List of dependencies.
     */
    virtual std::vector<String> getDependency() = 0;

    /**
     * @brief Calculates the metrics of a test suite.
     * @param results Stores the results of the metric plugin for each cluster in JSON format.
     */
    virtual void calculate(rapidjson::Document& results) = 0;
};

} /* namespace soda */

#endif /* IMETRICPLUGIN_H */
