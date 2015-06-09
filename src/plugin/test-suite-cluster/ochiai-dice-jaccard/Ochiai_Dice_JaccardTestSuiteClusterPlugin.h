#ifndef OCHIAI_DICE_JACCARDTESTSUITECLUSTERPLUGIN_H
#define OCHIAI_DICE_JACCARDTESTSUITECLUSTERPLUGIN_H

#include "engine/CKernel.h"
#include "cluster.hpp"


namespace soda {

typedef unsigned int ClusterId;

class Ochiai_Dice_JaccardTestSuiteClusterPlugin : public ITestSuiteClusterPlugin
{
public:

    Ochiai_Dice_JaccardTestSuiteClusterPlugin();
    ~Ochiai_Dice_JaccardTestSuiteClusterPlugin();

    std::string getName();

    std::string getDescription();

    std::map<String, String> getRequiredParameters();

    void init(rapidjson::Document &doc);

    void execute(CSelectionData &data, std::map<std::string, CClusterDefinition>& clusterList);

    void row_results(CSelectionData &data, int algorithm_index);

    void cols_results(CSelectionData &data, int algorithm_index);

    void set_row_results(CSelectionData &data, int index_1, int index_2, int count_1, int element, int algorithm_index);

    void set_cols_results(CSelectionData &data, int index_1, int index_2, int count_1, int element, int algorithm_index);

    int metszet_row(CSelectionData &data, int index_1, int index_2, int size);

    int metszet_cols(CSelectionData &data, int index_1, int index_2, int size);

    int unio_row(CSelectionData &data, int index_1, int index_2, int size);

    int unio_cols(CSelectionData &data, int index_1, int index_2, int size);

    float results_vs_limit( float results, float limit );

    void kMeans_row();

    void kMeans_cols();

public:
    std::vector< std::vector<float> > floatRowVectors;
    std::vector< std::vector<float> > floatColsVectors;

    std::vector<ClusterId> RowIndexVector;
    std::vector<ClusterId>  ColsIndexVector;

 private:
    int algorithm_index; // 0 - ochiai, 1-dice, 2-jaccard
    double limit = 0.0;
    int cluster_number;
};

} /* namespace soda */

#endif /* OCHIAI_DICE_JACCARDTESTSUITECLUSTERPLUGIN_H */
