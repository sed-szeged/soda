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

#ifndef CREADCOVERAGE_H
#define CREADCOVERAGE_H

#ifndef BOOST_FILESYSTEM_VERSION
#define BOOST_FILESYSTEM_VERSION 3
#endif

#include "data/CCoverageMatrix.h"
#include "boost/filesystem.hpp"

using namespace soda;

namespace sodatools {

/**
 * @brief The CReadCoverage class reads raw coverage data from directories.
 */
class CReadCoverage {
public:

    /**
     * @brief Creates a CReadCoverage object.
     * @param has_passfail  If true than removes pass fail info.
     * @param read_tc  If true than reads test case names.
     * @param read_ce  If true than reads the code element names.
     * @param verbose  If true than methods will print information to the standard output.
     */
    CReadCoverage(CCoverageMatrix&, bool has_passfail=false, bool read_tc=true, bool read_ce=true, bool verbose=true);

    /**
     * @brief Destroys a CReadCoverage object.
     */
    virtual ~CReadCoverage();

    /**
     * @brief Reads One Test per File format coverage data from the specified path.
     * @throw Exception at invalid directory path.
     */
    virtual void readOneTestPerFileCoverageFromDirectoryStructure(const char *);

    /**
     * @brief Reads One Test per File format coverage data from the specified path.
     */
    virtual void readOneTestPerFileCoverageFromDirectoryStructure(const String&);

    /**
     * @brief Set the value of the m_verbose variable to v.
     * @param v  Value to be set.
     */
    inline void setVerbose(bool v) {m_verbose = v;}

private:

    /**
     * @brief Reads One Test per File format code element names and test case names
     *        from the specified directory.
     *        Recursively searches the files in the given path p
     *        File name format: <Dirname>/<Basename>.<Extension>
     *        substring beginning at the cut position of <Dirname>/<Basename> is used as the test case name
     *        File format is: <CodeElementName>,<SourceFilePath>
     *        <CodeElementName> is used as the codeElement name
     */
    virtual void readOneTestPerFileCoverageFromDirectory1stPass(boost::filesystem::path, size_t);

    /**
     * @brief Reads One Test per File format coverage data from the specified directory.
     *        Recursively searches the files in the given path p
     *        File name format: <Dirname>/<Basename>.<Extension>
     *        substring beginning at the cut position of <Dirname>/<Basename> is used as the test case name
     *        File format is: <CodeElementName>,<SourceFilePath>
     *        <CodeElementName> is used as the codeElement name
     */
    virtual void readOneTestPerFileCoverageFromDirectory(boost::filesystem::path, size_t);

private:

    /**
     * @brief Stores coverage data.
     */
    CCoverageMatrix *m_coverage;

    /**
     * @brief If true than fail info will be removed at loading.
     */
    bool             m_hasPassFailInfo;

    /**
     * @brief If true than reads test case names.
     */
    bool             m_readTestcases;

    /**
     * @brief If true than reads the code element names.
     */
    bool             m_readCodeElements;

    /**
     * @brief If true than methods will print information to the standard output.
     */
    bool             m_verbose;
};

} // namespace sodatools

#endif /* CREADCOVERAGE_H */
