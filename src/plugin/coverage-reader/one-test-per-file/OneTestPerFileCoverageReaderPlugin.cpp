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
#include <fstream>

#include "boost/tokenizer.hpp"
#include "exception/CException.h"
#include "OneTestPerFileCoverageReaderPlugin.h"

namespace soda {

OneTestPerFileCoverageReaderPlugin::OneTestPerFileCoverageReaderPlugin() :
    m_coverage(NULL)
{}

OneTestPerFileCoverageReaderPlugin::~OneTestPerFileCoverageReaderPlugin()
{}

std::string OneTestPerFileCoverageReaderPlugin::getName()
{
    return "one-test-per-file";
}

std::string OneTestPerFileCoverageReaderPlugin::getDescription()
{
    return "The coverage information of a test case is in one file and the file name is the test case name.";
}

CCoverageMatrix* OneTestPerFileCoverageReaderPlugin::read(const variables_map &vm)
{
    m_coverage = new CCoverageMatrix();
    readFromDirectoryStructure(vm["path"].as<String>());
    return m_coverage;
}

static void cutPassFailInfo(String& str)
{
    size_t b = str.find("PASS: ");
    size_t l = 6;
    if (b == std::string::npos) {
        b = str.find("FAIL: ");
    }
    if (b != std::string::npos) {
        str.erase(b, l);
    }
}

static void cutExtension(std::string& str)
{
    size_t b = str.find_last_of('.');
    if (b != std::string::npos) {
        str.erase(b);
    }
}

void OneTestPerFileCoverageReaderPlugin::readFromDirectoryStructure(const char * dirname)
{
    fs::path coverage_path(dirname);
    if (!(exists(coverage_path) && is_directory(coverage_path))) {
        throw CException("OneTestPerFileCoverageReaderPlugin::readFromDirectoryStructure()", "Specified path does not exists or is not a directory.");
    }

    size_t cutsize = coverage_path.generic_string().length() + ((*(coverage_path.generic_string().rbegin()) == '/') ? 0 : 1);
    readFromDirectory1stPass(coverage_path, cutsize);
    m_coverage->refitMatrixSize();
    readFromDirectory(coverage_path, cutsize);
}

void OneTestPerFileCoverageReaderPlugin::readFromDirectoryStructure(const std::string& dirname)
{
    readFromDirectoryStructure(dirname.c_str());
}

void OneTestPerFileCoverageReaderPlugin::readFromDirectory1stPass(fs::path p, size_t cut)
{
    static int info_tcnt = 0; //INFO
    static int info_tmax = 0;
    int info_cnt = 0; //INFO
    int info_max;

    std::cout << "Directory: " << p << " 1st pass" << std::endl;
    std::cout.flush();

    std::vector<fs::path> pathVector;
    std::copy(fs::directory_iterator(p), fs::directory_iterator(), back_inserter(pathVector));
    std::sort(pathVector.begin(), pathVector.end());
    info_tmax += info_max = pathVector.size();

    for (std::vector<fs::path>::iterator it = pathVector.begin(); it != pathVector.end(); it++) {
        if (is_directory(*it)) { // recurse into subdirs
            if (basename(*it) != "") {
                readFromDirectory1stPass(*it, cut);
            } else {
                info_tmax--;
                info_max--;
            }
        } else {
            std::cout << info_cnt++ << "/" << info_max << ' ' << info_tcnt++ << "/" << info_tmax << '\r';
            std::cout.flush();

            std::string tcname = it->generic_string().substr(cut);
            cutPassFailInfo(tcname);
            cutExtension(tcname);
            m_coverage->addTestcaseName(tcname);

            std::ifstream in(it->c_str());
            std::string line;
            boost::char_separator<char> sep(",");
            StringVector data;
            while (std::getline(in, line)) {
                boost::tokenizer<boost::char_separator<char> > tokens(line, sep);
                boost::tokenizer<boost::char_separator<char> >::iterator it;
                for (it = tokens.begin(); it != tokens.end(); ++it) {
                    data.push_back(*it);
                }
                if (data.size() == 2u) {
                    m_coverage->addCodeElementName(data[0]);
                }
                data.clear();
            }
            in.close();
        }
    }
}

void OneTestPerFileCoverageReaderPlugin::readFromDirectory(fs::path p, size_t cut)
{
    static int info_tcnt = 0; //INFO
    static int info_tmax = 0;
    int info_cnt = 0; //INFO
    int info_max;

    std::cout << "Directory: " << p << std::endl;
    std::cout.flush();

    std::vector<fs::path> pathVector;
    std::copy(fs::directory_iterator(p), fs::directory_iterator(), back_inserter(pathVector));
    std::sort(pathVector.begin(), pathVector.end());
    info_tmax+=info_max=pathVector.size();

    for (std::vector<fs::path>::iterator it = pathVector.begin(); it != pathVector.end(); it++) {
        if (is_directory(*it)) { // recurse into subdirs
            if (basename(*it) != "") {
                readFromDirectory(*it, cut);
            } else {
                info_tmax--; info_max--;
            }
        } else {
            std::cout << info_cnt++ << "/" << info_max << ' ' << info_tcnt++ << "/" << info_tmax << '\r';
            std::cout.flush();

            std::string tcname = it->generic_string().substr(cut);
            cutPassFailInfo(tcname);
            cutExtension(tcname);

            std::ifstream in(it->c_str());
            std::string line;
            boost::char_separator<char> sep(",");
            StringVector data;
            while (std::getline(in, line)) {
                boost::tokenizer<boost::char_separator<char> > tokens(line, sep);
                boost::tokenizer<boost::char_separator<char> >::iterator it;
                for (it = tokens.begin(); it != tokens.end(); ++it) {
                    data.push_back(*it);
                }
                if (data.size() == 2u) {
                    m_coverage->setRelation(tcname, data[0], true);
                }
                data.clear();
            }
            in.close();
        }
    }
}

extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getCoverageReaderPluginManager().addPlugin(new OneTestPerFileCoverageReaderPlugin());
}

} /* namespace soda */
