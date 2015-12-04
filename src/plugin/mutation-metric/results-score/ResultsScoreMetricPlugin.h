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

#ifndef RESULTSSCOREMETRICPLUGIN_H
#define RESULTSSCOREMETRICPLUGIN_H

#include "engine/CKernel.h"

namespace soda {

class ResultsScoreMetricPlugin : public IMutationMetricPlugin
{
public:
    ResultsScoreMetricPlugin();
    virtual ~ResultsScoreMetricPlugin();

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
     * @param args Json document with the specified input arguments.
     * @param tcidList [OPTIONAL] List of test case ids that restricts the computation.
     */
    void init(CSelectionData *data, const rapidjson::Document& args, IntVector *tcidList);

    /**
     * @brief Returns the list of plugins names that the plugin depens on.
     * @return List of dependencies.
     */
    std::vector<std::string> getDependency();

    /**
     * @brief Calculates the metrics of a test suite.
     * @param results Stores the results of the metric plugin for each cluster.
     */
    void calculate(rapidjson::Document &results);

private:
    CSelectionData *data;
    IntVector *tcidList;
};

} /* namespace soda */

#endif /* RESULTSSCOREMETRICPLUGIN_H */
