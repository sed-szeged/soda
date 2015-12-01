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

#include <sstream>

#include "engine/CKernel.h"

#ifdef _WIN32
    #define LIB_EXTENSION ".dll"
#elif __linux__
    #define LIB_EXTENSION ".so"
#else
    #define LIB_EXTENSION ".dylib"
#endif

namespace soda {

CKernel::CKernel() :
    pluginMap(new PluginMap()),
    bugsetReaderPluginManager(nullptr),
    changesetReaderPluginManager(nullptr),
    coverageReaderPluginManager(nullptr),
    resultsReaderPluginManager(nullptr),
    testSuiteClusterPluginManager(nullptr),
    testSuiteMetricPluginManager(nullptr),
    testSuitePrioritizationPluginManager(nullptr),
    testSuiteReductionPluginManager(nullptr),
    faultLocalizationTechniquePluginManager(nullptr),
    m_pluginDir(SODA_PLUGIN_DIR)
{

}

CKernel::~CKernel()
{
    if (changesetReaderPluginManager) {
        delete changesetReaderPluginManager;
    }
    if (coverageReaderPluginManager) {
        delete coverageReaderPluginManager;
    }
    if (resultsReaderPluginManager) {
        delete resultsReaderPluginManager;
    }
    if (testSuiteClusterPluginManager) {
        delete testSuiteClusterPluginManager;
    }
    if (testSuiteMetricPluginManager) {
        delete testSuiteMetricPluginManager;
    }
    if (testSuitePrioritizationPluginManager) {
        delete testSuitePrioritizationPluginManager;
    }
    if (testSuiteReductionPluginManager) {
        delete testSuiteReductionPluginManager;
    }
    if (faultLocalizationTechniquePluginManager) {
       delete faultLocalizationTechniquePluginManager;
    }
    if (mutationMetricPluginManager) {
        delete mutationMetricPluginManager;
    }

    // Delete internal plugin representations. The loaded libraries will be unloaded.
    for (PluginMap::iterator it = pluginMap->begin(); it != pluginMap->end(); it++) {
        delete it->second;
    }
    delete pluginMap;
}

BugsetReaderPluginManager& CKernel::getBugsetReaderPluginManager()
{
    if (bugsetReaderPluginManager == nullptr) {
        bugsetReaderPluginManager = new BugsetReaderPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/bugset-reader");
    }
    return *bugsetReaderPluginManager;
}

ChangesetReaderPluginManager& CKernel::getChangesetReaderPluginManager()
{
    if (changesetReaderPluginManager == nullptr) {
        changesetReaderPluginManager = new ChangesetReaderPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/changeset-reader");
    }
    return *changesetReaderPluginManager;
}

CoverageReaderPluginManager& CKernel::getCoverageReaderPluginManager()
{
    if (coverageReaderPluginManager == nullptr) {
        coverageReaderPluginManager = new CoverageReaderPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/coverage-reader");
    }
    return *coverageReaderPluginManager;
}

ResultsReaderPluginManager& CKernel::getResultsReaderPluginManager()
{
    if (resultsReaderPluginManager == nullptr) {
        resultsReaderPluginManager = new ResultsReaderPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/results-reader");
    }
    return *resultsReaderPluginManager;
}

TestSuiteClusterPluginManager& CKernel::getTestSuiteClusterPluginManager()
{
    if (testSuiteClusterPluginManager == nullptr) {
        testSuiteClusterPluginManager = new TestSuiteClusterPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/test-suite-cluster");
    }
    return *testSuiteClusterPluginManager;
}

TestSuiteMetricPluginManager& CKernel::getTestSuiteMetricPluginManager()
{
    if (testSuiteMetricPluginManager == nullptr) {
        testSuiteMetricPluginManager = new TestSuiteMetricPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/test-suite-metric");
    }
    return *testSuiteMetricPluginManager;
}

TestSuitePrioritizationPluginManager& CKernel::getTestSuitePrioritizationPluginManager()
{
    if (testSuitePrioritizationPluginManager == nullptr) {
        testSuitePrioritizationPluginManager = new TestSuitePrioritizationPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/test-suite-prioritization");
    }
    return *testSuitePrioritizationPluginManager;
}

TestSuiteReductionPluginManager& CKernel::getTestSuiteReductionPluginManager()
{
    if (testSuiteReductionPluginManager == nullptr) {
        testSuiteReductionPluginManager = new TestSuiteReductionPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/test-suite-reduction");
    }
    return *testSuiteReductionPluginManager;
}

FaultLocalizationTechniquePluginManager& CKernel::getFaultLocalizationTechniquePluginManager()
{
    if (faultLocalizationTechniquePluginManager == nullptr) {
        faultLocalizationTechniquePluginManager = new FaultLocalizationTechniquePluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/fault-localization-technique");
    }
    return *faultLocalizationTechniquePluginManager;
}

MutationMetricPluginManager& CKernel::getMutationMetricPluginManager() {
    if (mutationMetricPluginManager == nullptr) {
        mutationMetricPluginManager = new MutationMetricPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/mutation-metric");
    }
    return *mutationMetricPluginManager;
}

void CKernel::loadPluginsFromDirectory(const std::string &pluginDir)
{
    fs::path path(pluginDir);
    if (fs::exists(path) && fs::is_directory(path)) {
        fs::directory_iterator endIt;
        for (fs::directory_iterator it(path); it != endIt; it++) {
            if (fs::is_directory(it->status())) {
                loadPluginsFromDirectory(it->path().string());
            }
            if (!fs::is_regular_file(it->status())) {
                continue;
            }
            if (it->path().extension() != LIB_EXTENSION) {
                continue;
            }
            std::string pluginPath = it->path().string();
            if (pluginMap->find(pluginPath) == pluginMap->end()) {
                CPlugin *plugin = new CPlugin(pluginPath);
                (*pluginMap)[pluginPath] = plugin;
                plugin->registerPlugin(*this);
            }
        }
    }
}

} /* namespace soda */
