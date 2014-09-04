#ifndef COVERAGETESTSUITECLUSTERPLUGIN_H
#define COVERAGETESTSUITECLUSTERPLUGIN_H

#include "engine/CKernel.h"

namespace soda {

class CoverageTestSuiteClusterPlugin : public ITestSuiteClusterPlugin
{
private:
    typedef struct {
        IndexType testcaseId;
        IndexType priorityValue;
    } qelement;
    friend bool operator<(qelement d1, qelement d2);
public:

    CoverageTestSuiteClusterPlugin();
    ~CoverageTestSuiteClusterPlugin();

    std::string getName();

    std::string getDescription();

    void init(rapidjson::Document &doc);

    void execute(CSelectionData &data, std::map<std::string, CClusterDefinition>& clusterList);

private:
    std::vector<IndexType> m_sizes;
};

} /* namespace soda */

#endif /* COVERAGETESTSUITECLUSTERPLUGIN_H */
