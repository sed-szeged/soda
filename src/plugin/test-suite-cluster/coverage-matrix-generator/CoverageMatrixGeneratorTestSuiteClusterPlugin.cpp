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

#include "CoverageMatrixGeneratorTestSuiteClusterPlugin.h"
#include "data/CBitMatrix.h"
#include "boost/lexical_cast.hpp"


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

    ones.resize( cluster_number+1, std::vector<int>(cluster_number+1));
    zeros.resize( cluster_number+1, std::vector<int>(cluster_number+1));

    vectorInit();

    clusterIntersection(coverageMatrix);

    metrika1Calc();

    metrika2Calc();

    metrika3Calc();

    metrika4Calc();

}



void CoverageMatrixGeneratorTestSuiteClusterPlugin::addTestCases(CCoverageMatrix* coverageMatrix){
    for(int i = 0 ; i < row_size ; i++){
        std::string tests="test_";
        std::string name = tests.append( boost::lexical_cast<std::string>(i) );
        coverageMatrix->addTestcaseName(name);
    }
}

void CoverageMatrixGeneratorTestSuiteClusterPlugin::addMethods(CCoverageMatrix *coverageMatrix){
    for(int i = 0 ; i < cols_size ; i++){
        std::string methods="methods_";
        std::string name = methods.append( boost::lexical_cast<std::string>(i) );
        coverageMatrix->addCodeElementName(name);
    }

}


void CoverageMatrixGeneratorTestSuiteClusterPlugin::row_index_generate(){
    for(int i = 0 ; i < row_size ; i++){
        int tmp=std::rand()%(cluster_number+1);
        if(tmp>=2){
            row_cluster_index.push_back( std::rand() % tmp );
        } else {
            row_cluster_index.push_back( std::rand() % 3 + (cluster_number-3) );
        }
    }
}

void CoverageMatrixGeneratorTestSuiteClusterPlugin::cols_index_generate(){
    for(int i = 0 ; i < cols_size ; i++){
        int tmp=std::rand()%(cluster_number+1);
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

void CoverageMatrixGeneratorTestSuiteClusterPlugin::vectorInit(){

    for(int i = 0 ; i <= cluster_number ; i++){
        for(int j = 0 ; j <= cluster_number ; j++){
            ones[i][j]=0;
            zeros[i][j]=0;
        }
    }
}

void CoverageMatrixGeneratorTestSuiteClusterPlugin::clusterIntersection(CCoverageMatrix* coverageMatrix){

    for(int i = 0; i < row_size ; i++){
        for(int j = 0 ; j < cols_size ; j++){
            if( coverageMatrix->getBitMatrix().get(IndexType(i),IndexType(j)) ){
                ones[row_cluster_index[i]][cols_cluster_index[j]]++;
            } else {
                zeros[row_cluster_index[i]][cols_cluster_index[j]]++;
            }

        }
    }
}


/************* metrics4 *************
* ("good points") / ("bad points") *
************************************
*/
void CoverageMatrixGeneratorTestSuiteClusterPlugin::metrika4Calc(){


   int global_good = 0, global_bad = 0;
   for(int i = 0 ; i < cluster_number ; i++ ){
       int local_bad = 0, local_good = 0;
       for(int j = 0 ; j < cluster_number ; j++ ){
           if( i==j ){
               local_good += ones[i][j] ;
               local_bad += zeros[i][j];
           } else {
               local_bad += ones[i][j];
               local_good += zeros[i][j];
           }
       }
       global_good += local_good;
       global_bad += local_bad;
       std::cout<<i+1<<". "<<float(local_good)/float(local_bad)<<std::endl;
   }

   std::cout<<"Global metrics4 ((\"good points\") / (\"bad points\")) : "<<float(global_good)/float(global_bad)<<std::endl<<std::endl;
}



/*
************ metrics3 ************
* ("good points") / (sum points) *
**********************************
*/
void CoverageMatrixGeneratorTestSuiteClusterPlugin::metrika3Calc(){


   int global_good = 0, global_all = 0;
   for(int i = 0 ; i < cluster_number ; i++ ){
       int all = 0, good = 0;
       for(int j = 0 ; j < cluster_number ; j++ ){
           if( i==j ){
               good += ones[i][j] ;
           } else {
               good += zeros[i][j];
           }
           all += ones[i][j];
           all += zeros[i][j];
       }
       global_good += good;
       global_all += all;
       std::cout<<i+1<<". "<<float(good)/float(all)<<std::endl;
   }
   std::cout<<"Global metrics3 ((\"good points\") / (sum points)) : "<<float(global_good)/float(global_all)<<std::endl<<std::endl;
}



/*
********************* metrics2 ********************
* [("good points")-("bad points")] / (sum points) *
***************************************************
*/
void CoverageMatrixGeneratorTestSuiteClusterPlugin::metrika2Calc(){


   int global_good = 0, global_all = 0;
   for(int i = 0 ; i < cluster_number ; i++ ){
       int all = 0, good = 0;
       for(int j = 0 ; j < cluster_number ; j++ ){
           if( i==j ){
               good += ones[i][j] ;
               good -= zeros[i][j];
           } else {
               good += zeros[i][j];
               good -= ones[i][j];
           }
           all += ones[i][j];
           all += zeros[i][j];
       }
       global_good += good;
       global_all += all;
       std::cout<<i+1<<". "<<float(good)/float(all)<<std::endl;
   }
   std::cout<<"Global metrics2 ([(\"good points\")-(\"bad points\")] / (sum points)) : "<<float(global_good)/float(global_all)<<std::endl<<std::endl;
}



/*
************** metrics1 **************
* ("good 1 points") / (sum 1 points) *
**************************************
*/
void CoverageMatrixGeneratorTestSuiteClusterPlugin::metrika1Calc(){


   int global_good = 0, global_all = 0 ;
   for(int i = 0 ; i < cluster_number ; i++ ){
       int all = 0, good = 0;
       for(int j = 0 ; j < cluster_number ; j++ ){
               if( i==j )
                   good += ones[i][j] ;
               all += ones[i][j];

       }
       global_good += good;
       global_all += all;
       std::cout<<i+1<<". "<<float(good)/float(all)<<std::endl;
   }

   std::cout<<"Global metrics1 ((\"good 1 points\") / (|1 points|)) : "<<float(global_good)/float(global_all)<<std::endl<<std::endl;
}


extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteClusterPluginManager().addPlugin(new CoverageMatrixGeneratorTestSuiteClusterPlugin());
}

} /* namespace soda */
