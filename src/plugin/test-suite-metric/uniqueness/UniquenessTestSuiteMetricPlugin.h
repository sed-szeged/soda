#ifndef UNIQUENESSTESTSUITEMETRICPLUGIN_H
#define UNIQUENESSTESTSUITEMETRICPLUGIN_H

#include "engine/CKernel.h"

namespace soda {

class UniquenessTestSuiteMetricPlugin : public ITestSuiteMetricPlugin
{
public:
public:

    UniquenessTestSuiteMetricPlugin();
    ~UniquenessTestSuiteMetricPlugin();

    std::string getName();

    std::string getDescription();

    void init(CSelectionData *data, std::map<std::string, CClusterDefinition> *clusterList, IndexType revision);

    std::vector<std::string> getDependency();

    void calculate(rapidjson::Document &results);

private:
    CSelectionData *m_data;
    std::map<std::string, CClusterDefinition> *m_clusterList;
    IndexType m_revision;
};

} /* namespace soda */

#endif /* UNIQUENESSTESTSUITEMETRICPLUGIN_H */
