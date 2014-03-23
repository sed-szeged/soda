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
#include "CTestSuiteMetricsPluginManager.h"

namespace soda {

CTestSuiteMetricsPluginManager::CTestSuiteMetricsPluginManager() :
    m_clusterPluginManager(new CInternalPluginManager<CTestSuiteMetricsPluginManager>("plugins/cluster")),
    m_clusterPlugins(new std::map<String, IClusterPlugin*>()),
    m_metricPluginManager(new CInternalPluginManager<CTestSuiteMetricsPluginManager>("plugins/metric")),
    m_metricPlugins(new std::map<String, IMetricPlugin*>())
{}

CTestSuiteMetricsPluginManager::~CTestSuiteMetricsPluginManager()
{
    delete m_clusterPluginManager;
    delete m_clusterPlugins;
    delete m_metricPluginManager;
    delete m_metricPlugins;
}

void CTestSuiteMetricsPluginManager::loadClusterPlugins()
{
    m_clusterPluginManager->loadPlugins(*this);
}

void CTestSuiteMetricsPluginManager::addClusterPlugin(IClusterPlugin *plugin)
{
    (*m_clusterPlugins)[plugin->getName()] = plugin;
}

IClusterPlugin* CTestSuiteMetricsPluginManager::getClusterPlugin(const String &name)
{
    return m_clusterPlugins->at(name);
}

std::vector<String> CTestSuiteMetricsPluginManager::getClusterPluginNames()
{
    std::vector<String> names;

    for (std::map<String, IClusterPlugin*>::iterator it = m_clusterPlugins->begin(); it != m_clusterPlugins->end(); it++) {
        names.push_back(it->first);
    }

    return names;
}

void CTestSuiteMetricsPluginManager::loadMetricPlugins()
{
    m_metricPluginManager->loadPlugins(*this);
}

void CTestSuiteMetricsPluginManager::addMetricPlugin(IMetricPlugin *plugin)
{
    (*m_metricPlugins)[plugin->getName()] = plugin;
}

IMetricPlugin* CTestSuiteMetricsPluginManager::getMetricPlugin(const String &name)
{
    return m_metricPlugins->at(name);
}

std::vector<String> CTestSuiteMetricsPluginManager::getMetricPluginNames()
{
    std::vector<String> names;

    for (std::map<String, IMetricPlugin*>::iterator it = m_metricPlugins->begin(); it != m_metricPlugins->end(); it++) {
        names.push_back(it->first);
    }

    return names;
}

} /* namespace soda */
