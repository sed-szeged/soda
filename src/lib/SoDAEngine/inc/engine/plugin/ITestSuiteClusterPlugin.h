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

#ifndef ICLUSTERPLUGIN_H
#define ICLUSTERPLUGIN_H

#include <map>
#include <string>

#include "data/CClusterDefinition.h"
#include "data/CSelectionData.h"
#include "rapidjson/document.h"

namespace soda {

/**
 * @brief Interface of the cluster plugins.
 */
class ITestSuiteClusterPlugin
{
public:

    /**
     * @brief Virtual destructor.
     */
    virtual ~ITestSuiteClusterPlugin() {}

    /**
     * @brief Returns the name of the plugin.
     * @return
     */
    virtual std::string getName() = 0;

    /**
     * @brief Returns the description of the plugin.
     * @return
     */
    virtual std::string getDescription() = 0;

    /**
     * @brief Returns the required parameters for the plugin.
     * @return Map of name and type pairs.
     */
    std::map<String, String> getRequiredParameters();

    /**
     * @brief Initializes the plugin.
     * @param reader To read external options.
     */
    virtual void init(rapidjson::Document &doc) = 0;

    /**
     * @brief Runs the clustering algorithm.
     * @param [IN] data The test suite data.
     * @param [OUT] clusterList The list of clusters.
     */
    virtual void execute(CSelectionData &data, std::map<std::string, CClusterDefinition>& clusterList) = 0;
};

} /* namespace soda */

#endif /* ICLUSTERPLUGIN_H */
