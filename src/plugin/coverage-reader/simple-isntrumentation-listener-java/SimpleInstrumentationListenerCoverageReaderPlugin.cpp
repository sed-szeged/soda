/*
 * Copyright (C): 2013-2015 Department of Software Engineering, University of Szeged
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
#include <fstream>
#include <ctype.h>

#include "boost/algorithm/string_regex.hpp"
#include "boost/regex.hpp"
#include "boost/lexical_cast.hpp"
#include "exception/CException.h"
#include "SimpleInstrumentationListenerCoverageReaderPlugin.h"

namespace pt = boost::property_tree;

namespace soda {

SimpleInstrumentationListenerJavaCoverageReaderPlugin::SimpleInstrumentationListenerJavaCoverageReaderPlugin() :
    coverage(nullptr), mutationCoverage(nullptr)
{}

SimpleInstrumentationListenerJavaCoverageReaderPlugin::~SimpleInstrumentationListenerJavaCoverageReaderPlugin()
{}

std::string SimpleInstrumentationListenerJavaCoverageReaderPlugin::getName()
{
    return "simple-instrumentation-listener-java";
}

std::string SimpleInstrumentationListenerJavaCoverageReaderPlugin::getDescription()
{
    return "Reads coverage from SimpleInstrumentationListener output.";
}

CCoverageMatrix* SimpleInstrumentationListenerJavaCoverageReaderPlugin::read(const variables_map &vm)
{
    coverage = new CCoverageMatrix();
    codeElements = vm["list-code-elements"].as<String>();
    outputPath = vm["output"].as<String>();
    outputPath.append(".mut");
    readFromFile(vm["path"].as<String>());
    return coverage;
}

void SimpleInstrumentationListenerJavaCoverageReaderPlugin::readFromFile(String const &file)
{
    CCoverageMatrix *mutationM = new CCoverageMatrix();
    fs::path coverage_path(file);
    if (!(exists(coverage_path) && is_regular_file(coverage_path))) {
        throw CException("SimpleInstrumentationListenerJavaCoverageReaderPlugin::readFromDirectoryStructure()", "Specified path does not exists or is not a file.");
    }

    std::ifstream in(file);
    String line;
    while (std::getline(in, line)) {
        StringVector data;
        // Input lines contains test name:code element naem pairs with a ':' separator.
        data.push_back(line.substr(0, line.find(":{")));
        data.push_back(line.substr(line.find(":{") + 1));
        if (data[1].find("mutation") != String::npos) {
            mutationM->addOrSetRelation(data[0], data[1], true);
        }
        else {
            coverage->addOrSetRelation(data[0], data[1], true);
        }
    }
    in.close();

    if (!codeElements.empty()) {
        std::ifstream codeElementsList(codeElements);
        while (std::getline(codeElementsList, line)) {
            if (line.find("mutation") != String::npos) {
                mutationM->addCodeElementName(line);
            }
            else {
                coverage->addCodeElementName(line);
            }
            coverage->refitMatrixSize();
            mutationM->refitMatrixSize();
        }
    }

    if (mutationM->getTestcases().size()) {
        mutationM->save(outputPath);
    }
    delete mutationM;
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getCoverageReaderPluginManager().addPlugin(new SimpleInstrumentationListenerJavaCoverageReaderPlugin());
}

} /* namespace soda */
