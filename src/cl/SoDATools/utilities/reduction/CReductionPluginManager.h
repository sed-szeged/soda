/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Havas <havasd@inf.u-szeged.hu>
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

#ifndef CREDUCTIONPLUGINMANAGER_H
#define CREDUCTIONPLUGINMANAGER_H

#include "plugin/CInternalPluginManager.h"
#include "plugins/IReductionPlugin.h"

namespace soda {

/**
 * @brief This class manages the reduction algorithm plugins.
 */
class CReductionPluginManager
{
public:
    /**
     * @brief Creates a new manager.
     */
    CReductionPluginManager();
    ~CReductionPluginManager();

    /**
     * @brief Registers a reduction plugin. The plugin calls this in its registerPlugin function.
     * @param plugin An instance of the plugin.
     */
    void addReductionPlugin(IReductionPlugin *plugin);

    /**
     * @brief Loads reduction plugins.
     */
    void loadReductionPlugins();

    /**
     * @brief Returns a plugin by its name.
     * @param name The name of the plugin.
     * @return The plugin instance.
     */
    IReductionPlugin* getReductionPlugin(const String &name);

    /**
     * @brief Returns the names of the available reduction plugins.
     * @return A list of plugin names.
     */
    std::vector<String> getReductionPluginNames();

private:
    CInternalPluginManager<CReductionPluginManager> *m_reductionPluginManager;
    std::map<std::string, IReductionPlugin*> *m_reductionPlugins;
};

} /* namespace soda */

#endif /* CREDUCTIONPLUGINMANAGER_H */
