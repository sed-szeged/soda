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

CCoverageMatrix* GcovCoverageReaderPlugin::read(const variables_map &vm)
{
    // prepare filters
    m_fileFilter.clear();
    m_codeElementNameFilter.assign(vm["cut-source-path"].as<String>());
    String filter = vm["filter-input-files"].as<String>();
    boost::char_separator<char> sep(",");
    boost::tokenizer<boost::char_separator<char> > tokens(filter, sep);
    boost::tokenizer<boost::char_separator<char> >::iterator it;
    for (it = tokens.begin(); it != tokens.end(); ++it) {
        m_fileFilter.push_back(boost::regex(*it));
    }

    m_coverage = new CCoverageMatrix();
    readFromDirectoryStructure(vm["path"].as<String>());
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
        throw CException("GcovCoverageReaderPlugin::readFromDirectoryStructure()", "Specified path does not exists or is not a directory.");
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

void GcovCoverageReaderPlugin::readCoverageDataFromFile(fs::path p)
{
    std::ifstream in(p.string().c_str());
    std::string line;
    std::string sourcePath;
    bool firstLine = true;
    bool skip = false;
    boost::char_separator<char> sep(":");

    while (std::getline(in, line)) {
        StringVector rowData;
        boost::tokenizer<boost::char_separator<char> > tokens(line, sep);
        boost::tokenizer<boost::char_separator<char> >::iterator it;
        for (it = tokens.begin(); it != tokens.end(); ++it) {
            std::string tmp = *it;
            boost::algorithm::trim(tmp);
            rowData.push_back(tmp);
        }

        // read source path from first line
        if (firstLine) {
            for (std::vector<boost::regex>::iterator it = m_fileFilter.begin(); it != m_fileFilter.end(); ++it) {
                if (boost::regex_search(rowData[3], *it)) {
                    in.close();
                    return;
                }
            }
            sourcePath = boost::regex_replace(rowData[3], m_codeElementNameFilter, "");
            firstLine = false;
        }

        int lineNumber = boost::lexical_cast<int>(rowData[1]); // executed line number

        // skip gcov file messages
        if (lineNumber == 0)
            continue;
        // skip empty lines
        if (rowData.size() < 3)
            continue;

        boost::algorithm::trim(rowData[2]);
        boost::regex reg("((^(\\{)$)|(^(\\})$)|(^//)|(^#))"); // regex for {, }, # and // chars
        boost::regex openComment("^(\\/\\*)"); // regex for /*
        boost::regex closeComment("(\\*\\/)$"); // regex for */
        if (boost::regex_search(rowData[2], reg)) // skips lines starting with //, #, {, }
            continue;
        if (!skip && boost::regex_search(rowData[2], openComment)) // skips multi line comments
            skip = true;
        if (skip) {
            if (boost::regex_search(rowData[2], closeComment))
                skip = false;
            continue;
        }

        std::stringstream codeElementName;
        codeElementName << sourcePath << ":" << lineNumber;
        m_coverage->addOrSetRelation(m_currentTestcase, codeElementName.str(), isdigit(rowData[0][0]) != 0);
    }
    in.close();
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getCoverageReaderPluginManager().addPlugin(new GcovCoverageReaderPlugin());
}

} /* namespace soda */
