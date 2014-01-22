/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: Tamás Gergely <gertom@inf.u-szeged.hu>
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
#include "datareader/CReadCoverage.h"
#include "boost/tokenizer.hpp"
#include "exception/CException.h"

using namespace std;
using namespace boost::filesystem;

namespace sodatools {

CReadCoverage::CReadCoverage(CCoverageMatrix& matrix, bool has_passfail, bool read_tc, bool read_mt, bool verbose) :
    m_coverage(&matrix),
    m_hasPassFailInfo(has_passfail),
    m_readTestcases(read_tc),
    m_readCodeElements(read_mt),
    m_verbose(verbose)
{
}

CReadCoverage::~CReadCoverage()
{
}

void CReadCoverage::readOneTestPerFileCoverageFromDirectoryStructure(const char * dirname)
{
    path coverage_path(dirname);
    if (!(exists(coverage_path) && is_directory(coverage_path))) {
        throw CException("CReadCoverage::readOneTestPerFileCoverageFromDirectoryStructure()", "Specified path does not exists or is not a directory.");
    }
    size_t cutsize = coverage_path.generic_string().length() + ((*(coverage_path.generic_string().rbegin()) == '/') ? 0 : 1);
    if(m_readTestcases || m_readCodeElements) {
        readOneTestPerFileCoverageFromDirectory1stPass(coverage_path, cutsize);
        m_coverage->refitMatrixSize();
    }
    readOneTestPerFileCoverageFromDirectory(coverage_path, cutsize);
}

void CReadCoverage::readOneTestPerFileCoverageFromDirectoryStructure(const String& dirname)
{
    readOneTestPerFileCoverageFromDirectoryStructure(dirname.c_str());
}

/*****************************************************************************
 * Private parts
 */

static void cutPassFailInfo(String& str) {
    size_t b = str.find("PASS: ");
    size_t l = 6;
    if(b==string::npos) {
        b = str.find("FAIL: ");
    }
    if(b!=string::npos) {
        str.erase(b, l);
    }
}

static void cutExtension(String& str) {
    size_t b = str.find_last_of('.');
    if(b!=string::npos) {
        str.erase(b);
    }
}

void CReadCoverage::readOneTestPerFileCoverageFromDirectory1stPass(path p, size_t cut)
{
    static int info_tcnt=0; //INFO
    static int info_tmax=0;
    int info_cnt=0; //INFO
    int info_max;
    if(m_verbose) { //INFO
        cout << "Directory: " << p << " 1st pass" << endl;
        cout.flush();
    }
    vector<path> pathVector;
    copy(directory_iterator(p), directory_iterator(), back_inserter(pathVector));
    sort(pathVector.begin(), pathVector.end());
    info_tmax+=info_max=pathVector.size();

    for(vector<path>::iterator it = pathVector.begin(); it != pathVector.end(); it++) {
        if(is_directory(*it)) { // recurse into subdirs
            if(basename(*it)!="") {
                readOneTestPerFileCoverageFromDirectory1stPass(*it, cut);
            } else {
                info_tmax--; info_max--;
            }
        } else {
            if(m_verbose) { //INFO
                cout << info_cnt++ << "/" << info_max << ' ' << info_tcnt++ << "/" << info_tmax << '\r';
                cout.flush();
            }
            String tcname   = it->generic_string().substr(cut);
            if(m_hasPassFailInfo) {
                cutPassFailInfo(tcname);
            }
            cutExtension(tcname);
            if(m_readTestcases) {
                m_coverage->addTestcaseName(tcname);
            }
            if(m_readCodeElements) {
                String name = it->string();
                std::ifstream in(name.c_str());
                String line;
                boost::char_separator<char> sep(",");
                StringVector data;
                while (getline(in, line)) {
                    boost::tokenizer<boost::char_separator<char> > tokens(line, sep);
                    boost::tokenizer<boost::char_separator<char> >::iterator it;
                    for ( it = tokens.begin(); it != tokens.end(); ++it) {
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
}

void CReadCoverage::readOneTestPerFileCoverageFromDirectory(path p, size_t cut)
{
    static int info_tcnt=0; //INFO
    static int info_tmax=0;
    int info_cnt=0; //INFO
    int info_max;
    if(m_verbose) { //INFO
        cout << "Directory: " << p << endl;
        cout.flush();
    }
    vector<path> pathVector;
    copy(directory_iterator(p), directory_iterator(), back_inserter(pathVector));
    sort(pathVector.begin(), pathVector.end());
    info_tmax+=info_max=pathVector.size();

    for(vector<path>::iterator it = pathVector.begin(); it != pathVector.end(); it++) {
        if(is_directory(*it)) { // recurse into subdirs
            if(basename(*it)!="") {
                readOneTestPerFileCoverageFromDirectory(*it, cut);
            } else {
                info_tmax--; info_max--;
            }
        } else {
            if(m_verbose) { //INFO
                cout << info_cnt++ << "/" << info_max << ' ' << info_tcnt++ << "/" << info_tmax << '\r';
                cout.flush();
            }
            String tcname   = it->generic_string().substr(cut);
            if(m_hasPassFailInfo) {
                cutPassFailInfo(tcname);
            }
            cutExtension(tcname);

            String name = it->string();
            std::ifstream in(name.c_str());
            String line;
            boost::char_separator<char> sep(",");
            StringVector data;
            while (getline(in, line)) {
                boost::tokenizer<boost::char_separator<char> > tokens(line, sep);
                boost::tokenizer<boost::char_separator<char> >::iterator it;
                for ( it = tokens.begin(); it != tokens.end(); ++it) {
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

} // namespace sodatools
