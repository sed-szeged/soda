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

#ifndef GCOVCOVERAGEREADERPLUGIN_H
#define GCOVCOVERAGEREADERPLUGIN_H

#ifndef BOOST_FILESYSTEM_VERSION
#define BOOST_FILESYSTEM_VERSION 3
#endif

#include "boost/filesystem.hpp"
#include "engine/CKernel.h"

namespace fs = boost::filesystem;

namespace soda {

/**
 * @brief The GcovCoverageReaderPlugin class implements methods which reads
 *        gcov data and converts it to statement level coverage data.
 */
class GcovCoverageReaderPlugin : public ICoverageReaderPlugin
{
public:

    GcovCoverageReaderPlugin();
    ~GcovCoverageReaderPlugin();

    std::string getName();
    std::string getDescription();

    CCoverageMatrix* read(const std::string &path);

private:
    /**
     * @brief Reads gcov coverage data from the specified path.
     *        First subdirectories of the specified path contains the test cases.
     *        Recursively searches the files and the directories in the given path.
     *        Gcov files should be separated by test cases and the directory names
     *        are used as test case names.
     * @throw Exception at invalid directory path.
     */
    void readFromDirectoryStructure(const char *);

    /**
     * @brief Reads gcov coverage data from the specified path.
     */
    void readFromDirectoryStructure(const std::string&);

    /**
     * @brief Reads test case names from the directories. Directory name is a test case name.
     */
    void readTestcaseNames(fs::path);

    /**
     * @brief Reads coverage data from directories.
     */
    void readCoverage(fs::path);

    /**
     * @brief Reads gcov coverage data from gcov files.
     *        Code element names are created from the file path and the line number of the source file which
     *        were executed in the current test case.
     */
    void readCoverageDataFromFile(fs::path);

    /**
     * @brief Stores coverage data.
     */
    CCoverageMatrix *m_coverage;

    /**
     * @brief Current test case name.
     */
    String m_currentTestcase;
};

}

#endif /* GCOVCOVERAGEREADERPLUGIN_H */
