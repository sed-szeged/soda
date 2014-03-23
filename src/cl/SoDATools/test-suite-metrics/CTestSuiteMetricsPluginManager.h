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

#ifndef CTESTSUITEMETRICSPLUGINMANAGER_H
#define CTESTSUITEMETRICSPLUGINMANAGER_H

#include "plugin/CInternalPluginManager.h"
#include "plugins/cluster/IClusterPlugin.h"
#include "plugins/metric/IMetricPlugin.h"

namespace soda {

/**
 * @brief This class manages the metrics and cluster plugins.
 */
class CTestSuiteMetricsPluginManager
{

public:
    CTestSuiteMetricsPluginManager();
    ~CTestSuiteMetricsPluginManager();

    /**
     * @brief Registers a cluster plugin. The plugin calls this in its registerPlugin function.
     * @param plugin An instance of the plugin.
     */
    void addClusterPlugin(IClusterPlugin *plugin);

    /**
     * @brief Loads cluster plugins.
     */
    void loadClusterPlugins();

    /**
     * @brief Returns a plugin by its name.
     * @param name The name of the plugin.
     * @return The plugin instance.
     */
    IClusterPlugin* getClusterPlugin(const std::string &name);

    /**
     * @brief Returns the names of the available cluster plugins.
     * @return A list of plugin names.
     */
    std::vector<std::string> getClusterPluginNames();

    /**
     * @brief Registers a metric plugin. The plugin calls this in its registerPlugin function.
     * @param plugin An instance of the plugin.
     */
    void addMetricPlugin(IMetricPlugin *plugin);

    /**
     * @brief Loads metric plugins.
     */
    void loadMetricPlugins();

    /**
     * @brief Returns a plugin by its name.
     * @param name The name of the plugin.
     * @return The plugin instance.
     */
    IMetricPlugin* getMetricPlugin(const std::string &name);

    /**
     * @brief Returns the names of the available metric plugins.
     * @return A list of plugin names.
     */
    std::vector<std::string> getMetricPluginNames();

private:
    CInternalPluginManager<CTestSuiteMetricsPluginManager> *m_clusterPluginManager;
    std::map<std::string, IClusterPlugin*> *m_clusterPlugins;

    CInternalPluginManager<CTestSuiteMetricsPluginManager> *m_metricPluginManager;
    std::map<std::string, IMetricPlugin*> *m_metricPlugins;

};

} /* namespace soda */

#endif /* CTESTSUITEMETRICSPLUGINMANAGER_H */
