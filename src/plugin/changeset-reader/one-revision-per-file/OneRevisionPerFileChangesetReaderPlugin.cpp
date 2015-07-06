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

#include <fstream>
#include <iostream>

#include "boost/lexical_cast.hpp"
#include "boost/tokenizer.hpp"
#include "OneRevisionPerFileChangesetReaderPlugin.h"
#include "exception/CException.h"


namespace soda {

OneRevisionPerFileChangesetReaderPlugin::OneRevisionPerFileChangesetReaderPlugin() :
    m_changeset(NULL)
{}

OneRevisionPerFileChangesetReaderPlugin::~OneRevisionPerFileChangesetReaderPlugin()
{}

std::string OneRevisionPerFileChangesetReaderPlugin::getName()
{
    return "one-revision-per-file";
}

std::string OneRevisionPerFileChangesetReaderPlugin::getDescription()
{
    return "The change information is in one file for a revision and the format is: TODO.";
}

CChangeset* OneRevisionPerFileChangesetReaderPlugin::read(const std::string &path)
{
    m_changeset = new CChangeset();
    readFromDirectoryStructure(path);
    return m_changeset;
}

void OneRevisionPerFileChangesetReaderPlugin::readFromDirectoryStructure(const char * dirname)
{
    fs::path changes_path(dirname);
    if (!(exists(changes_path) && is_directory(changes_path))) {
        throw CException("OneRevisionPerFileChangesetReaderPlugin::readFromDirectoryStructure()", "Specified path does not exists or is not a directory.");
    }

    readFromDirectory1stPass(changes_path);
    m_changeset->refitSize();
    readFromDirectory(changes_path);
}

void OneRevisionPerFileChangesetReaderPlugin::readFromDirectoryStructure(const std::string& dirname)
{
    readFromDirectoryStructure(dirname.c_str());
}

void OneRevisionPerFileChangesetReaderPlugin::readFromDirectory1stPass(fs::path p)
{
    static int info_Tcnt = 0;
    static int info_Tmax = 0;
    int info_cnt = 0; //INFO
    int info_max;

    std::cout << "Directory: " << p << " 1st pass" << std::endl;
    std::cout.flush();

    std::vector<fs::path> pathVector;
    std::copy(fs::directory_iterator(p), fs::directory_iterator(), back_inserter(pathVector));
    sort(pathVector.begin(), pathVector.end());
    info_Tmax += info_max = pathVector.size();

    for (std::vector<fs::path>::iterator it = pathVector.begin(); it != pathVector.end(); it++) {
        if (is_directory(*it)) { // recurse into subdirs
            if (basename(*it) != "") {
                readFromDirectory1stPass(*it);
            } else {
                info_max--;
                info_Tmax--;
            }
        } else {
            std::cout << info_cnt++ << "/" << info_max << ' ' << info_Tcnt++ << "/" << info_Tmax << '\r';
            std::cout.flush();
            std::ifstream in(it->string().c_str());
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
                    m_changeset->addCodeElementName(data[0]);
                }
                data.clear();
            }
            in.close();
        }
    }
}

void OneRevisionPerFileChangesetReaderPlugin::readFromDirectory(fs::path p)
{
    int info_cnt = 0; //INFO
    int info_max;

    std::cout << "Directory: " << p << std::endl;
    std::cout.flush();

    std::vector<fs::path> pathVector;
    std::copy(fs::directory_iterator(p), fs::directory_iterator(), back_inserter(pathVector));
    std::sort(pathVector.begin(), pathVector.end());
    info_max = pathVector.size();

    for (std::vector<fs::path>::iterator it = pathVector.begin(); it != pathVector.end(); it++) {
        if (is_directory(*it)) { // recurse into subdirs
            if (basename(*it) != "")
                readFromDirectory(*it);
        } else {
            std::cout << info_cnt++ << "/" << info_max << '\r';
            std::cout.flush();

            RevNumType revision = boost::lexical_cast<RevNumType>(basename(*it));
            m_changeset->addRevision(revision);

            std::ifstream in(it->string().c_str());
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
                    if (m_changeset->getCodeElements().containsValue(data[0])) {
                        m_changeset->setChange(revision, data[0]);
                    } else {
                        throw CException("OneRevisionPerFileChangesetReaderPlugin::readFromDirectory", "Invalid codeElement name: `" + data[0] + "'");
                    }
                }
                data.clear();
            }
            in.close();
        }
    }
}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getChangesetReaderPluginManager().addPlugin(new OneRevisionPerFileChangesetReaderPlugin());
}

} /* namespace soda */
