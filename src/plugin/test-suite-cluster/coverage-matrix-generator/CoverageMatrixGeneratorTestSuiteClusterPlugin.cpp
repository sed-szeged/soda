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

#include "CoverageMatrixGeneratorTestSuiteClusterPlugin.h"
#include "data/CBitMatrix.h"

namespace soda {

CoverageMatrixGeneratorTestSuiteClusterPlugin::CoverageMatrixGeneratorTestSuiteClusterPlugin()
{
}

CoverageMatrixGeneratorTestSuiteClusterPlugin::~CoverageMatrixGeneratorTestSuiteClusterPlugin()
{
}

std::string CoverageMatrixGeneratorTestSuiteClusterPlugin::getName()
{
    return "matrix-generator";
}

std::string CoverageMatrixGeneratorTestSuiteClusterPlugin::getDescription()
{
    return "matrix-generator";
}

std::map<String, String> CoverageMatrixGeneratorTestSuiteClusterPlugin::getRequiredParameters()
{
    return std::map<String, String>();
}

void CoverageMatrixGeneratorTestSuiteClusterPlugin::init(rapidjson::Document &doc)
{
    matrix_name = doc["coverage-data"].GetString();
    row_size = doc["row-size"].GetInt();
    cols_size = doc["cols-size"].GetInt();
    cluster_number = doc["cluster-number"].GetInt();
    in_count = doc["in(%)"].GetInt();
    out_count = doc["out(%)"].GetInt();

}

void CoverageMatrixGeneratorTestSuiteClusterPlugin::execute(CSelectionData &data, std::map<std::string, CClusterDefinition>& clusterList)
{
    CIDManager idManagerTest;
    CIDManager idManagerMethod;
    CBitMatrix* bitMatrix = new CBitMatrix();
    CCoverageMatrix* coverageMatrix = new CCoverageMatrix( &idManagerTest, &idManagerMethod, bitMatrix);

    std::srand (std::time(NULL));

    // add TC
    CoverageMatrixGeneratorTestSuiteClusterPlugin::addTestCases(coverageMatrix);

    // add M
    CoverageMatrixGeneratorTestSuiteClusterPlugin::addMethods(coverageMatrix);

    coverageMatrix->refitMatrixSize();

    // cluster-index generate
    CoverageMatrixGeneratorTestSuiteClusterPlugin::row_index_generate();
    CoverageMatrixGeneratorTestSuiteClusterPlugin::cols_index_generate();

    // matrix value settings
    CoverageMatrixGeneratorTestSuiteClusterPlugin::matrix_set(bitMatrix);


    coverageMatrix->save(matrix_name);

    /*****
    for(int i = 0 ; i < row_size ; i++){
        for(int j = 0 ; j < cols_size ; j++){
            std::cout<<bitMatrix->get(i,j)<<" ";
        }std::cout<<std::endl;
    }
    *******/

    /***********
    for(int i = 0 ; i < row_size ; i++){
        std::cout<<coverageMatrix->getTestcases().getValue(i)<<std::endl;
    }

    for(int j = 0 ; j < cols_size ; j++){
        std::cout<<coverageMatrix->getCodeElements().getValue(j)<<std::endl;
    }
    ******************/


}



void CoverageMatrixGeneratorTestSuiteClusterPlugin::addTestCases(CCoverageMatrix* coverageMatrix){
    for(int i = 0 ; i < row_size ; i++){
        std::string tests="test_";
        std::ostringstream oss;
        oss<<i;
        std::string tmp=oss.str();
        std::string name = tests.append(tmp);
        coverageMatrix->addTestcaseName(name);
    }

}

void CoverageMatrixGeneratorTestSuiteClusterPlugin::addMethods(CCoverageMatrix *coverageMatrix){
    for(int i = 0 ; i < cols_size ; i++){
        std::string methods="methods_";
        std::ostringstream oss;
        oss<<i;
        std::string tmp=oss.str();
        std::string name = methods.append(tmp);
        coverageMatrix->addCodeElementName(name);
    }

}


void CoverageMatrixGeneratorTestSuiteClusterPlugin::row_index_generate(){
    for(int i = 0 ; i < row_size ; i++){
        int tmp=std::rand()%cluster_number;
        if(tmp>=2){
            row_cluster_index.push_back( std::rand() % tmp );
        } else {
            row_cluster_index.push_back( std::rand() % 3 + (cluster_number-3) );
        }
    }
}

void CoverageMatrixGeneratorTestSuiteClusterPlugin::cols_index_generate(){
    for(int i = 0 ; i < cols_size ; i++){
        int tmp=std::rand()%cluster_number;
        if(tmp>=2){
            cols_cluster_index.push_back( std::rand() % tmp );
        } else {
            cols_cluster_index.push_back( std::rand() % 3 + (cluster_number-3) );
        }
    }
}


void CoverageMatrixGeneratorTestSuiteClusterPlugin::matrix_set( CBitMatrix* bitMatrix){

    for(int i = 0 ; i < row_size ; i++){
        for(int j=0 ; j < cols_size ; j++){

            if( row_cluster_index[i] == cols_cluster_index[j] ){

                if ( ( std::rand() % 100) <= in_count ){
                    bitMatrix->set(i,j,true);
                } else {
                    bitMatrix->set(i,j,false);
                }

            } else {

                if ( (std::rand() % 100) <= out_count ){
                    bitMatrix->set(i,j,true);
                } else {
                    bitMatrix->set(i,j,false);
                }

            }
        }
    }
}



extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteClusterPluginManager().addPlugin(new CoverageMatrixGeneratorTestSuiteClusterPlugin());
}

} /* namespace soda */
