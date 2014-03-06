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

#include "CSelectionsPluginManager.h"

namespace soda {

CSelectionsPluginManager::CSelectionsPluginManager() :
    m_prioritizationPluginManager(new CInternalPluginManager<CSelectionsPluginManager>("plugins/prioritization")),
    m_prioritizationPlugins(new std::map<String, IPrioritizationPlugin*>())
{}

CSelectionsPluginManager::~CSelectionsPluginManager()
{
    delete m_prioritizationPluginManager;
    delete m_prioritizationPlugins;
}

void CSelectionsPluginManager::loadPrioritizationPlugins()
{
    m_prioritizationPluginManager->loadPlugins(*this);
}

void CSelectionsPluginManager::addPrioritizationPlugin(IPrioritizationPlugin *plugin)
{
    (*m_prioritizationPlugins)[plugin->getName()] = plugin;
}

IPrioritizationPlugin* CSelectionsPluginManager::getPrioritizationPlugin(const String &name)
{
    return m_prioritizationPlugins->at(name);
}

std::vector<String> CSelectionsPluginManager::getPrioritizationPluginNames()
{
    std::vector<String> names;

    for (std::map<String, IPrioritizationPlugin*>::iterator it = m_prioritizationPlugins->begin(); it != m_prioritizationPlugins->end(); it++) {
        names.push_back(it->first);
    }

    return names;
}

} /* namespace soda */
