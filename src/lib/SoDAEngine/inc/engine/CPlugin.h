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

#ifndef CPLUGIN_H
#define CPLUGIN_H

#include <string>

#include "CSharedLibraryHandler.h"

namespace soda {

class CKernel;

/**
 * @brief Internal represenation of a plugin.
 */
class CPlugin {
public:
    /**
     * @brief Loads a plugin from the specified path.
     * @param pluginPath The path to the plugin.
     */
    CPlugin(const std::string &pluginPath);

    /**
     * @brief Copy constructor.
     * @param plugin
     */
    CPlugin(const CPlugin &plugin);

    ~CPlugin();

    /**
     * @brief Registers a plugin to the plugin manager.
     * @param pluginManager The plugin manager that handles the plugins.
     */
    void registerPlugin(CKernel &kernel);

private:
    /**
     * @brief The function signature of the function defined by the plugin to register itself through the Kernel.
     */
    typedef void registerPluginFunctionSignature(CKernel &);

    SharedLibraryHandle m_sharedLibraryHandle;
    size_t *m_reference_count;

    registerPluginFunctionSignature *m_registerFunction;

};

} /* namespace soda */

#endif /* CPLUGIN_H */
