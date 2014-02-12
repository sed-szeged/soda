#ifndef DEJAGNUONEREVISIONPERFILERESULTSREADERPLUGIN_H
#define DEJAGNUONEREVISIONPERFILERESULTSREADERPLUGIN_H

#include "../IResultsReaderPlugin.h"

namespace soda {

class DejaGNUOneRevisionPerFileResultsReaderPlugin : public IResultsReaderPlugin
{
public:

    DejaGNUOneRevisionPerFileResultsReaderPlugin();
    ~DejaGNUOneRevisionPerFileResultsReaderPlugin();

    std::string getName();
    std::string getDescription();

    CResultsMatrix* read(const std::string &path);
};

}

#endif /* DEJAGNUONEREVISIONPERFILERESULTSREADERPLUGIN_H */
