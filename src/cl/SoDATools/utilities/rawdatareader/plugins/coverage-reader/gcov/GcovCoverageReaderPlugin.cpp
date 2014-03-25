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
#include "boost/filesystem.hpp"
#include "CRawDataReaderPluginManager.h"
#include "GcovCoverageReaderPlugin.h"
#include "exception/CException.h"

using namespace std;
using namespace boost::filesystem;

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

void GcovCoverageReaderPlugin::readFromDirectoryStructure(const String& dirname)
{
    readFromDirectoryStructure(dirname.c_str());
}

void GcovCoverageReaderPlugin::readFromDirectoryStructure(const char * dirname)
{
    path coverage_path(dirname);
    if (!(exists(coverage_path) || !is_directory(coverage_path))) {
        throw CException("OneTestPerFileCoverageReaderPlugin::readFromDirectoryStructure()", "Specified path does not exists or is not a directory.");
    }

    readTestcaseNames(coverage_path);
    m_coverage->refitMatrixSize();
    vector<path> pathVector;
    copy(directory_iterator(coverage_path), directory_iterator(), back_inserter(pathVector));
    sort(pathVector.begin(), pathVector.end());
    for (vector<path>::iterator it = pathVector.begin(); it != pathVector.end(); ++it) {
        if (is_directory((*it))) {
            m_currentTestcase = (*it).leaf().string();
            std::cout << "[INFO] Reading coverage data from directory: " << *it << std::endl;
            readCoverage((*it));
        }
    }
}

void GcovCoverageReaderPlugin::readTestcaseNames(path p)
{
    vector<path> pathVector;
    copy(directory_iterator(p), directory_iterator(), back_inserter(pathVector));
    sort(pathVector.begin(), pathVector.end());
    for (vector<path>::iterator it = pathVector.begin(); it != pathVector.end(); ++it) {
        if (is_directory((*it)))
            m_coverage->addTestcaseName((*it).leaf().string());
    }
}

void GcovCoverageReaderPlugin::readCoverage(path p)
{
    vector<path> pathVector;
    copy(directory_iterator(p), directory_iterator(), back_inserter(pathVector));
    sort(pathVector.begin(), pathVector.end());
    for (vector<path>::iterator it = pathVector.begin(); it != pathVector.end(); ++it) {
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

void GcovCoverageReaderPlugin::readCoverageDataFromFile(path p)
{
    std::ifstream in(p.c_str());
    String line;
    String sourcePath;
    bool firstLine = true;
    boost::char_separator<char> sep(":");

    while (getline(in, line)) {
        StringVector data;
        boost::tokenizer<boost::char_separator<char> > tokens(line, sep);
        boost::tokenizer<boost::char_separator<char> >::iterator it;
        for (it = tokens.begin(); it != tokens.end(); ++it) {
            String tmp = *it;
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
            stringstream codeElementName;
            codeElementName << sourcePath << ":" << lineNumber;;
            m_coverage->addOrSetRelation(m_currentTestcase, codeElementName.str());
        } else {
            continue;
        }
    }
    in.close();
}

extern "C" void registerPlugin(CRawDataReaderPluginManager &manager)
{
    manager.addCoverageReaderPlugin(new GcovCoverageReaderPlugin());
}

} /* namespace soda */
