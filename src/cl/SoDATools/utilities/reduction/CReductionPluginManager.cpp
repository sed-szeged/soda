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

#include "CReductionPluginManager.h"

namespace soda {

CReductionPluginManager::CReductionPluginManager() :
    m_reductionPluginManager(new CInternalPluginManager<CReductionPluginManager>("plugins/")),
    m_reductionPlugins(new std::map<String, IReductionPlugin*>())
{}

CReductionPluginManager::~CReductionPluginManager()
{
    delete m_reductionPluginManager;
    delete m_reductionPlugins;
}

void CReductionPluginManager::loadReductionPlugins()
{
    m_reductionPluginManager->loadPlugins(*this);
}

void CReductionPluginManager::addReductionPlugin(IReductionPlugin *plugin)
{
    (*m_reductionPlugins)[plugin->getName()] = plugin;
}

IReductionPlugin* CReductionPluginManager::getReductionPlugin(const String &name)
{
    return m_reductionPlugins->at(name);
}

std::vector<String> CReductionPluginManager::getReductionPluginNames()
{
    std::vector<String> names;

    for (std::map<String, IReductionPlugin*>::iterator it = m_reductionPlugins->begin(); it != m_reductionPlugins->end(); it++) {
        names.push_back(it->first);
    }

    return names;
}

} /* namespace soda */
