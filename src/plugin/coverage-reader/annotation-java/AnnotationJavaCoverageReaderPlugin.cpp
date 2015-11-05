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
#include "AnnotationJavaCoverageReaderPlugin.h"

namespace pt = boost::property_tree;

namespace soda {

AnnotationJavaCoverageReaderPlugin::AnnotationJavaCoverageReaderPlugin() :
    coverage(NULL)
{}

AnnotationJavaCoverageReaderPlugin::~AnnotationJavaCoverageReaderPlugin()
{}

std::string AnnotationJavaCoverageReaderPlugin::getName()
{
    return "annotation-java";
}

std::string AnnotationJavaCoverageReaderPlugin::getDescription()
{
    return "Reads coverage from SimpleInstrumentationListener output.";
}

CCoverageMatrix* AnnotationJavaCoverageReaderPlugin::read(const variables_map &vm)
{
    coverage = new CCoverageMatrix();
    readFromFile(vm["path"].as<String>());
    annotations = vm["list-annot"].as<String>();
    return coverage;
}

void AnnotationJavaCoverageReaderPlugin::readFromFile(String const &file)
{
    CCoverageMatrix *mutationM = new CCoverageMatrix();
    fs::path coverage_path(file);
    if (!(exists(coverage_path) && is_regular_file(coverage_path))) {
        throw CException("AnnotationJavaCoverageReaderPlugin::readFromDirectoryStructure()", "Specified path does not exists or is not a file.");
    }

    std::ifstream in(file);
    String line;
    while (std::getline(in, line)) {
        StringVector data;
        
        data.push_back(line.substr(0, line.find_first_of(":")));
        data.push_back(line.substr(line.find_first_of(":") + 1));
        if (data[1].find("mutation") != String::npos) {
            mutationM->addOrSetRelation(data[0], data[1], true);
        }
        else {
            coverage->addOrSetRelation(data[0], data[1], true);
        }
    }
    in.close();

    std::ifstream annots(annotations);
    while (std::getline(annots, line)) {
        if (line.find("mutation") != String::npos) {
            mutationM->addCodeElementName(line);
        }
        else {
            coverage->addCodeElementName(line);
        }
        coverage->refitMatrixSize();
        mutationM->refitMatrixSize();
    }

    mutationM->save("mutation.cov.SoDA");
    delete mutationM;
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getCoverageReaderPluginManager().addPlugin(new AnnotationJavaCoverageReaderPlugin());
}

} /* namespace soda */
