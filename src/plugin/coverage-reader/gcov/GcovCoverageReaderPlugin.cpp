/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Havas <havasd@inf.u-szeged.hu>
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

#include "boost/tokenizer.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "exception/CException.h"
#include "GcovCoverageReaderPlugin.h"


namespace soda {

GcovCoverageReaderPlugin::GcovCoverageReaderPlugin() :
    m_coverage(NULL)
{}

GcovCoverageReaderPlugin::~GcovCoverageReaderPlugin()
{}

std::string GcovCoverageReaderPlugin::getName()
{
    return "gcov";
}

std::string GcovCoverageReaderPlugin::getDescription()
{
    return "Reads statement level coverage data from gcov files.";
}

CCoverageMatrix* GcovCoverageReaderPlugin::read(const std::string &path)
{
    m_coverage = new CCoverageMatrix();
    readFromDirectoryStructure(path);
    return m_coverage;
}

void GcovCoverageReaderPlugin::readFromDirectoryStructure(const std::string& dirname)
{
    readFromDirectoryStructure(dirname.c_str());
}

void GcovCoverageReaderPlugin::readFromDirectoryStructure(const char * dirname)
{
    fs::path coverage_path(dirname);
    if (!(exists(coverage_path) || !is_directory(coverage_path))) {
        throw CException("OneTestPerFileCoverageReaderPlugin::readFromDirectoryStructure()", "Specified path does not exists or is not a directory.");
    }

    readTestcaseNames(coverage_path);
    m_coverage->refitMatrixSize();
    std::vector<fs::path> pathVector;
    std::copy(fs::directory_iterator(coverage_path), fs::directory_iterator(), back_inserter(pathVector));
    std::sort(pathVector.begin(), pathVector.end());
    for (std::vector<fs::path>::iterator it = pathVector.begin(); it != pathVector.end(); ++it) {
        if (is_directory((*it))) {
            m_currentTestcase = (*it).leaf().string();
            std::cout << "[INFO] Reading coverage data from directory: " << *it << std::endl;
            readCoverage((*it));
        }
    }
}

void GcovCoverageReaderPlugin::readTestcaseNames(fs::path p)
{
    std::vector<fs::path> pathVector;
    std::copy(fs::directory_iterator(p), fs::directory_iterator(), back_inserter(pathVector));
    std::sort(pathVector.begin(), pathVector.end());
    for (std::vector<fs::path>::iterator it = pathVector.begin(); it != pathVector.end(); ++it) {
        if (is_directory((*it)))
            m_coverage->addTestcaseName((*it).leaf().string());
    }
}

void GcovCoverageReaderPlugin::readCoverage(fs::path p)
{
    std::vector<fs::path> pathVector;
    std::copy(fs::directory_iterator(p), fs::directory_iterator(), back_inserter(pathVector));
    std::sort(pathVector.begin(), pathVector.end());
    for (std::vector<fs::path>::iterator it = pathVector.begin(); it != pathVector.end(); ++it) {
        if (is_directory((*it)))
            readCoverage((*it));
        else if (is_regular_file((*it)) && (*it).extension() == ".gcov")
            readCoverageDataFromFile((*it));
    }
}

static bool isNumber(String& str)
{
    for (int i = 0; i < (int)str.length(); ++i) {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

void GcovCoverageReaderPlugin::readCoverageDataFromFile(fs::path p)
{
    std::ifstream in(p.c_str());
    std::string line;
    std::string sourcePath;
    bool firstLine = true;
    boost::char_separator<char> sep(":");

    while (std::getline(in, line)) {
        StringVector data;
        boost::tokenizer<boost::char_separator<char> > tokens(line, sep);
        boost::tokenizer<boost::char_separator<char> >::iterator it;
        for (it = tokens.begin(); it != tokens.end(); ++it) {
            std::string tmp = *it;
            boost::algorithm::trim(tmp);
            data.push_back(tmp);
        }

        // read source path from first line
        if (firstLine) {
            sourcePath = data[3];
            firstLine = false;
        }

        // data[0] is always non-zero if it's a numer
        if (isNumber(data[0])) {
            int lineNumber = boost::lexical_cast<int>(data[1]); // executed line number
            std::stringstream codeElementName;
            codeElementName << sourcePath << ":" << lineNumber;
            m_coverage->addOrSetRelation(m_currentTestcase, codeElementName.str());
        } else {
            continue;
        }
    }
    in.close();
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getCoverageReaderPluginManager().addPlugin(new GcovCoverageReaderPlugin());
}

} /* namespace soda */
