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

#ifndef PARTITIONMETRICTESTSUITEMETRICPLUGIN_H
#define PARTITIONMETRICTESTSUITEMETRICPLUGIN_H

#include "engine/CKernel.h"

namespace soda {

class PartitionMetricTestSuiteMetricPlugin : public ITestSuiteMetricPlugin
{
public:

    PartitionMetricTestSuiteMetricPlugin();
    ~PartitionMetricTestSuiteMetricPlugin();

    std::string getName();

    std::string getDescription();

    void init(CSelectionData *data, std::map<std::string, CClusterDefinition> *clusterList, IndexType revision);

    std::vector<std::string> getDependency();

    void calculate(rapidjson::Document &results);

private:
    CSelectionData *m_data;
    std::map<std::string, CClusterDefinition> *m_clusterList;
};

} /* namespace soda */

#endif /* PARTITIONMETRICTESTSUITEMETRICPLUGIN_H */
