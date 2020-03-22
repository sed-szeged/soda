/*
 * Copyright (C): 2020 Department of Software Engineering, University of Szeged
 *
 * Authors: Ferenc Horváth <hferenc@inf.u-szeged.hu>
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

#ifndef NODEJSCOVERAGEREADERPLUGIN_H
#define NODEJSCOVERAGEREADERPLUGIN_H

#ifndef BOOST_FILESYSTEM_VERSION
#define BOOST_FILESYSTEM_VERSION 3
#endif

#include "boost/regex.hpp"
#include "boost/filesystem.hpp"
#include "engine/CKernel.h"

namespace fs = boost::filesystem;

namespace soda {

/**
 * @brief The NodeJsCoverageReaderPlugin class implements methods that reads
 *        NodeJS generated JSON files which contain call chains and converts it to coverage data.
 */
class NodeJsCoverageReaderPlugin : public ICoverageReaderPlugin
{
public:

    NodeJsCoverageReaderPlugin();
    virtual ~NodeJsCoverageReaderPlugin();

    std::string getName();
    std::string getDescription();

    CCoverageMatrix* read(const variables_map &vm);

private:
    /**
     * @brief Reads One Test per File coverage data from the specified path.
     * @throw Exception at invalid directory path.
     */
    void readFromDirectoryStructure(const char *);

    /**
     * @brief Reads One Test per File coverage data from the specified path.
     */
    void readFromDirectoryStructure(const std::string&);

    /**
     * @brief Reads One Test per File format coverage data from the specified directory.
     *        Recursively searches the files in the given path p
     *        File name format: <Dirname>/<Basename>.<Extension>
     *        <Basename> is used as the test case name
     *        File format is JSON (NodeJS generated chains format)
     */
    void readFromDirectory(fs::path);

    /**
     * @brief Reads chain level coverage from the given JSON document.
     */
    void readChainCoverage(const std::string&, const rapidjson::Document&);

    /**
     * @brief Reads function level coverage from the given JSON document.
     */
    void readFunctionCoverage(const std::string&, const rapidjson::Document&);

    /**
     * @brief Regular expression which is used to cut parts (e.g., common prefix) of the code element names.
     */
    boost::regex m_codeElementNameFilter;

    /**
     * @brief Stores coverage data.
     */
    CCoverageMatrix *m_coverage;

    enum Granularity {METHOD = 0, CHAIN};

    /**
     * @brief Stores the granularity of the coverage.
     */
    Granularity m_granularity;
};

}

#endif /* NODEJSCOVERAGEREADERPLUGIN_H */
