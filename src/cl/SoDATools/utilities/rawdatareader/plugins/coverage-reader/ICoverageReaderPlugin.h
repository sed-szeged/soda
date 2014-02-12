#ifndef ICOVERAGEREADERPLUGIN_H
#define ICOVERAGEREADERPLUGIN_H

#include "data/CCoverageMatrix.h"

namespace soda {

/**
 * @brief Interface of the coverage reader plugins.
 */
class ICoverageReaderPlugin
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
     * @brief Reads a coverage from the specified path.
     * @param [in] path The path to the coverage.
     * @return A CCoverageMatrix instance.
     */
    virtual CCoverageMatrix* read(const std::string &path) = 0;
};

} /* namespace soda */

#endif /* ICOVERAGEREADERPLUGIN_H */
