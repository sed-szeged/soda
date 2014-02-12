#ifndef ICHANGESETREADERPLUGIN_H
#define ICHANGESETREADERPLUGIN_H

#include "data/CRevision.h"
#include "data/CChangeset.h"

namespace soda {

/**
 * @brief Interface of the changeset reader plugins.
 */
class IChangesetReaderPlugin
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
     * @brief Reads a changeset from the specified path.
     * @param path The path to the changeset.
     * @return A CChangeset instance.
     */
    virtual CChangeset* read(const std::string &path) = 0;
};

} /* namespace soda */

#endif /* ICHANGESETREADERPLUGIN_H */
