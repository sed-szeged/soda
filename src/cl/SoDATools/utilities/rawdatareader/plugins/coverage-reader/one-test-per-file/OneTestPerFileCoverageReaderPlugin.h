#ifndef ONETESTPERFILECOVERAGEREADERPLUGIN_H
#define ONETESTPERFILECOVERAGEREADERPLUGIN_H

#include "../ICoverageReaderPlugin.h"

namespace soda {

class OneTestPerFileCoverageReaderPlugin : public ICoverageReaderPlugin
{
public:

    OneTestPerFileCoverageReaderPlugin();
    ~OneTestPerFileCoverageReaderPlugin();

    std::string getName();
    std::string getDescription();

    CCoverageMatrix* read(const std::string &path);
};

}

#endif /* ONETESTPERFILECOVERAGEREADERPLUGIN_H */
