/*
 * Copyright (C): 2015-2016 Department of Software Engineering, University of Szeged
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

#ifndef COVERAGEMATRIXGENERATORTESTSUITECLUSTERPLUGIN_H
#define COVERAGEMATRIXGENERATORTESTSUITECLUSTERPLUGIN_H

#include "engine/CKernel.h"
#include "data/CBitMatrix.h"

namespace soda {


class CoverageMatrixGeneratorTestSuiteClusterPlugin : public ITestSuiteClusterPlugin
{
public:

    CoverageMatrixGeneratorTestSuiteClusterPlugin();
    ~CoverageMatrixGeneratorTestSuiteClusterPlugin();

    std::string getName();

    std::string getDescription();

    std::map<String, String> getRequiredParameters();

    void init(rapidjson::Document &doc);

    void execute(CSelectionData &data, std::map<std::string, CClusterDefinition>& clusterList);

    void addTestCases(CCoverageMatrix* coverageMatrix);

    void addMethods(CCoverageMatrix* coverageMatrix);

    void row_index_generate();

    void cols_index_generate();

    void matrix_set( CBitMatrix* bitMatrix );

    void vectorInit();

    void clusterIntersection(CCoverageMatrix* coverageMatrix);

    void metrika1Calc();

    void metrika2Calc();

    void metrika3Calc();

    void metrika4Calc();


public:
    std::string matrix_name;
    int row_size;
    int cols_size;
    int cluster_number;
    int in_count;
    int out_count;

    std::vector<int> row_cluster_index;
    std::vector<int> cols_cluster_index;
    std::vector< std::vector<int> > ones;
    std::vector< std::vector<int> > zeros;

};

} /* namespace soda */

#endif /* COVERAGEMATRIXGENERATORTESTSUITECLUSTERPLUGIN_H */
