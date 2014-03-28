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

namespace soda {

CKernel::CKernel() :
    m_pluginMap(new PluginMap()),
    m_changesetReaderPluginManager(NULL),
    m_coverageReaderPluginManager(NULL),
    m_resultsReaderPluginManager(NULL),
    m_testSuiteClusterPluginManager(NULL),
    m_testSuiteMetricPluginManager(NULL),
    m_testSuitePrioritizationPluginManager(NULL),
    m_testSuiteReductionPluginManager(NULL),
    m_pluginDir(SODA_PLUGIN_DIR)
{

}

CKernel::~CKernel()
{
    for (PluginMap::iterator it = m_pluginMap->begin(); it != m_pluginMap->end(); it++) {
        delete it->second;
    }
    delete m_pluginMap;

    if (m_changesetReaderPluginManager) {
        delete m_changesetReaderPluginManager;
    }
    if (m_coverageReaderPluginManager) {
        delete m_coverageReaderPluginManager;
    }
    if (m_resultsReaderPluginManager) {
        delete m_resultsReaderPluginManager;
    }
    if (m_testSuiteClusterPluginManager) {
        delete m_testSuiteClusterPluginManager;
    }
    if (m_testSuiteMetricPluginManager) {
        delete m_testSuiteMetricPluginManager;
    }
    if (m_testSuitePrioritizationPluginManager) {
        delete m_testSuitePrioritizationPluginManager;
    }
    if (m_testSuiteReductionPluginManager) {
        delete m_testSuiteReductionPluginManager;
    }
}

ChangesetReaderPluginManager& CKernel::getChangesetReaderPluginManager()
{
    if (m_changesetReaderPluginManager == NULL) {
        m_changesetReaderPluginManager = new ChangesetReaderPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/changeset-reader");
    }
    return *m_changesetReaderPluginManager;
}

CoverageReaderPluginManager& CKernel::getCoverageReaderPluginManager()
{
    if (m_coverageReaderPluginManager == NULL) {
        m_coverageReaderPluginManager = new CoverageReaderPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/coverage-reader");
    }
    return *m_coverageReaderPluginManager;
}

ResultsReaderPluginManager& CKernel::getResultsReaderPluginManager()
{
    if (m_resultsReaderPluginManager == NULL) {
        m_resultsReaderPluginManager = new ResultsReaderPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/results-reader");
    }
    return *m_resultsReaderPluginManager;
}

TestSuiteClusterPluginManager& CKernel::getTestSuiteClusterPluginManager()
{
    if (m_testSuiteClusterPluginManager == NULL) {
        m_testSuiteClusterPluginManager = new TestSuiteClusterPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/test-suite-cluster");
    }
    return *m_testSuiteClusterPluginManager;
}

TestSuiteMetricPluginManager& CKernel::getTestSuiteMetricPluginManager()
{
    if (m_testSuiteMetricPluginManager == NULL) {
        m_testSuiteMetricPluginManager = new TestSuiteMetricPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/test-suite-metric");
    }
    return *m_testSuiteMetricPluginManager;
}

TestSuitePrioritizationPluginManager& CKernel::getTestSuitePrioritizationPluginManager()
{
    if (m_testSuitePrioritizationPluginManager == NULL) {
        m_testSuitePrioritizationPluginManager = new TestSuitePrioritizationPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/test-suite-prioritization");
    }
    return *m_testSuitePrioritizationPluginManager;
}

TestSuiteReductionPluginManager& CKernel::getTestSuiteReductionPluginManager()
{
    if (m_testSuiteReductionPluginManager == NULL) {
        m_testSuiteReductionPluginManager = new TestSuiteReductionPluginManager();
        loadPluginsFromDirectory(m_pluginDir + "/test-suite-reduction");
    }
    return *m_testSuiteReductionPluginManager;
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
            if (it->path().extension() != ".so") {
                continue;
            }
            std::string pluginPath = it->path().string();
            if (m_pluginMap->find(pluginPath) == m_pluginMap->end()) {
                CPlugin *plugin = new CPlugin(pluginPath);
                (*m_pluginMap)[pluginPath] = plugin;
                plugin->registerPlugin(*this);
            }
        }
    }
}

} /* namespace soda */
