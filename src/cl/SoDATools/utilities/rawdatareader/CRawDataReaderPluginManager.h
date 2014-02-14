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

#ifndef CRAWDATAREADERPLUGINMANAGER_H
#define CRAWDATAREADERPLUGINMANAGER_H

#include <map>
#include <string>
#include <vector>

#include "plugin/CInternalPluginManager.h"
#include "plugins/changeset-reader/IChangesetReaderPlugin.h"
#include "plugins/coverage-reader/ICoverageReaderPlugin.h"
#include "plugins/results-reader/IResultsReaderPlugin.h"


namespace soda {

/**
 * @brief This class manages the reader plugins. Three types of readers are supported:
 *        - changeset
 *        - coverage
 *        - results
 */
class CRawDataReaderPluginManager
{
public:
    /**
     * @brief Creates a new manager.
     */
    CRawDataReaderPluginManager();
    ~CRawDataReaderPluginManager();

    /**
     * @brief Registers a changeset reader plugin. The plugin calls this in its registerPlugin function.
     * @param plugin An instance of the plugin.
     */
    void addChangesetReaderPlugin(IChangesetReaderPlugin *plugin);

    /**
     * @brief Loads changeset reader plugins.
     */
    void loadChangesetReaderPlugins();

    /**
     * @brief Returns a plugin by its name.
     * @param name The name of the plugin.
     * @return The plugin instance.
     */
    IChangesetReaderPlugin* getChangesetReaderPlugin(const std::string &name);

    /**
     * @brief Returns the names of the available changeset reader plugins.
     * @return A list of plugin names.
     */
    std::vector<std::string> getChangesetReaderPluginNames();

    /**
     * @brief Registers a coverage reader plugin. The plugin calls this in its registerPlugin function.
     * @param plugin An instance of the plugin.
     */
    void addCoverageReaderPlugin(ICoverageReaderPlugin *plugin);

    /**
     * @brief Loads coverage reader plugins.
     */
    void loadCoverageReaderPlugins();

    /**
     * @brief Returns a plugin by its name.
     * @param name The name of the plugin.
     * @return The plugin instance.
     */
    ICoverageReaderPlugin* getCoverageReaderPlugin(const std::string &name);

    /**
     * @brief Returns the names of the available coverage reader plugins.
     * @return A list of plugin names.
     */
    std::vector<std::string> getCoverageReaderPluginNames();

    /**
     * @brief Registers a results reader plugin. The plugin calls this in its registerPlugin function.
     * @param plugin An instance of the plugin.
     */
    void addResultsReaderPlugin(IResultsReaderPlugin *plugin);

    /**
     * @brief Loads results reader plugins.
     */
    void loadResultsReaderPlugins();

    /**
     * @brief Returns a plugin by its name.
     * @param name The name of the plugin.
     * @return The plugin instance.
     */
    IResultsReaderPlugin* getResultsReaderPlugin(const std::string &name);

    /**
     * @brief Returns the names of the available results reader plugins.
     * @return A list of plugin names.
     */
    std::vector<std::string> getResultsReaderPluginNames();

private:
    CInternalPluginManager<CRawDataReaderPluginManager> *m_changesetReaderPluginManager;
    std::map<std::string, IChangesetReaderPlugin*> *m_changesetReaderPlugins;

    CInternalPluginManager<CRawDataReaderPluginManager> *m_coverageReaderPluginManager;
    std::map<std::string, ICoverageReaderPlugin*> *m_coverageReaderPlugins;

    CInternalPluginManager<CRawDataReaderPluginManager> *m_resultsReaderPluginManager;
    std::map<std::string, IResultsReaderPlugin*> *m_resultsReaderPlugins;
};

} /* namespace soda */

#endif /* CRAWDATAREADERPLUGINMANAGER_H */
