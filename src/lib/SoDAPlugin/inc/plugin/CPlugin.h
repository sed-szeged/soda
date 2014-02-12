#ifndef CPLUGIN_H
#define CPLUGIN_H

#include <string>

#include "CSharedLibraryHandler.h"

namespace soda {

/**
 * @brief Internal represenation of a plugin.
 */
template<typename TPluginManager>
class CPlugin {
public:
    /**
     * @brief Loads a plugin from the specified path.
     * @param pluginPath The path to the plugin.
     */
    CPlugin(const std::string &pluginPath) :
        m_sharedLibraryHandle(0),
        m_reference_count(0),
        m_registerFunction(0)
    {
        try {
            m_sharedLibraryHandle = CSharedLibraryHandler::loadLibrary(pluginPath);
            m_registerFunction =
                    CSharedLibraryHandler::getFunctionPointer<registerPluginFunctionSignature>(m_sharedLibraryHandle, "registerPlugin");
            m_reference_count = new size_t(1);
        } catch (std::exception &e) {
            if (m_sharedLibraryHandle != NULL) {
                CSharedLibraryHandler::unloadLibrary(m_sharedLibraryHandle);
            }
            throw e;
        }
    }

    /**
     * @brief Copy constructor.
     * @param plugin
     */
    CPlugin(const CPlugin &plugin)
    {
        m_sharedLibraryHandle(plugin.m_sharedLibraryHandle);
        m_reference_count(plugin.m_reference_count);
        m_registerFunction(plugin.m_versionFunction);

        if (*m_reference_count) {
            (*m_reference_count)++;
        }
    }

    ~CPlugin()
    {
        (*m_reference_count)--;
        if (*m_reference_count == 0) {
            delete m_reference_count;
            CSharedLibraryHandler::unloadLibrary(m_sharedLibraryHandle);
        }
    }

    /**
     * @brief Registers a plugin to the plugin manager.
     * @param pluginManager The plugin manager that handles the plugins.
     */
    void registerPlugin(TPluginManager &pluginManager)
    {
        m_registerFunction(pluginManager);
    }

private:
    typedef void registerPluginFunctionSignature(TPluginManager &);

    void   *m_sharedLibraryHandle;
    size_t *m_reference_count;

    registerPluginFunctionSignature *m_registerFunction;

};

} /* namespace soda */

#endif /* CPLUGIN_H */
