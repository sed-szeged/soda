/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
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

    std::cout<< "TC: "<< numTC << " ; CE: " << numCE << std::endl;

    row_cluster_index = clustering_row(data,numTC);

    cols_cluster_index = clustering_cols(data,numCE);

}



std::vector<int> HammingTestSuiteClusterPlugin::clustering_row(CSelectionData &data, int size ){

    int actual_index = 0;
    int tolerance = ( size * m_hamm_diff_row ) / 100 ; // Hamming tolerance
    int _0cluster_count = 0;
    std::vector<int> clusterindex_vector(size,-1);

    std::cout<<"Hamm row tol.: "<<tolerance<<std::endl;

    for(int index_1 = 0 ; index_1 < size ; index_1++ ){

        int element_1 = data.getCoverage()->getBitMatrix().getRow(IndexType(index_1)).count();

        // 0cluster
        if( element_1 > _0cluster_limit ){
            _0cluster_count++;
            clusterindex_vector[index_1] = 0;

        } else if ( clusterindex_vector[index_1] == -1 ) {

            actual_index++;
            clusterindex_vector[index_1] = actual_index;
            for(int index_2 = index_1 ; index_2 < size ; index_2++ ){

                int element_2 = data.getCoverage()->getBitMatrix().getRow(IndexType(index_2)).count();

                if( index_1 != index_2 && clusterindex_vector[index_2] == -1 && abs(element_1-element_2) < tolerance )
                    if( hamming_row(data, index_1, index_2, tolerance, size) ) clusterindex_vector[index_2] = actual_index; // ... akkor azonos lesz a cimkejuk

            }
        }
    }

    std::cout<<"cluster count: "<< actual_index << " ; 0cluster count: "<<_0cluster_count<<std::endl;
    return clusterindex_vector;

}

std::vector<int> HammingTestSuiteClusterPlugin::clustering_cols(CSelectionData &data, int size ){

    int actual_index = 0;
    int tolerance = ( size * m_hamm_diff_cols ) / 100 ;
    int _0cluster_count = 0;
    std::vector<int> clusterindex_vector(size,-1);

    std::cout<<"Hamm cols tol.: "<<tolerance<<std::endl;

    for(int index_1 = 0 ; index_1 < size ; index_1++ ){

        int element_1 = data.getCoverage()->getBitMatrix().getCol(IndexType(index_1)).count();

        // 0cluster
        if( element_1 > _0cluster_limit ){
            clusterindex_vector[index_1] = 0;
            _0cluster_count++;

        } else if ( clusterindex_vector[index_1] == -1 ) {

            actual_index++;
            clusterindex_vector[index_1] = actual_index;

            for(int index_2 = index_1 ; index_2 < size ; index_2++ ){
                int element_2 = data.getCoverage()->getBitMatrix().getCol(IndexType(index_2)).count();

                if( index_1 != index_2 && clusterindex_vector[index_2] == -1 && abs(element_1-element_2) < tolerance )
                    if( hamming_cols(data, index_1, index_2, tolerance, size) ) clusterindex_vector[index_2] = actual_index;

            }
        }
    }

    std::cout<<"cluster count: "<< actual_index << " ; 0cluster count: "<<_0cluster_count<<std::endl;
    return clusterindex_vector;

}



bool HammingTestSuiteClusterPlugin::hamming_row(CSelectionData &data, int index1, int index2, int tolerance, int size){

    int distance = 0;
    for(int a = 0 ; a < size ; a++)
        distance += int( (data.getCoverage()->getBitMatrix().getRow(index1)[a]) ^ (data.getCoverage()->getBitMatrix().getRow(index2)[a]) );

    if( distance < tolerance )return true;

    return false;
}


bool HammingTestSuiteClusterPlugin::hamming_cols(CSelectionData &data, int index1, int index2, int tolerance, int size){

    int distance = 0;
    for(int a = 0 ; a < size ; a++)
        distance += int( (data.getCoverage()->getBitMatrix().getCol(index1)[a]) ^ (data.getCoverage()->getBitMatrix().getCol(index2)[a]) );

    if( distance < tolerance )return true;

    return false;
}



extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteClusterPluginManager().addPlugin(new HammingTestSuiteClusterPlugin());
}

} /* namespace soda */
