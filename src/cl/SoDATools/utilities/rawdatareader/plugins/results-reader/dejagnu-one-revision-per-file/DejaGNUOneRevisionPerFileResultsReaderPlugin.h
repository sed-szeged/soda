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

#ifndef DEJAGNUONEREVISIONPERFILERESULTSREADERPLUGIN_H
#define DEJAGNUONEREVISIONPERFILERESULTSREADERPLUGIN_H

#ifndef BOOST_FILESYSTEM_VERSION
#define BOOST_FILESYSTEM_VERSION 3
#endif

#include "boost/filesystem.hpp"
#include "../IResultsReaderPlugin.h"

namespace soda {

class DejaGNUOneRevisionPerFileResultsReaderPlugin : public IResultsReaderPlugin
{
public:

    DejaGNUOneRevisionPerFileResultsReaderPlugin();
    ~DejaGNUOneRevisionPerFileResultsReaderPlugin();

    std::string getName();
    std::string getDescription();

    CResultsMatrix* read(const std::string &path);

private:
    /**
     * @brief Reads One Revision per File format results data from the specified path.
     * @throw Exception at invalid directory path.
     */
    virtual void readFromDirectoryStructure(const char *);

    /**
     * @brief Reads One Revision per File format results data from the specified path.
     */
    virtual void readFromDirectoryStructure(const String&);

    /**
     * @brief Reads One Revision per File format revision numbers and
     *        test case names from the specified directory.
     *        Recursively searches the files in the given path p
     *        File name format: <Dirname>/<Filename>
     *        <Filename> is used as the revision number
     *        File format is free text
     *        Lines matching the format "<TAG>: <TestcaseName>" are processed, where
     *        <TAG> can be: "{X}(PASS|FAIL|UNSUPPORTED)"
     *        <TestcaseName> is used as the test case name
     */
    void readFromDirectory1stPass(boost::filesystem::path);

    /**
     * @brief Reads One Revision per File format results data from the specified directory.
     *        Recursively searches the files in the given path p
     *        File name format: <Dirname>/<Filename>
     *        <Filename> is used as the revision number
     *        File format is free text
     *        Lines matching the format "<TAG>: <TestcaseName>" are processed, where
     *        <TAG> can be: "{X}(PASS|FAIL|UNSUPPORTED)"
     *        PASS|XPASS -> passed
     *        FAIL|XFAIL -> failed
     *        UNSUPPORTED|XUNSUPPORTED -> not executed
     *        <TestcaseName> is used as the test case name
     */
    void readFromDirectory(boost::filesystem::path);

    /**
     * @brief Stores results data.
     */
    CResultsMatrix *m_results;
};

}

#endif /* DEJAGNUONEREVISIONPERFILERESULTSREADERPLUGIN_H */
