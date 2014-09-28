#ifndef DUPLATIONTESTSUITECLUSTERPLUGIN_H
#define DUPLATIONTESTSUITECLUSTERPLUGIN_H

#include "engine/CKernel.h"

namespace soda {

class DuplationTestSuiteClusterPlugin : public ITestSuiteClusterPlugin
{
public:

    DuplationTestSuiteClusterPlugin();
    ~DuplationTestSuiteClusterPlugin();

    std::string getName();

    std::string getDescription();

    std::map<String, String> getRequiredParameters();

    void init(rapidjson::Document &doc);

    void execute(CSelectionData &data, std::map<std::string, CClusterDefinition>& clusterList);

private:
    std::vector<IndexType> m_sizes;
};

} /* namespace soda */

#endif /* DUPLATIONTESTSUITECLUSTERPLUGIN_H */
