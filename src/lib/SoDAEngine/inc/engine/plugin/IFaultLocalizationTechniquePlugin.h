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

#ifndef IFAULTLOCALIZATIONTECHNIQUEPLUGIN_H
#define IFAULTLOCALIZATIONTECHNIQUEPLUGIN_H

#include <map>
#include <string>
#include <vector>

#include "data/CClusterDefinition.h"
#include "data/CSelectionData.h"
#include "data/SoDALibDefs.h"
#include "rapidjson/document.h"

namespace soda {

/**
 * @brief Interface of the fault localization technique plugins.
 */
class IFaultLocalizationTechniquePlugin
{
public:
    /**
     * @brief The distribution of the fault localization technique values.
     *        The key is the value and the value is the number of the
     *        code elements with that fault localization technique value.
     */
    typedef std::map<double, IndexType> FLDistribution;

    // cluster_id->cid->score
    typedef std::map<String, std::map<IndexType, double> > FLScore;

public:

    /**
     * @brief Virtual destructor.
     */
    virtual ~IFaultLocalizationTechniquePlugin() {}

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
    * @brief Returns the list of plugins names that the plugin depens on.
    * @return List of dependencies.
    */
    virtual std::vector<String> getDependency() = 0;

    /**
     * @brief Initialize the plugin.
     * @param data The test suite data.
     * @param clusters The cluster to process
     * @param revision The revision to process
     * @param failedCodeElements The id of the code elements in the coverage matrix that contains bug.
     */
    virtual void init(CSelectionData *data, ClusterMap *clusters, IndexType revision, IntVector failedCodeElements) = 0;

    /**
     * @brief Returns the distributiuon of fault lcoalization technique values.
     * @return
     */
    virtual FLDistribution& getDistribution() = 0;

    /**
     * @brief Calculates the score values for each code element in the clusters.
     * @param res Rapidjson document which contains the calculated values.
     */
    virtual void calculate(rapidjson::Document &res) = 0;

    /**
     * @brief Returns the fl score of all the elements.
     * @return
     */
    virtual FLScore& getFlScore() = 0;
};

} /* namespace soda */

#endif /* IFAULTLOCALIZATIONTECHNIQUEPLUGIN_H */
