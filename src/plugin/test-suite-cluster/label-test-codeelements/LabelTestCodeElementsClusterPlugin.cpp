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
#include <fstream>
#include <vector>

#include "boost/algorithm/string/classification.hpp"
#include "boost/algorithm/string/split.hpp"

#include "LabelTestCodeElementsClusterPlugin.h"

namespace soda {

LabelTestCodeElementsClusterPlugin::LabelTestCodeElementsClusterPlugin() :
    m_testList(std::string()),
    m_codeElementList(std::string())
{
}


LabelTestCodeElementsClusterPlugin::~LabelTestCodeElementsClusterPlugin()
{

}

std::string LabelTestCodeElementsClusterPlugin::getName()
{
    return "label-test-codeelements";
}

std::string LabelTestCodeElementsClusterPlugin::getDescription()
{
    return "Loads the clusters from files and combine them.";
}

std::map<String, String> LabelTestCodeElementsClusterPlugin::getRequiredParameters()
{
    std::map<String, String> requiredParams;
    requiredParams["cluster-test-list"] = TYPE_PATH;
    requiredParams["cluster-code-elements-list"] = TYPE_PATH;
    return requiredParams;
}

void LabelTestCodeElementsClusterPlugin::init(rapidjson::Document &doc)
{
    m_testList = doc["cluster-test-list"].GetString();
    m_codeElementList = doc["cluster-code-elements-list"].GetString();
}

void LabelTestCodeElementsClusterPlugin::execute(CSelectionData &data, std::map<std::string, CClusterDefinition> &clusterList)
{
    // Read the test cases and their label
    std::ifstream testList;
    testList.open(m_testList.c_str());
    std::string line;

    std::multimap<std::string, IndexType> tests;
    std::multimap<std::string, IndexType> codeElements;


    while (std::getline(testList, line)) {
        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::algorithm::is_any_of(";"));
        IndexType tcid = data.getCoverage()->getTestcases().getID(parts[0]);
        tests.insert(std::pair<std::string, IndexType>(parts[1], tcid));
    }

    testList.close();

    std::ifstream codeElementList;
    codeElementList.open(m_codeElementList.c_str());
    while (std::getline(codeElementList, line)) {
        std::vector<std::string> parts;
        boost::algorithm::split(parts, line, boost::algorithm::is_any_of(";"));
        IndexType cid = data.getCoverage()->getCodeElements().getID(parts[0]);
        codeElements.insert(std::pair<std::string, IndexType>(parts[2], cid));
    }

    codeElementList.close();

    // For each test cluster
    for(std::multimap<std::string, IndexType>::iterator testIt = tests.begin(), testEnd = tests.end();
            testIt != testEnd;
                testIt = tests.upper_bound(testIt->first)) {

        // For each code element cluster
        for(std::multimap<std::string, IndexType>::iterator codeElementIt = codeElements.begin(), codeElementEnd = codeElements.end();
                codeElementIt != codeElementEnd;
                    codeElementIt = codeElements.upper_bound(codeElementIt->first)) {

            std::string cluster = testIt->first + " - " + codeElementIt->first;
            CClusterDefinition def;

            std::pair<std::multimap<std::string, IndexType>::iterator, std::multimap<std::string, IndexType>::iterator> pair;
            // Fill the test cases
            pair = tests.equal_range(testIt->first);
            for (std::multimap<std::string, IndexType>::iterator it = pair.first; it != pair.second; it++) {
                def.addTestCase(it->second);
            }
            // Fill the code elements
            pair = codeElements.equal_range(codeElementIt->first);
            for (std::multimap<std::string, IndexType>::iterator it = pair.first; it != pair.second; it++) {
                def.addCodeElement(it->second);
            }

            clusterList[cluster] = def;
        }
    }
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteClusterPluginManager().addPlugin(new LabelTestCodeElementsClusterPlugin());
}

} /* namespace soda */


