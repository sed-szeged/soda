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
#include <cmath>
#include <cstdlib>
#include <fstream>

#include "boost/lexical_cast.hpp"

#include "RandomTestSuiteClusterPlugin.h"

namespace soda {

int randomCluster (int i) { return std::rand()%i;}

RandomTestSuiteClusterPlugin::RandomTestSuiteClusterPlugin() :
    m_sizes(std::vector<IndexType>())
{
}

RandomTestSuiteClusterPlugin::~RandomTestSuiteClusterPlugin()
{
}

std::string RandomTestSuiteClusterPlugin::getName()
{
    return "random";
}

std::string RandomTestSuiteClusterPlugin::getDescription()
{
    return "Generates random test groups with different sizes given in the percentage of test cases.";
}

std::map<String, String> RandomTestSuiteClusterPlugin::getRequiredParameters()
{
    std::map<String, String> requiredParams;
    requiredParams["cluster-sizes"] = TYPE_INTARRAY;
    return requiredParams;
}

void RandomTestSuiteClusterPlugin::init(rapidjson::Document &doc)
{
    m_sizes.clear();
    for (rapidjson::Value::ConstValueIterator it = doc["cluster-sizes"].Begin(); it != doc["cluster-sizes"].End(); it++) {
        m_sizes.push_back(it->GetInt());
    }
}

void RandomTestSuiteClusterPlugin::execute(CSelectionData &data, std::map<std::string, CClusterDefinition>& clusterList)
{
    std::srand (unsigned(std::time(NULL)));

    std::vector<IndexType> testCaseIds(data.getCoverage()->getTestcases().getIDList());

    for (std::vector<IndexType>::iterator it = m_sizes.begin(); it != m_sizes.end(); it++) {
        IndexType size = std::floor(testCaseIds.size() * ((double)*it / 100));
        if (size > testCaseIds.size()) {
            // TODO Handle this error.
            std::cerr << "Invalid size." << std::endl;
            continue;
        }
        std::random_shuffle(testCaseIds.begin(), testCaseIds.end(), randomCluster);
        std::string clusterName = boost::lexical_cast<std::string>(*it);
        for (IndexType i = 0; i < size; i++) {
            clusterList[clusterName].addTestCase(testCaseIds[i]);
        }
        clusterList[clusterName].addCodeElements(data.getCoverage()->getCodeElements().getIDList());
    }


}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteClusterPluginManager().addPlugin(new RandomTestSuiteClusterPlugin());
}

} /* namespace soda */
