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


#include "Ochiai_Dice_JaccardTestSuiteClusterPlugin.h"
#include "cluster.hpp"
#include "data/CBitMatrix.h"

namespace soda {

Ochiai_Dice_JaccardTestSuiteClusterPlugin::Ochiai_Dice_JaccardTestSuiteClusterPlugin()
{
}

Ochiai_Dice_JaccardTestSuiteClusterPlugin::~Ochiai_Dice_JaccardTestSuiteClusterPlugin()
{
}

std::string Ochiai_Dice_JaccardTestSuiteClusterPlugin::getName()
{
    return "ochiai-dice-jaccard";
}

std::string Ochiai_Dice_JaccardTestSuiteClusterPlugin::getDescription()
{
    return "Descr";
}

std::map<String, String> Ochiai_Dice_JaccardTestSuiteClusterPlugin::getRequiredParameters()
{
    return std::map<String, String>();
}

void Ochiai_Dice_JaccardTestSuiteClusterPlugin::init(rapidjson::Document &doc)
{
    algorithm_index = doc["alg.index"].GetInt();
    limit = doc["limit"].GetDouble();
    cluster_number = doc["cluster-number"].GetInt();
}

void Ochiai_Dice_JaccardTestSuiteClusterPlugin::execute(CSelectionData &data, std::map<std::string, CClusterDefinition>& clusterList)
{

    if( algorithm_index == 0 ) std::cout<<std::endl<<"Running Ochiai..."<<std::endl;
    if( algorithm_index == 1 ) std::cout<<std::endl<<"Running Dice..."<<std::endl;
    if( algorithm_index == 2 ) std::cout<<std::endl<<"Running Jaccard..."<<std::endl;


    int numTC = int(data.getCoverage()->getNumOfTestcases());
    int numCE = int(data.getCoverage()->getNumOfCodeElements());
    std::cout<<"Matrix size: "<<numTC<<" X "<<numCE<<std::endl;


    // TestCase X TestCase matrix calc.
    std::cout<<"... on rows..."<<std::endl;
    row_results(data, algorithm_index);



    // matrix transpose
    CIDManager idManagerTest, idManagerMethod;
    CBitMatrix* bitMatrix = new CBitMatrix();
    CCoverageMatrix* coverageTramspseMatrix = new CCoverageMatrix( &idManagerTest, &idManagerMethod, bitMatrix);
    matrixTranspose(data, coverageTramspseMatrix, bitMatrix, numTC, numCE);
    //std::cout<<"Transpose matrix: "<<coverageTramspseMatrix->getNumOfTestcases()<<" X "<<coverageTramspseMatrix->getNumOfCodeElements();



    // CodeElements X CodeElements matrix calc.
    std::cout<<"... on cols..."<<std::endl;
    cols_results(coverageTramspseMatrix, algorithm_index);



    // k-means alg. on (row) x (row) matrix
    std::cout<<std::endl<<"k-means running on row..."<<std::endl;
    kMeans_row();



    // k-means alg. on (cols) x (cols) matrix
    std::cout<<"... and on cols"<<std::endl;
    kMeans_cols();



    std::cout<<std::endl<<"Set clusterlist"<<std::endl;
    setClusterList( numTC, numCE, clusterList);

}



void Ochiai_Dice_JaccardTestSuiteClusterPlugin::row_results(CSelectionData &data, int alg_index){

    int numTC = int(data.getCoverage()->getNumOfTestcases());
    int numCE = int(data.getCoverage()->getNumOfCodeElements());

    floatRowVectors.resize(numTC, std::vector<float>(numTC));

    for(int index_1 = 0 ; index_1 < numTC ; index_1++){
        int element_1 = int(data.getCoverage()->getBitMatrix().getRow(IndexType(index_1)).count());

        for(int index_2 = 0 ; index_2 < numTC ; index_2++ )
            set_row_results(data, index_1, index_2, element_1, numCE, alg_index);

    }

}




void Ochiai_Dice_JaccardTestSuiteClusterPlugin::set_row_results(CSelectionData &data, int index_1, int index_2, int element_1, int numCE, int alg_index){

    int element_2 = int(data.getCoverage()->getBitMatrix().getRow(IndexType(index_2)).count());

    float* v_pointer = floatRowVectors[index_1].data();
    float results=0.0;

    if(alg_index==0){   // ochiai calc.
        if( element_1*element_2 != 0 )
            results = intersections_row(data, index_1, index_2, numCE) / (std::sqrt( element_1*element_2));

    } else if(alg_index==1){    // dice calc.
        if( element_1+element_2 != 0 )
            results = 2*intersections_row(data, index_1, index_2, numCE) / float(element_1+element_2);

    } else if(alg_index==2){    // jaccard calc.
        int unions = unions_row(data, index_1, index_2, numCE);
        if( unions != 0 )
            results = intersections_row(data, index_1, index_2, numCE) / float(unions);

    } else {
        std::cout<<"Error"<<std::endl;
        return;
    }

    *(v_pointer+index_2) = results_vs_limit(results,float(limit)) ;
}



void Ochiai_Dice_JaccardTestSuiteClusterPlugin::cols_results(CCoverageMatrix* matrix, int alg_index){

    int numTC = int(matrix->getNumOfTestcases());
    int numCE = int(matrix->getNumOfCodeElements());

    floatColsVectors.resize(numTC, std::vector<float>(numTC));

    for(int index_1 = 0 ; index_1 < numTC ; index_1++){

        int element_1 = matrix->getBitMatrix().getRow( IndexType(index_1) ).count();

        for(int index_2 = 0 ; index_2 < numTC ; index_2++ )
            set_cols_results(matrix, index_1, index_2, element_1, numCE, alg_index);

    }
}



void Ochiai_Dice_JaccardTestSuiteClusterPlugin::set_cols_results(CCoverageMatrix* matrix, int index_1, int index_2, int element_1, int numCE, int alg_index){

    int element_2 = int( matrix->getBitMatrix().getRow( IndexType(index_2)).count() );

    float results=0.0;
    float* v_pointer = floatColsVectors[index_2].data();

    if(alg_index==0){   // ochiai calc.
        if( element_1*element_2 != 0 )
            results = intersections_cols(matrix, index_1, index_2, numCE) / (std::sqrt( element_1*element_2));

    } else if(alg_index==1){    // dice calc.
        if( element_1+element_2 != 0 )
            results = 2*intersections_cols(matrix, index_1, index_2, numCE) / float(element_1+element_2);

    } else if(alg_index==2){    // jaccard calc.
        int unions = unions_cols(matrix, index_1, index_2, numCE);
        if( unions != 0 )
            results = intersections_cols(matrix, index_1, index_2, numCE) / float(unions);

    } else {
        std::cout<<"Error"<<std::endl;
        return;
    }

    *(v_pointer+index_1)= results_vs_limit(results,float(limit)) ;
}




float Ochiai_Dice_JaccardTestSuiteClusterPlugin::results_vs_limit( float results, float limit ){
    if ( limit != -1.0 )
        return results > limit ? 1.0 : 0.0;

    return results;
}





int Ochiai_Dice_JaccardTestSuiteClusterPlugin::intersections_row(CSelectionData &data, int index_1, int index_2, int size){
    int intersections = 0;
    for(int a = 0 ; a < size ; a++)
        intersections += int( int(data.getCoverage()->getBitMatrix().getRow(index_1)[a]) && int(data.getCoverage()->getBitMatrix().getRow(index_2)[a]) );

    return intersections;
}



int Ochiai_Dice_JaccardTestSuiteClusterPlugin::intersections_cols(CCoverageMatrix* matrix, int index_1, int index_2, int size){
    int intersections = 0;

    for(int a = 0 ; a < size ; a++)
        intersections += int( matrix->getBitMatrix().getRow(index_1)[a] && matrix->getBitMatrix().getRow(index_2)[a] );

    return intersections;
}




int Ochiai_Dice_JaccardTestSuiteClusterPlugin::unions_row(CSelectionData &data, int index_1, int index_2, int size){
    int unions = 0;
    for(int a = 0 ; a < size ; a++)
        unions += int( int(data.getCoverage()->getBitMatrix().getRow(index_1)[a]) || int(data.getCoverage()->getBitMatrix().getRow(index_2)[a]) );

    return unions;
}



int Ochiai_Dice_JaccardTestSuiteClusterPlugin::unions_cols(CCoverageMatrix* matrix, int index_1, int index_2, int size){
    int unions = 0;
    for(int a = 0 ; a < size ; a++)
        unions += int( matrix->getBitMatrix().getRow(index_1)[a] || matrix->getBitMatrix().getRow(index_2)[a] );

    return unions;
}



void Ochiai_Dice_JaccardTestSuiteClusterPlugin::kMeans_row(){

    Clustering::ClusterId num_clusters = cluster_number;
    Clustering::PointId num_points = floatRowVectors[0].size();
    Clustering::Dimensions num_dimensions = floatRowVectors[0].size();
    Clustering::PointsSpace ps_row(num_points, num_dimensions, Ochiai_Dice_JaccardTestSuiteClusterPlugin::floatRowVectors );
    Clustering::Clusters clusters_row(num_clusters, ps_row);


    clusters_row.k_means();

    Ochiai_Dice_JaccardTestSuiteClusterPlugin::RowIndexVector = clusters_row.points_to_clusters__;

    // alg. results (index) start: 0
    std::map<int,int> reIndex;
    int newIndex=1;
    for (std::vector<ClusterId>::iterator it = RowIndexVector.begin() ; it != RowIndexVector.end(); ++it)
        *it = (*it)+1;

    /*
     * TODO: 0cluster calculate and set in vectors
     */

}

void Ochiai_Dice_JaccardTestSuiteClusterPlugin::kMeans_cols(){

    Clustering::ClusterId num_clusters = cluster_number;
    Clustering::PointId num_points = floatColsVectors[0].size();
    Clustering::Dimensions num_dimensions = floatColsVectors[0].size();
    Clustering::PointsSpace ps_cols(num_points, num_dimensions, Ochiai_Dice_JaccardTestSuiteClusterPlugin::floatColsVectors );
    Clustering::Clusters clusters_cols(num_clusters, ps_cols);

    clusters_cols.k_means();

    Ochiai_Dice_JaccardTestSuiteClusterPlugin::ColsIndexVector = clusters_cols.points_to_clusters__;

    // alg. results (index) start: 0   
    for (std::vector<ClusterId>::iterator it = ColsIndexVector.begin() ; it != ColsIndexVector.end(); ++it)
        *it = (*it)+1;


    /*int a = 0;
    for (std::vector<ClusterId>::iterator it = ColsIndexVector.begin() ; it != ColsIndexVector.end(); ++it){
        std::cout<<a<<" - "<<*it<<std::endl;
        a++;
    } std::cout<<std::endl;*/


}



void Ochiai_Dice_JaccardTestSuiteClusterPlugin::setClusterList(int numTC, int numCE, std::map<std::string, CClusterDefinition>& clusterList){

    CClusterDefinition def;
    std::vector<ClusterId>::iterator maxIndex = std::max_element(RowIndexVector.begin(),RowIndexVector.end());

    for(int a= 0 ; a <= int(*maxIndex)+1 ; a++)
        clusterList[ boost::lexical_cast<std::string>(a) ] = CClusterDefinition();

    for(int i = 0 ; i < numTC ; i++)
        clusterList[ boost::lexical_cast<std::string>( int(RowIndexVector[i])) ].addTestCase(IndexType(i));

    for(int i = 0 ; i < numCE ; i++)
        clusterList[ boost::lexical_cast<std::string>( int(ColsIndexVector[i])) ].addCodeElement(IndexType(i));
}


int Ochiai_Dice_JaccardTestSuiteClusterPlugin::colsCount(CSelectionData &data, int index, int numTC){

    int sum = 0;
    for(int i = 0 ; i < numTC ; i++)
         sum += int(data.getCoverage()->getBitMatrix().getRow(IndexType(index))[i]);

    return sum;
}


void Ochiai_Dice_JaccardTestSuiteClusterPlugin::matrixTranspose(CSelectionData &data, CCoverageMatrix* coverageMatrix, CBitMatrix* bitMatrix, int numTC, int numCE){

    for(int i = 0 ; i < numCE ; i++)
        coverageMatrix->addTestcaseName( boost::lexical_cast<std::string>(i) );

    for(int i = 0 ; i < numTC ; i++)
        coverageMatrix->addCodeElementName( boost::lexical_cast<std::string>(i) );

    coverageMatrix->refitMatrixSize();

    for(int i = 0 ; i < numCE ; i++)
        for(int j = 0 ; j < numTC ; j++)
            bitMatrix->set(i,j, data.getCoverage()->getBitMatrix().get(IndexType(j),IndexType(i)));

}





extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteClusterPluginManager().addPlugin(new Ochiai_Dice_JaccardTestSuiteClusterPlugin());
}

} /* namespace soda */
