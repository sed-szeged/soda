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

#include <iostream>

#include "OneClusterPlugin.h"
#include "CTestSuiteMetricsPluginManager.h"


namespace soda {

OneClusterPlugin::OneClusterPlugin()
{

}

OneClusterPlugin::~OneClusterPlugin()
{

}

std::string OneClusterPlugin::getName()
{
    return "one-cluster";
}

std::string OneClusterPlugin::getDescription()
{
    return "Each test case and code element belong to one cluster.";
}

void OneClusterPlugin::execute(CSelectionData &data, std::vector<CClusterDefinition> &clusterList)
{    
    CClusterDefinition cluster;
    cluster.addTestCases(data.getCoverage()->getTestcases().getIDList());
    cluster.addCodeElements(data.getCoverage()->getCodeElements().getIDList());

    clusterList.push_back(cluster);
}

extern "C" void registerPlugin(CTestSuiteMetricsPluginManager &manager)
{
    manager.addClusterPlugin(new OneClusterPlugin());
}

} /* namespace soda */
