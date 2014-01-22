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

#ifndef CREADRESULTS_H
#define CREADRESULTS_H

#ifndef BOOST_FILESYSTEM_VERSION
#define BOOST_FILESYSTEM_VERSION 3
#endif

#include "data/CResultsMatrix.h"
#include "boost/filesystem.hpp"

using namespace soda;

namespace sodatools {

/**
 * @brief The CReadResults class reads raw results data from directories.
 */
class CReadResults {
public:

    /**
     * @brief Creates a CReadResults object.
     * @param read_rv  If true than reads revisions.
     * @param read_tc  If true than reads test case names.
     * @param verbose  If true than methods will print information to the standard output.
     */
    CReadResults(CResultsMatrix*, bool read_rv=true, bool read_tc=true, bool verbose=true);

    /**
     * @brief Destroys a CReadResults object.
     */
    ~CReadResults();

    /**
     * @brief Reads One Test per File format results data from the specified path.
     * @throw Exception at invalid directory path.
     */
    virtual void readOneTestPerFileResultsFromDirectoryStructure(const char *);

    /**
     * @brief Reads One Test per File format results data from the specified path.
     */
    virtual void readOneTestPerFileResultsFromDirectoryStructure(const String&);

    /**
     * @brief Set the value of the m_verbose variable to v.
     * @param v Value to be set.
     */
    inline void setVerbose(bool v) {m_verbose = v;}

private:

    /**
     * @brief Reads One Test per File format revision numbers and test case names from the specified directory.
     *        Recursively searches the files in the given path p
     *        File name format: <Dirname>/<Filename>
     *        <Filename> is used as the revision number
     *        File format is free text
     *        Lines matching the format "<TAG>: <TestcaseName>" are processed, where
     *        <TAG> can be: "{X}(PASS|FAIL|UNSUPPORTED)"
     *        <TestcaseName> is used as the test case name
     */
    virtual void readOneTestPerFileResultsFromDirectory1stPass(boost::filesystem::path);

    /**
     * @brief Reads One Test per File format results data from the specified directory.
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
    virtual void readOneTestPerFileResultsFromDirectory(boost::filesystem::path);

private:

    /**
     * @brief Stores results data.
     */
    CResultsMatrix *m_results;

    /**
     * @brief If true than reads revisions.
     */
    bool            m_readRevisions;

    /**
     * @brief If true than reads test case names.
     */
    bool            m_readTestcases;

    /**
     * @brief If true than methods will print information to the standard output.
     */
    bool            m_verbose;
};

} // namespace sodatools

#endif /* CREADRESULTS_H */
