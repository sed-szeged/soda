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

#ifndef ONETESTPERFILECOVERAGEREADERPLUGIN_H
#define ONETESTPERFILECOVERAGEREADERPLUGIN_H

#ifndef BOOST_FILESYSTEM_VERSION
#define BOOST_FILESYSTEM_VERSION 3
#endif

#include "boost/filesystem.hpp"
#include "engine/CKernel.h"

namespace fs = boost::filesystem;

namespace soda {

/**
 * @brief The OneTestPerFileCoverageReaderPlugin class implements methods which reads raw
 *        one test per file data and converts it to coverage data.
 */
class OneTestPerFileCoverageReaderPlugin : public ICoverageReaderPlugin
{
public:

    OneTestPerFileCoverageReaderPlugin();
    virtual ~OneTestPerFileCoverageReaderPlugin();

    std::string getName();
    std::string getDescription();

    CCoverageMatrix* read(const std::string &path);

private:
    /**
     * @brief Reads One Test per File format coverage data from the specified path.
     * @throw Exception at invalid directory path.
     */
    void readFromDirectoryStructure(const char *);

    /**
     * @brief Reads One Test per File format coverage data from the specified path.
     */
    void readFromDirectoryStructure(const std::string&);

    /**
     * @brief Reads One Test per File format code element names and test case names
     *        from the specified directory.
     *        Recursively searches the files in the given path p
     *        File name format: <Dirname>/<Basename>.<Extension>
     *        substring beginning at the cut position of <Dirname>/<Basename> is used as the test case name
     *        File format is: <CodeElementName>,<SourceFilePath>
     *        <CodeElementName> is used as the codeElement name
     */
    void readFromDirectory1stPass(fs::path, size_t);

    /**
     * @brief Reads One Test per File format coverage data from the specified directory.
     *        Recursively searches the files in the given path p
     *        File name format: <Dirname>/<Basename>.<Extension>
     *        substring beginning at the cut position of <Dirname>/<Basename> is used as the test case name
     *        File format is: <CodeElementName>,<SourceFilePath>
     *        <CodeElementName> is used as the codeElement name
     */
    void readFromDirectory(fs::path, size_t);

    /**
     * @brief Stores coverage data.
     */
    CCoverageMatrix *m_coverage;
};

}

#endif /* ONETESTPERFILECOVERAGEREADERPLUGIN_H */
