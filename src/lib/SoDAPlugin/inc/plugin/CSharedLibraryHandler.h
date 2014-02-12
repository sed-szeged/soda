#ifndef CSHAREDLIBRARYHANDLER_H
#define CSHAREDLIBRARYHANDLER_H

#include <stdexcept>
#include <dlfcn.h>
#include <string>
#include <iostream>


namespace soda {

/**
 * @brief Class to Load shared libraries from the filesystem.
 */
class CSharedLibraryHandler {

public:

    /**
     * @brief Loads a shared libraryfrom the given path.
     * @param path The path to the library.
     * @return
     */
    static void *loadLibrary(const std::string &path)
    {
        void *sharedLibraryHandle = dlopen(path.c_str(), RTLD_NOW);
        if (sharedLibraryHandle == NULL) {
            std::cout << "Can not load the shared library." << std::endl;
            std::cout << dlerror() << std::endl;
            throw std::runtime_error("Can not load the shared library.");
        }

        return sharedLibraryHandle;
    }

    /**
     * @brief Unloads a shared library.
     * @param sharedLibraryHandle The handle of the loaded library.
     */
    static void  unloadLibrary(void *sharedLibraryHandle)
    {
        int result = dlclose(sharedLibraryHandle);
        if (result != 0) {
            throw std::runtime_error("Can not unload the shared library.");
        }
    }

    /**
     * @brief Returns a pointer to a function.
     * @param sharedLibraryHandle The handle of the loaded library.
     * @param functionName The name of the function.
     */
    template<typename TFunctionSignature>
    static TFunctionSignature *getFunctionPointer(void *sharedLibraryHandle, const std::string &functionName)
    {
        dlerror();

        void *functionAddress = dlsym(sharedLibraryHandle, functionName.c_str());

        char *error = dlerror();
        if (error != NULL) {
            throw std::runtime_error("Can not find the function: " + functionName);
        }
        return reinterpret_cast<TFunctionSignature *>(functionAddress);
    }
};

}

#endif /* CSHAREDLIBRARYHANDLER_H */
