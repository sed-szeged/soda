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


#include "HammingTestSuiteClusterPlugin.h"
#include "data/CBitMatrix.h"

namespace soda {

HammingTestSuiteClusterPlugin::HammingTestSuiteClusterPlugin()
{
}

HammingTestSuiteClusterPlugin::~HammingTestSuiteClusterPlugin()
{
}

std::string HammingTestSuiteClusterPlugin::getName()
{
    return "hamming";
}

std::string HammingTestSuiteClusterPlugin::getDescription()
{
    return "Clustering with Hamming distance";
}

std::map<String, String> HammingTestSuiteClusterPlugin::getRequiredParameters()
{
    return std::map<String, String>();
}


void HammingTestSuiteClusterPlugin::init(rapidjson::Document &doc)
{
    m_hamm_diff_row = doc["hamming_dist_row(%)"].GetInt();
    m_hamm_diff_cols = doc["hamming_dist_cols(%)"].GetInt();
    _0cluster_limit = doc["0cluster(%)"].GetInt();
}

void HammingTestSuiteClusterPlugin::execute(CSelectionData &data, std::map<std::string, CClusterDefinition>& clusterList)
{
    int numTC = int(data.getCoverage()->getNumOfTestcases());
    int numCE = int(data.getCoverage()->getNumOfCodeElements());
    std::cout<<std::endl<< "TC: "<< numTC << " ; CE: " << numCE << std::endl<< std::endl;


    std::cout<<"Row clustering..."<<std::endl;
    row_cluster_index = clustering( data.getCoverage(), m_hamm_diff_row, _0cluster_limit );


    // matrix transpose
    CIDManager idManagerTest, idManagerMethod;
    CBitMatrix* bitMatrix = new CBitMatrix();
    CCoverageMatrix* coverageTramsposeMatrix = new CCoverageMatrix( &idManagerTest, &idManagerMethod, bitMatrix);


    // original matrix transpose
    matrixTranspose(data, coverageTramsposeMatrix, bitMatrix, numTC, numCE);


    // cols clustering
    std::cout<<"Cols clustering..."<<std::endl;
    cols_cluster_index = clustering( coverageTramsposeMatrix, m_hamm_diff_cols, _0cluster_limit);


    // set clusterList
    HammingTestSuiteClusterPlugin::setClusterList(numTC, numCE, clusterList);



}



void HammingTestSuiteClusterPlugin::setClusterList(int numTC, int numCE, std::map<std::string, CClusterDefinition>& clusterList){

    CClusterDefinition def;

    std::vector<int>::iterator biggest = std::max_element(row_cluster_index.begin(),row_cluster_index.end());

    for(int a= 0 ; a <= *biggest ; a++)
        clusterList[ boost::lexical_cast<std::string>(a) ] = CClusterDefinition();

    for(int i = 0 ; i < numTC ; i++)
        clusterList[ boost::lexical_cast<std::string>( row_cluster_index[i]) ].addTestCase(IndexType(i));
    
    for(int i = 0 ; i < numCE ; i++)
        clusterList[ boost::lexical_cast<std::string>( cols_cluster_index[i]) ].addCodeElement(IndexType(i));
}



std::vector<int> HammingTestSuiteClusterPlugin::clustering(CCoverageMatrix* data, int diff, int nullcluster){

    int size1 = data->getBitMatrix().getNumOfRows();
    int size2 = data->getBitMatrix().getNumOfCols();

    int actual_index = 0;
    int tolerance = ( size2 * diff ) / 100 ; // Hamming tolerance
    int _0cluster_count = 0;
    int _0cluster_tolerance = (size2 * nullcluster ) / 100;

    std::vector<int> clusterindex_vector(size1,-1);

    std::cout<<"\tHamm tol.: "<<tolerance<<std::endl;
    std::cout<<"\t0cluster tol.: "<<_0cluster_tolerance <<std::endl;

    for(int index_1 = 0 ; index_1 < size1 ; index_1++ ){

        //if(index_1%1000==0) std::cout<<index_1<<". ("<<size1<<")"<<std::endl;

        int element_1 = data->getBitMatrix().getRow(IndexType(index_1)).count();

        // 0cluster
        if( element_1 > _0cluster_tolerance ){
            _0cluster_count++;
            clusterindex_vector[index_1] = 0;

        } else if ( clusterindex_vector[index_1] == -1 ) {

            actual_index++;
            clusterindex_vector[index_1] = actual_index;
            for(int index_2 = index_1 ; index_2 < size1 ; index_2++ ){

                int element_2 = data->getBitMatrix().getRow(IndexType(index_2)).count();

                if( index_1 != index_2 && clusterindex_vector[index_2] == -1 && abs(element_1-element_2) < tolerance )
                    if( hamming_distance(data, index_1, index_2, tolerance, size2) )  clusterindex_vector[index_2] = actual_index;

            }
        }
    }

    std::cout<<"\tcluster count: "<< actual_index << " ; 0cluster count: "<<_0cluster_count<<std::endl<< std::endl;
    return clusterindex_vector;

}



// Hamming distance calculate
bool HammingTestSuiteClusterPlugin::hamming_distance(CCoverageMatrix* data, int index1, int index2, int tolerance, int size){
    int distance = 0;
    for(int a = 0 ; a < size ; a++)
        distance += ( data->getBitMatrix().get(IndexType(index1),IndexType(a)) ^ data->getBitMatrix().get(IndexType(index2),IndexType(a)) );


    if( distance < tolerance )return true;

    return false;
}



// original matrix transpose
void HammingTestSuiteClusterPlugin::matrixTranspose(CSelectionData &data, CCoverageMatrix* coverageMatrix, CBitMatrix* bitMatrix, int numTC, int numCE){

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
    kernel.getTestSuiteClusterPluginManager().addPlugin(new HammingTestSuiteClusterPlugin());
}

} /* namespace soda */
