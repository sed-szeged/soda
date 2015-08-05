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
#ifndef COMMONFAULTLOCALIZATIONTECHNIQUEPLUGIN_H
#define COMMONFAULTLOCALIZATIONTECHNIQUEPLUGIN_H

#include "engine/CKernel.h"

namespace soda {

class CommonFaultLocalizationTechniquePlugin : public IFaultLocalizationTechniquePlugin
{
public:
    CommonFaultLocalizationTechniquePlugin();
    virtual ~CommonFaultLocalizationTechniquePlugin();

    /**
     * @brief Returns the name of the plugin.
     * @return
     */
    String getName();

    /**
     * @brief Returns the description of the plugin.
     * @return
     */
     String getDescription();

     /**
     * @brief Returns the list of plugins names that the plugin depens on.
     * @return List of dependencies.
     */
     StringVector getDependency();

    /**
     * @brief Initialize the plugin.
     * @param data The test suite data.
     * @param clusters List of clusters.
     * @param revisionList The revisions to consider.
     */
    void init(CSelectionData *data, ClusterMap *clusters, IndexType revision);

    /**
     * @brief Calculates the score values for each code element in clusters.
     * @param cluster The cluster to use during the calculation.
     * @param output The path to the output directory.
     */
    void calculate(rapidjson::Document &res);

    /**
     * @brief Returns the distributiuon of fault lcoalization technique values.
     * @return
     */
    FLDistribution& getDistribution();

private:
    FLDistribution distribution;
    CSelectionData *m_data;
    ClusterMap *clusterList;
    IndexType       m_revision;
};

} /* namespace soda */

#endif // COMMONFAULTLOCALIZATIONTECHNIQUEPLUGIN_H
