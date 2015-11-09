/*
 * Copyright (C): 2013-2015 Department of Software Engineering, University of Szeged
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

#ifndef SIMPLEINSTRUMENTATIONLISTENERCOVERAGEREADERPLUGIN_H
#define SIMPLEINSTRUMENTATIONLISTENERCOVERAGEREADERPLUGIN_H

#ifndef BOOST_FILESYSTEM_VERSION
#define BOOST_FILESYSTEM_VERSION 3
#endif

#include "boost/regex.hpp"
#include "boost/filesystem.hpp"
#include "engine/CKernel.h"

namespace fs = boost::filesystem;

namespace soda {

/**
 * @brief The SimpleInstrumentationListenerJavaCoverageReaderPlugin class implements methods which reads
 *        SimpleInstrumentationListener output. SimpleInstrumentationListener can be found in the SoDA maven plugin package.
 */
class SimpleInstrumentationListenerJavaCoverageReaderPlugin : public ICoverageReaderPlugin
{
public:

    SimpleInstrumentationListenerJavaCoverageReaderPlugin();
    virtual ~SimpleInstrumentationListenerJavaCoverageReaderPlugin();

    std::string getName();
    std::string getDescription();

    CCoverageMatrix* read(const variables_map &vm);

private:
    /**
     * @brief Fills CCoverageMatrix with the content of the given file.
     *        Format: <test name>:<code element>
     */
    void readFromFile(String const &file);

    /**
     * @brief Stores coverage data.
     */
    CCoverageMatrix *coverage;
    CCoverageMatrix *mutationCoverage;

    String codeElements;
    String outputPath;
};

}

#endif /* SIMPLEINSTRUMENTATIONLISTENERCOVERAGEREADERPLUGIN_H */
