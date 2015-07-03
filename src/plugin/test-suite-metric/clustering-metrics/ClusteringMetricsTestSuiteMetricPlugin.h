/*
 * Copyright (C): 2015 Department of Software Engineering, University of Szeged
 *
 * Authors: Bela Vancsics <vancsics@inf.u-szeged.hu>
 *
 * This file is part of SoDA.
 *
 *  SoDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SoDA is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with SoDA.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CLUSTERINGMETRICSTESTSUITEMETRICPLUGIN_H
#define CLUSTERINGMETRICSTESTSUITEMETRICPLUGIN_H

#include "engine/CKernel.h"

namespace soda {

class ClusteringMetricsTestSuiteMetricPlugin : public ITestSuiteMetricPlugin
{
public:

    ClusteringMetricsTestSuiteMetricPlugin();

    ~ClusteringMetricsTestSuiteMetricPlugin();

    std::string getName();

    std::string getDescription();

    void init(CSelectionData *data, std::map<std::string, CClusterDefinition> *clusterList, IndexType revision);

    std::vector<std::string> getDependency();

    void calculate(rapidjson::Document& results);

    void vectorInit();

    void clusterIntersection();

    void clusterPairsRow(int size1, int size2);

    void clusterPairsCols(int size1, int size2);

    int rowClusterCount();

    int colsClusterCount();

    void metrika1Calc();

    void metrika2Calc();

    void metrika3Calc();

    void metrika4Calc();

    void printVector(std::vector<float> metric);

    void clusterInfo();

    void imageWrite(CSelectionData *data);

private:
    CSelectionData *m_data;
    std::map<std::string, CClusterDefinition> *m_clusterList;
    std::vector< std::vector<int> > ones;
    std::vector< std::vector<int> > zeros;
    std::map<int, std::set<int> > pairs;

    std::vector<float> metrics1;
    std::vector<float> metrics2;
    std::vector<float> metrics3;
    std::vector<float> metrics4;

    int row_clusters;
    int cols_clusters;
    std::string resultsFile;

};

} /* namespace soda */

#endif /* CLUSTERINGMETRICSTESTSUITEMETRICPLUGIN_H */
