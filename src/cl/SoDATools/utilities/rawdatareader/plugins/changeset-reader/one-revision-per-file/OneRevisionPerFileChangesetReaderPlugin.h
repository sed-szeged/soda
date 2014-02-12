#ifndef ONEREVISIONPERFILECHANGESETREADERPLUGIN_H
#define ONEREVISIONPERFILECHANGESETREADERPLUGIN_H

#include "../IChangesetReaderPlugin.h"

namespace soda {

class OneRevisionPerFileChangesetReaderPlugin : public IChangesetReaderPlugin
{
public:

    OneRevisionPerFileChangesetReaderPlugin();
    ~OneRevisionPerFileChangesetReaderPlugin();

    std::string getName();
    std::string getDescription();

    CChangeset* read(const std::string &path);
};

} /* namespace soda */

#endif /* ONEREVISIONPERFILECHANGESETREADERPLUGIN_H */
