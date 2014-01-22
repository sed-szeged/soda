/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: László Langó <lango@inf.u-szeged.hu>
 *          Tamás Gergely <gertom@inf.u-szeged.hu>
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
#include "datareader/CReadChangeset.h"
#include "boost/lexical_cast.hpp"
#include "boost/tokenizer.hpp"
#include "data/CChangeset.h"
#include "exception/CException.h"

using namespace std;
using namespace boost::filesystem;

namespace sodatools {

CReadChangeset::CReadChangeset(CChangeset* changeset, bool read_mt, bool verbose) :
    m_changeset(changeset),
    m_readCodeElements(read_mt),
    m_verbose(verbose)
{
}

CReadChangeset::~CReadChangeset()
{
}

void CReadChangeset::readOneTestPerFileChangesetFromDirectoryStructure(const char * dirname)
{
    path changes_path(dirname);
    if (!(exists(changes_path) && is_directory(changes_path))) {
        throw CException("CReadChangeset::readOneTestPerFileChangesetFromDirectoryStructure()", "Specified path does not exists or is not a directory.");
    }
    if(m_readCodeElements) {
        readOneTestPerFileChangesetFromDirectory1stPass(changes_path);
        m_changeset->refitSize();
    }
    readOneTestPerFileChangesetFromDirectory(changes_path);
}

void CReadChangeset::readOneTestPerFileChangesetFromDirectoryStructure(const String& dirname)
{
    readOneTestPerFileChangesetFromDirectoryStructure(dirname.c_str());
}

/*****************************************************************************
 * Private parts
 */

void CReadChangeset::readOneTestPerFileChangesetFromDirectory1stPass(path p)
{
    static int info_Tcnt=0;
    static int info_Tmax=0;
    int info_cnt=0; //INFO
    int info_max;
    if(m_verbose) { //INFO
        cout << "Directory: " << p << " 1st pass" << endl;
        cout.flush();
    }
    vector<path> pathVector;
    copy(directory_iterator(p), directory_iterator(), back_inserter(pathVector));
    sort(pathVector.begin(), pathVector.end());
    info_Tmax+=info_max=pathVector.size();

    for(vector<path>::iterator it = pathVector.begin(); it != pathVector.end(); it++) {
        if(is_directory(*it)) { // recurse into subdirs
            if(basename(*it)!="") {
                readOneTestPerFileChangesetFromDirectory1stPass(*it);
            } else {
                info_max--; info_Tmax--;
            }
        } else {
            if(m_verbose) { //INFO
                cout << info_cnt++ << "/" << info_max << ' ' << info_Tcnt++ << "/" << info_Tmax << '\r';
                cout.flush();
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
                        m_changeset->addCodeElementName(data[0]);
                    }
                    data.clear();
                }
                in.close();
            }
        }
    }
}

void CReadChangeset::readOneTestPerFileChangesetFromDirectory(path p)
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
                readOneTestPerFileChangesetFromDirectory(*it);
        } else {
            if(m_verbose) { //INFO
                cout << info_cnt++ << "/" << info_max << '\r';
                cout.flush();
            }
            RevNumType revision = boost::lexical_cast<RevNumType>(basename(*it));
            m_changeset->addRevision(revision);

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
                    if (m_changeset->getCodeElements().containsValue(data[0])) {
                        m_changeset->setChange(revision, data[0]);
                    } else {
                        throw CException("CReadChangeset::readOneTestPerFileChangesetFromDirectory", "Invalid codeElement name: `" + data[0] + "'");
                    }
                }
                data.clear();
            }
            in.close();
        }
    }
}

} // namespace sodatools
