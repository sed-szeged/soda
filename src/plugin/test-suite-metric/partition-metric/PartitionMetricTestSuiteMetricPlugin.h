#ifndef PARTITIONMETRICTESTSUITEMETRICPLUGIN_H
#define PARTITIONMETRICTESTSUITEMETRICPLUGIN_H

#include "engine/CKernel.h"

namespace soda {

class PartitionMetricTestSuiteMetricPlugin : public ITestSuiteMetricPlugin
{
public:
public:

    PartitionMetricTestSuiteMetricPlugin();
    ~PartitionMetricTestSuiteMetricPlugin();

    std::string getName();

    std::string getDescription();

    void init(CSelectionData *data, std::map<std::string, CClusterDefinition> *clusterList, IndexType revision);

    std::vector<std::string> getDependency();

    void calculate(const std::string &output, std::map<std::string, MetricResults> &results);

private:
    CSelectionData *m_data;
    std::map<std::string, CClusterDefinition> *m_clusterList;
};

} /* namespace soda */

#endif /* PARTITIONMETRICTESTSUITEMETRICPLUGIN_H */
