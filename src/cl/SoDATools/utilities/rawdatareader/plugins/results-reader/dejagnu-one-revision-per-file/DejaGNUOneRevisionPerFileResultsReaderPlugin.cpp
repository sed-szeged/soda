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

#include "boost/lexical_cast.hpp"
#include "boost/regex.hpp"
#include "boost/tokenizer.hpp"
#include "CRawDataReaderPluginManager.h"
#include "DejaGNUOneRevisionPerFileResultsReaderPlugin.h"
#include "exception/CException.h"

using namespace std;
using namespace boost::filesystem;

namespace soda {

DejaGNUOneRevisionPerFileResultsReaderPlugin::DejaGNUOneRevisionPerFileResultsReaderPlugin() :
    m_results(NULL)
{}

DejaGNUOneRevisionPerFileResultsReaderPlugin::~DejaGNUOneRevisionPerFileResultsReaderPlugin()
{}

std::string DejaGNUOneRevisionPerFileResultsReaderPlugin::getName()
{
    return "dejagnu-one-revision-per-file";
}

std::string DejaGNUOneRevisionPerFileResultsReaderPlugin::getDescription()
{
    return "The results information of a revision in one file each line stores the result and the name of the test case. Lines starts with X(PASS: |FAIL: |UNSUPPORTED: ) tag followed by the test case name. The filename is used as revision number.";
}

CResultsMatrix* DejaGNUOneRevisionPerFileResultsReaderPlugin::read(const std::string &path)
{
    m_results = new CResultsMatrix();
    readFromDirectoryStructure(path);
    return m_results;
}

static bool startsWith(const char *str, const char *beg)
{
    while(*beg && *str) {
        if(*(str++)!=*(beg++)) {
            return false;
        }
    }
    return !(*beg);
}

void DejaGNUOneRevisionPerFileResultsReaderPlugin::readFromDirectoryStructure(const char * dirname)
{
    path coverage_path(dirname);
    if (!(exists(coverage_path) && is_directory(coverage_path))) {
        throw CException("DejaGNUOneRevisionPerFileResultsReaderPlugin::readFromDirectoryStructure()", "Specified path does not exists or is not a directory.");
    }

    readFromDirectory1stPass(coverage_path);
    m_results->refitMatrixSize();
    readFromDirectory(coverage_path);
}

void DejaGNUOneRevisionPerFileResultsReaderPlugin::readFromDirectoryStructure(const String& dirname)
{
    readFromDirectoryStructure(dirname.c_str());
}

void DejaGNUOneRevisionPerFileResultsReaderPlugin::readFromDirectory1stPass(path p)
{
    int info_cnt = 0; //INFO
    int info_max;

    cout << "Directory: " << p << " 1st pass" << endl;
    cout.flush();

    vector<path> pathVector;
    copy(directory_iterator(p), directory_iterator(), back_inserter(pathVector));
    sort(pathVector.begin(), pathVector.end());
    info_max = pathVector.size();

    for (vector<path>::iterator it = pathVector.begin(); it != pathVector.end(); it++) {
        if (is_directory(*it)) { // recurse into subdirs
            if (basename(*it) != "")
                readFromDirectory1stPass(*it);
        } else {
            cout << info_cnt++ << "/" << info_max << '\r';
            cout.flush();

            m_results->addRevisionNumber(boost::lexical_cast<int>(it->parent_path().filename().string()));

            ifstream in(it->c_str());
            String buffer;
            while (getline(in, buffer)) {
                const char *i = buffer.c_str();
                if (*i == 'X')
                    i++;
                if (startsWith(i, "PASS: ")) {
                    i += 6;
                    m_results->addTestcaseName(i);
                } else if (startsWith(i, "FAIL: ")) {
                    i += 6;
                    m_results->addTestcaseName(i);
                } else if (startsWith(i, "UNSUPPORTED: ")) {
                    i += 13;
                    m_results->addTestcaseName(i);
                }
            }
            in.close();
        }
    }
}

void DejaGNUOneRevisionPerFileResultsReaderPlugin::readFromDirectory(path p)
{
    int info_cnt = 0; //INFO
    int info_max;

    cout << "Directory: " << p << endl;
    cout.flush();

    vector<path> pathVector;
    copy(directory_iterator(p), directory_iterator(), back_inserter(pathVector));
    sort(pathVector.begin(), pathVector.end());
    info_max = pathVector.size();

    for (vector<path>::iterator it = pathVector.begin(); it != pathVector.end(); it++) {
        if (is_directory(*it)) { // recurse into subdirs
            if (basename(*it) != "")
                readFromDirectory(*it);
        } else {
            cout << info_cnt++ << "/" << info_max << '\r';
            cout.flush();

            int revision = boost::lexical_cast<int>(it->parent_path().filename().string());
            ifstream in(it->c_str());
            String buffer;
            while (getline(in, buffer)) {
                const char *i = buffer.c_str();
                if (*i == 'X')
                    i++;
                if (startsWith(i, "PASS: ")) {
                    i += 6;
                    m_results->setResult(revision, i, CResultsMatrix::trtPassed);
                } else if (startsWith(i, "FAIL: ")) {
                    i += 6;
                    m_results->setResult(revision, i, CResultsMatrix::trtFailed);
                } else if(startsWith(i, "UNSUPPORTED: ")) {
                    i += 13;
                    m_results->setResult(revision, i, CResultsMatrix::trtNotExecuted);
                }
            }
            in.close();
        }
    }
}

extern "C" void registerPlugin(CRawDataReaderPluginManager &manager)
{
    manager.addResultsReaderPlugin(new DejaGNUOneRevisionPerFileResultsReaderPlugin());
}

} /* namespace soda */
