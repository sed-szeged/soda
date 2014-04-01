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
#ifndef OCHIAIFAULTLOCALIZATIONTECHNIQUEPLUGIN_H
#define OCHIAIFAULTLOCALIZATIONTECHNIQUEPLUGIN_H

#include "engine/CKernel.h"

namespace soda {

class OchiaiFaultLocalizationTechniquePlugin : public IFaultLocalizationTechniquePlugin
{
public:
    OchiaiFaultLocalizationTechniquePlugin();
    virtual ~OchiaiFaultLocalizationTechniquePlugin();

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
     * @brief Initialize the plugin.
     * @param data The test suite data.
     * @param revisionList The revisions to consider.
     */
    void init(CSelectionData *data, IndexType revision);

    /**
     * @brief Calculates the metrics of a test suite.
     * @param cluster The cluster to use during the calculation.
     * @param output The path to the output directory.
     */
    void calculate(CClusterDefinition &cluster,  const std::string &output);

    /**
     * @brief Returns the values of fault localization technique for each code element.
     * @return
     */
    FLValues& getValues();

    /**
     * @brief Returns the distributiuon of fault lcoalization technique values.
     * @return
     */
    FLDistribution& getDistribution();

private:
    FLValues       *m_values;
    FLDistribution *m_distribution;
    CSelectionData *m_data;
    IndexType       m_revision;
};

} /* namespace soda */

#endif // OCHIAIFAULTLOCALIZATIONTECHNIQUEPLUGIN_H
