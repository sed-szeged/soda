#ifndef HAMMINGTESTSUITECLUSTERPLUGIN_H
#define HAMMINGTESTSUITECLUSTERPLUGIN_H

#include "engine/CKernel.h"

namespace soda {


class HammingTestSuiteClusterPlugin : public ITestSuiteClusterPlugin
{
public:

    HammingTestSuiteClusterPlugin();
    ~HammingTestSuiteClusterPlugin();

    std::string getName();

    std::string getDescription();

    std::map<String, String> getRequiredParameters();

    void init(rapidjson::Document &doc);

    void execute(CSelectionData &data, std::map<std::string, CClusterDefinition>& clusterList);

    std::vector<int> clustering_row(CSelectionData &data, int size );

    std::vector<int> clustering_cols(CSelectionData &data, int size );

    bool hamming_row(CSelectionData &data, int index1, int index2, int tolerance, int size);

    bool hamming_cols(CSelectionData &data, int index1, int index2, int tolerance, int size);


private:
    int m_hamm_diff_row;    // toleranciaszint (%)
    int m_hamm_diff_cols;   // toleranciaszint (%)
    int _0cluster_limit;    // ha ennel tobb 1 ertek van az elemben akkor 0klaszterbe kerul
    std::vector<int> row_cluster_index; // cimkek
    std::vector<int> cols_cluster_index;    // cimkek

};

} /* namespace soda */

#endif /* HAMMINGTESTSUITECLUSTERPLUGIN_H */
