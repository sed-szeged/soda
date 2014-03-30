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


#ifndef CPLUGINMANAGER_H
#define CPLUGINMANAGER_H
#include <string>
#include <vector>
#include <map>

namespace soda {

template<typename TPluginInterface>
class CPluginManager
{
public:
    CPluginManager() :
        m_plugins(new PluginInstanceMap())
    {}

    ~CPluginManager()
    {
        typename PluginInstanceMap::iterator it;
        for (it = m_plugins->begin(); it != m_plugins->end(); it++) {
            delete it->second;
        }
        delete m_plugins;
    }

    /**
     * @brief Registers a plugin. The plugin calls this in its registerPlugin function.
     * @param plugin An instance of the plugin.
     */
    void addPlugin(TPluginInterface *plugin)
    {
        (*m_plugins)[plugin->getName()] = plugin;
    }

    /**
     * @brief Returns a plugin by its name.
     * @param name The name of the plugin.
     * @return The plugin instance.
     */
    TPluginInterface* getPlugin(const std::string &name)
    {
        return m_plugins->at(name);
    }

    /**
     * @brief Returns the names of the available plugins.
     * @return A list of plugin names.
     */
    std::vector<std::string> getPluginNames()
    {
        std::vector<std::string> names;
        typename PluginInstanceMap::iterator it;

        for (it = m_plugins->begin(); it != m_plugins->end(); it++) {
            names.push_back(it->first);
        }

        return names;
    }

private:
    typedef std::map<std::string, TPluginInterface*> PluginInstanceMap;
    PluginInstanceMap *m_plugins;
};

} /* namespace soda */

#endif /* CPLUGINMANAGER_H */
