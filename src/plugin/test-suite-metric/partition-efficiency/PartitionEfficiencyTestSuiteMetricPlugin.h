#ifndef PARTITIONEFFICIENCYTESTSUITEMETRICPLUGIN_H
#define PARTITIONEFFICIENCYTESTSUITEMETRICPLUGIN_H

#include "engine/CKernel.h"

namespace soda {

class PartitionEfficiencyTestSuiteMetricPlugin : public ITestSuiteMetricPlugin
{
public:

    PartitionEfficiencyTestSuiteMetricPlugin();
    ~PartitionEfficiencyTestSuiteMetricPlugin();

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

#endif /* PARTITIONEFFICIENCYTESTSUITEMETRICPLUGIN_H */
