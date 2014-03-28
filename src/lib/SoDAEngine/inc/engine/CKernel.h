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

#ifndef CKERNEL_H
#define CKERNEL_H

#include <string>

#include "boost/filesystem.hpp"
#include "engine/CPlugin.h"
#include "engine/PluginTypes.h"

namespace fs = boost::filesystem;

namespace soda {

class CKernel
{
public:
    CKernel();
    ~CKernel();

    ChangesetReaderPluginManager& getChangesetReaderPluginManager();
    CoverageReaderPluginManager& getCoverageReaderPluginManager();
    ResultsReaderPluginManager& getResultsReaderPluginManager();
    TestSuiteClusterPluginManager& getTestSuiteClusterPluginManager();
    TestSuiteMetricPluginManager& getTestSuiteMetricPluginManager();
    TestSuitePrioritizationPluginManager& getTestSuitePrioritizationPluginManager();
    TestSuiteReductionPluginManager& getTestSuiteReductionPluginManager();

private:
    /**
     * @brief Loads plugins recursively from the given directory.
     * @param pluginDir The path where the plugins are.
     */
    void loadPluginsFromDirectory(const std::string &pluginDir);
private:
    typedef std::map<std::string, CPlugin* > PluginMap;

     PluginMap *m_pluginMap;

     // Managers
     ChangesetReaderPluginManager         *m_changesetReaderPluginManager;
     CoverageReaderPluginManager          *m_coverageReaderPluginManager;
     ResultsReaderPluginManager           *m_resultsReaderPluginManager;
     TestSuiteClusterPluginManager        *m_testSuiteClusterPluginManager;
     TestSuiteMetricPluginManager         *m_testSuiteMetricPluginManager;
     TestSuitePrioritizationPluginManager *m_testSuitePrioritizationPluginManager;
     TestSuiteReductionPluginManager      *m_testSuiteReductionPluginManager;
};

} /* namespace soda */

#endif /* CKERNEL_H */
