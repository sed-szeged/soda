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
#include "datareader/CReadResults.h"
#include "boost/lexical_cast.hpp"
#include "boost/regex.hpp"
#include "boost/tokenizer.hpp"
#include "exception/CException.h"

using namespace std;
using namespace boost::filesystem;

namespace sodatools {

static bool startsWith(const char *str, const char *beg) {
    while(*beg && *str) {
        if(*(str++)!=*(beg++)) {
            return false;
        }
    }
    return !(*beg);
}

CReadResults::CReadResults(CResultsMatrix* matrix, bool read_rv, bool read_tc, bool verbose) :
    m_results(matrix),
    m_readRevisions(read_rv),
    m_readTestcases(read_tc),
    m_verbose(verbose)
{
}

CReadResults::~CReadResults()
{
}

void CReadResults::readOneTestPerFileResultsFromDirectoryStructure(const char * dirname) {
    path coverage_path(dirname);
    if (!(exists(coverage_path) && is_directory(coverage_path))) {
        throw CException("CReadResults::readOneTestPerFileResultsFromDirectoryStructure()", "Specified path does not exists or is not a directory.");
    }
    if(m_readRevisions || m_readTestcases) {
        readOneTestPerFileResultsFromDirectory1stPass(coverage_path);
        m_results->refitMatrixSize();
    }
    readOneTestPerFileResultsFromDirectory(coverage_path);
}

void CReadResults::readOneTestPerFileResultsFromDirectoryStructure(const String& dirname) {
    readOneTestPerFileResultsFromDirectoryStructure(dirname.c_str());
}

/*****************************************************************************
 * Private parts
 */

void CReadResults::readOneTestPerFileResultsFromDirectory1stPass(path p)
{
    int info_cnt=0; //INFO
    int info_max;
    if(m_verbose) { //INFO
        cout << "Directory: " << p << " 1st pass" << endl;
        cout.flush();
    }
    vector<path> pathVector;
    copy(directory_iterator(p), directory_iterator(), back_inserter(pathVector));
    sort(pathVector.begin(), pathVector.end());
    info_max=pathVector.size();

    for(vector<path>::iterator it = pathVector.begin(); it != pathVector.end(); it++) {
        if(is_directory(*it)) { // recurse into subdirs
            if(basename(*it)!="")
                readOneTestPerFileResultsFromDirectory1stPass(*it);
        } else {
            if(m_verbose) { //INFO
                cout << info_cnt++ << "/" << info_max << '\r';
                cout.flush();
            }
            if(m_readRevisions) {
                m_results->addRevisionNumber(boost::lexical_cast<int>(it->parent_path().filename().string()));
            }
            if(m_readTestcases) {
                std::ifstream *in = new std::ifstream(it->c_str());
                String buffer;
                getline(*in, buffer);
                while(!in->eof()) {
                    const char *i=buffer.c_str();
                    if(*i=='X') i++;
                    if(startsWith(i, "PASS: ")) {
                        i+=6;
                        m_results->addTestcaseName(i);
                    } else if(startsWith(i, "FAIL: ")) {
                        i+=6;
                        m_results->addTestcaseName(i);
                    } else if(startsWith(i, "UNSUPPORTED: ")) {
                        i+=13;
                        m_results->addTestcaseName(i);
                    }
                    getline(*in, buffer);
                }
                in->close();
                delete in;
            }
        }
    }
}

void CReadResults::readOneTestPerFileResultsFromDirectory(path p)
{
    int info_cnt=0; //INFO
    int info_max;
    if(m_verbose) { //INFO
        cout << "Directory: " << p << endl;
        cout.flush();
    }
    vector<path> pathVector;
    copy(directory_iterator(p), directory_iterator(), back_inserter(pathVector));
    sort(pathVector.begin(), pathVector.end());
    info_max=pathVector.size();

    for(vector<path>::iterator it = pathVector.begin(); it != pathVector.end(); it++) {
        if(is_directory(*it)) { // recurse into subdirs
            if(basename(*it)!="")
                readOneTestPerFileResultsFromDirectory(*it);
        } else {
            if(m_verbose) { //INFO
                cout << info_cnt++ << "/" << info_max << '\r';
                cout.flush();
            }
            int revision      = boost::lexical_cast<int>(it->parent_path().filename().string());
            std::ifstream *in = new std::ifstream(it->c_str());
            String buffer;
            getline(*in, buffer);
            while(!in->eof()) {
                const char *i=buffer.c_str();
                if(*i=='X') i++;
                if(startsWith(i, "PASS: ")) {
                    i+=6;
                    m_results->setResult(revision, i, CResultsMatrix::trtPassed);
                } else if(startsWith(i, "FAIL: ")) {
                    i+=6;
                    m_results->setResult(revision, i, CResultsMatrix::trtFailed);
                } else if(startsWith(i, "UNSUPPORTED: ")) {
                    i+=13;
                    m_results->setResult(revision, i, CResultsMatrix::trtNotExecuted);
                }
                getline(*in, buffer);
            }
            in->close();
            delete in;
        }
    }
}

} // namespace sodatools
