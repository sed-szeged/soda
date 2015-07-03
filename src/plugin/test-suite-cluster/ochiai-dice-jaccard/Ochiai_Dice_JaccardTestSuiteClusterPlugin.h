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


#ifndef OCHIAI_DICE_JACCARDTESTSUITECLUSTERPLUGIN_H
#define OCHIAI_DICE_JACCARDTESTSUITECLUSTERPLUGIN_H

#include "engine/CKernel.h"
#include "cluster.hpp"
#include "data/CBitMatrix.h"


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

    void clusters(CCoverageMatrix* matrix, int algorithm_index, bool dimension);

    void set_results(CCoverageMatrix* matrix, int index_1, int index_2, int count_1, int element, int algorithm_index, bool dimension);

    int intersections(CCoverageMatrix* matrix, int index_1, int index_2, int size);

    int unionsCalc(CCoverageMatrix* matrix, int index_1, int index_2, int size);

    float results_vs_limit( float results, float limit );

    void kMeans(CCoverageMatrix* matrix, bool dimension);

    void setClusterList(int numTC, int numCE, std::map<std::string, CClusterDefinition>& clusterList);

    void matrixTranspose(CSelectionData &matrix, CCoverageMatrix* newMatrix, CBitMatrix* bitMatrix, int numTC, int numCE);

    void clusterDump(CCoverageMatrix* matrix, std::vector<ClusterId> labelVector, std::string outFile, bool dimension);

    void setIndex(CCoverageMatrix* matrix, std::vector<ClusterId> IndexVector);

    void matrixDensity(CCoverageMatrix* matrix);

    void histogram(CCoverageMatrix* matrix, bool dimension);


public:
    std::vector< std::vector<float> > floatRowVectors;
    std::vector< std::vector<float> > floatColsVectors;

    std::vector<ClusterId> RowIndexVector;
    std::vector<ClusterId>  ColsIndexVector;



 private:
    int algorithm_index; // 0 - ochiai, 1-dice, 2-jaccard
    double limit = 0.0;
    int cluster_number;
    int _0cluster_limit;
    std::string testClusterDump;
    std::string codeElementsClusterDump;

};

} /* namespace soda */

#endif /* OCHIAI_DICE_JACCARDTESTSUITECLUSTERPLUGIN_H */
