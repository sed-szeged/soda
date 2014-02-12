#ifndef IRESULTSREADERPLUGIN_H
#define IRESULTSREADERPLUGIN_H

#include "data/CResultsMatrix.h"

namespace soda {

/**
 * @brief Interface of the results reader plugins.
 */
class IResultsReaderPlugin
{
public:
    /**
     * @brief Returns the name of the plugin.
     * @return
     */
    virtual std::string getName() = 0;
    /**
     * @brief Returns the description of the plugin.
     * @return
     */
    virtual std::string getDescription() = 0;

    /**
     * @brief Reads results from the specified path.
     * @param [in] path The path to the results.
     * @return A CResultsMatrix instance.
     */
    virtual CResultsMatrix* read(const std::string &path) = 0;
};

} /* namespace soda */

#endif /* IRESULTSREADERPLUGIN_H */
