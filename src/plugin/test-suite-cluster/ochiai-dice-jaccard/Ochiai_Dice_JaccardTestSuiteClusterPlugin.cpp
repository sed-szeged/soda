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
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

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
    _0cluster_limit = doc["0cluster(%)"].GetInt();
    testClusterDump = doc["test_clusters_dump"].GetString();
    codeElementsClusterDump = doc["codeelement_clusters_dump"].GetString();
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
    //std::cout<<"... on rows..."<<std::endl;
    clusters(data.getCoverage(), algorithm_index , false);


    // matrix transpose
    CIDManager idManagerTest, idManagerMethod;
    CBitMatrix* bitMatrix = new CBitMatrix();
    CCoverageMatrix* coverageTransposeMatrix = new CCoverageMatrix( &idManagerTest, &idManagerMethod, bitMatrix);
    matrixTranspose(data, coverageTransposeMatrix, bitMatrix, numTC, numCE);


    // CodeElements X CodeElements matrix calc.
    //std::cout<<"... on cols..."<<std::endl;
    clusters(coverageTransposeMatrix, algorithm_index , true);


    // matrix density = | 1 points | / (matrix size)
    matrixDensity( data.getCoverage() );


    // histogram dump
    histogram( data.getCoverage(), false );
    histogram(coverageTransposeMatrix, true);


    // k-means alg. on (row) x (row) matrix
    //std::cout<<std::endl<<"k-means running on row..."<<std::endl;
    kMeans(data.getCoverage(),false);


    // k-means alg. on (cols) x (cols) matrix
    //std::cout<<"... and on cols"<<std::endl;
    kMeans(coverageTransposeMatrix,true);



    //std::cout<<std::endl<<"Set clusterlist"<<std::endl;
    setClusterList( numTC, numCE, clusterList);


    // clusters dump
    clusterDump(data.getCoverage(), RowIndexVector, testClusterDump, false );
    clusterDump(data.getCoverage(), ColsIndexVector, codeElementsClusterDump, true);

    // code elements inform.
    ep_np_ef_nf(data);

}


// code elements inform.
void Ochiai_Dice_JaccardTestSuiteClusterPlugin::ep_np_ef_nf(CSelectionData &data){

    int covSize = data.getCoverage()->getNumOfTestcases();
    int resSize = data.getResults()->getNumOfTestcases();


    IntVector revisions = data.getResults()->getRevisionNumbers();

    // calc. all revision
    for(IntVector::iterator rev_it = revisions.begin() ; rev_it != revisions.end() ; ++rev_it ){

        // create output file name
        std::ofstream outEP_NP_EF_NF;
        std::string dumpPath(codeElementsClusterDump);
        std::string fileName;
        std::vector<std::string> strs;
        boost::split(strs,dumpPath,boost::is_any_of("/"));
        for(int i = 0 ; i < strs.size()-1;i++)
            fileName += (strs[i]+"/");

        fileName += "EP-NP-EF-NF-"+getName()+"-"+(boost::lexical_cast<std::string>(algorithm_index))+"-"+(boost::lexical_cast<std::string>(cluster_number))+"-"+
                (boost::lexical_cast<std::string>(_0cluster_limit))+"-rev_"+boost::lexical_cast<std::string>(int(*rev_it))+".txt";



        outEP_NP_EF_NF.open(fileName.c_str());
        outEP_NP_EF_NF<<"passedCovered;passedNotCovered;failedCovered;failedNotCovered;"<<
                        "failedCovered/passedCovered;failedNotCovered/passedNotCovered;F/P;failedCovered/(passedCovered+failedCovered)\n";


        // the diff. between coverage and results (test cases)
        std::set<std::string> covTCSet;
        std::map<std::string,IndexType> covTCMap;
        for( int i = 0 ; i < covSize ; i++ ){
            std::string covTCName = data.getCoverage()->getTestcases().getValue(IndexType(i));
            covTCSet.insert( covTCName );
            covTCMap[ covTCName ] = data.getCoverage()->getTestcases().getID( covTCName );
        }


        std::set<std::string> resTCSet;
        std::map<std::string,IndexType> resTCMap;
        for( int i = 0 ; i < resSize ; i++ ){
            std::string resTCName = data.getResults()->getTestcases().getValue(IndexType(i));
            resTCSet.insert(resTCName);
            resTCMap[resTCName] = data.getResults()->getTestcases().getID(resTCName);
        }


        // the diff:
        std::set<std::string> covResDiff;
        std::set_difference(covTCSet.begin(), covTCSet.end(), resTCSet.begin(), resTCSet.end(), std::inserter(covResDiff, covResDiff.begin()) );
        //std::cout<<"In cov, out res: "<<covResDiff.size()<<std::endl;


        for(std::set<std::string>::iterator it = covResDiff.begin() ; it != covResDiff.end() ; ++it)
            covTCMap.erase( *it );


        // cov. - res. index pairs
        std::map<IndexType, IndexType> tcMap;
        for(std::map<std::string,IndexType>::iterator it_res=resTCMap.begin() ; it_res!=resTCMap.end() ; ++it_res){
            for(std::map<std::string,IndexType>::iterator it_cov=covTCMap.begin() ; it_cov!=covTCMap.end() ; ++it_cov){
                if( it_cov->first==it_res->first ) tcMap[it_cov->second] = it_res->second;
            }
        }


        IntVector codeElementIds = data.getCoverage()->getCodeElements().getIDList();
        IntVector testCaseIds;
        for(std::map<std::string,IndexType>::iterator it_cov=covTCMap.begin() ; it_cov!=covTCMap.end() ; ++it_cov)
            testCaseIds.push_back(it_cov->second);


        // calculate
        for (IndexType i = 0; i < codeElementIds.size(); i++) {

            IndexType cid = codeElementIds[i];
            IndexType failedCovered = 0, passedCovered = 0, failedNotCovered = 0, passedNotCovered = 0;

            for (IndexType j = 0; j < testCaseIds.size(); j++) {

                IndexType tcid = testCaseIds[j];
                IndexType tcidInResults = tcMap[tcid];

                if ( data.getResults()->getExecutionBitList(*rev_it).at(tcidInResults)) {

                    bool isPassed = data.getResults()->getPassedBitList(*rev_it).at(tcidInResults);
                    bool isCovered = data.getCoverage()->getBitMatrix().get(tcid, cid);

                    if (isCovered) {
                        if (isPassed) {
                            passedCovered++;
                        } else {
                            failedCovered++;
                        }
                    } else {
                        if (isPassed) {
                            passedNotCovered++;
                        } else {
                            failedNotCovered++;
                        }
                    }
                }
            }

            float efPERep = 0.0 , nfPERnp = 0.0 , FPERP = 0.0 , efPERe;

            if( passedCovered > 0 ) efPERep = float(failedCovered) / float(passedCovered);

            if( passedNotCovered > 0 ) nfPERnp = float(failedNotCovered) / float(passedNotCovered);

            if( (passedCovered + passedNotCovered) > 0 ) FPERP = float( failedCovered + failedNotCovered ) / float( passedCovered + passedNotCovered );

            if( (passedCovered + failedCovered) > 0 ) efPERe = float(failedCovered) / float( passedCovered + failedCovered );

            outEP_NP_EF_NF<<passedCovered<<";"<<passedNotCovered<<";"<<failedCovered<<";"<<failedNotCovered<<";"<<
                         efPERep<<";"<<nfPERnp<<";"<<FPERP<<";"<<efPERe<<"\n";

        }
    }
}


float Ochiai_Dice_JaccardTestSuiteClusterPlugin::results_vs_limit( float results, float limit ){
    if ( limit != -1.0 )
        return results > limit ? 1.0 : 0.0;

    return results;
}


void Ochiai_Dice_JaccardTestSuiteClusterPlugin::kMeans(CCoverageMatrix *matrix, bool dimension){

    Clustering::ClusterId num_clusters = cluster_number;
    Clustering::PointId num_points;
    Clustering::Dimensions num_dimensions;

    if( !dimension ){
        num_points = floatRowVectors[0].size();
        num_dimensions = floatRowVectors[0].size();
    } else {
        num_points = floatColsVectors[0].size();
        num_dimensions = floatColsVectors[0].size();
    }

    int size = matrix->getBitMatrix().getNumOfCols();
    int position = 0;

    if( !dimension ){

        Clustering::PointsSpace ps(num_points, num_dimensions, floatRowVectors );
        Clustering::Clusters clusters(num_clusters, ps);

        clusters.k_means();

        RowIndexVector = clusters.points_to_clusters__;

        for (std::vector<ClusterId>::iterator it = RowIndexVector.begin() ; it != RowIndexVector.end(); ++it){
            if( matrix->getBitMatrix().getRow(IndexType(position)).count() > (size*_0cluster_limit/100) ){
                *it = ClusterId(0); // add to 0cluster
            } else {
                *it = (*it)+1;
            }
            position++;
        }

    } else {

        Clustering::PointsSpace ps(num_points, num_dimensions, floatColsVectors );
        Clustering::Clusters clusters(num_clusters, ps);

        clusters.k_means();

        ColsIndexVector = clusters.points_to_clusters__;

        for (std::vector<ClusterId>::iterator it = ColsIndexVector.begin() ; it != ColsIndexVector.end(); ++it){
            if( matrix->getBitMatrix().getRow(IndexType(position)).count() > (size*_0cluster_limit/100) ){
                *it = ClusterId(0); // add to 0cluster
            } else {
                *it = (*it)+1;
            }
            position++;
        }
    }


}



void Ochiai_Dice_JaccardTestSuiteClusterPlugin::setClusterList(int numTC, int numCE, std::map<std::string, CClusterDefinition>& clusterList){

    std::vector<ClusterId>::iterator maxIndex = std::max_element(RowIndexVector.begin(),RowIndexVector.end());

    for(int a = 0 ; a <= int(*maxIndex) ; a++)
        clusterList[ boost::lexical_cast<std::string>(a) ] = CClusterDefinition();

    for(int i = 0 ; i < numTC ; i++)
        clusterList[ boost::lexical_cast<std::string>( int(RowIndexVector[i])) ].addTestCase(IndexType(i));

    for(int i = 0 ; i < numCE ; i++)
        clusterList[ boost::lexical_cast<std::string>( int(ColsIndexVector[i])) ].addCodeElement(IndexType(i));

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



void Ochiai_Dice_JaccardTestSuiteClusterPlugin::clusterDump(CCoverageMatrix* data, std::vector<ClusterId> labelVector, std::string outFile, bool dimension){

    std::vector<ClusterId>::iterator biggest = std::max_element( labelVector.begin(), labelVector.end() );
    int size = labelVector.size();
    std::ofstream outClusters;
    outClusters.open(outFile.c_str());

    for(int a = 0 ; a <= int(*biggest) ; a++){
        outClusters << a << ". cluster elements ("<<std::count(labelVector.begin(),labelVector.end(),a)<<") :\n";
        for ( int index = 0 ; index < size ; index++ ){
            if( !dimension ){
                if( labelVector[index] == a )
                    outClusters << "\t" << data->getTestcases().getValue(IndexType(index)) << "\n" ;
            } else {
                if( labelVector[index] == a )
                    outClusters << "\t" << data->getCodeElements().getValue(IndexType(index)) << "\n" ;
            }
        }
        outClusters<<"\n";
        for(int b = 0 ; b < 10 ; b++) outClusters<<"=======";
        outClusters<<"\n\n";
    }

}


void Ochiai_Dice_JaccardTestSuiteClusterPlugin::clusters(CCoverageMatrix* matrix, int algorithm_index, bool dimension){

    int numTC = int(matrix->getNumOfTestcases());
    int numCE = int(matrix->getNumOfCodeElements());

    if( !dimension ){
        floatRowVectors.resize(numTC, std::vector<float>(numTC));
    } else {
        floatColsVectors.resize(numTC, std::vector<float>(numTC));
    }

    for(int index_1 = 0 ; index_1 < numTC ; index_1++){
        int element_1 = int( matrix->getBitMatrix().getRow(IndexType(index_1)).count() );

        for(int index_2 = 0 ; index_2 < numTC ; index_2++ )
            set_results(matrix, index_1, index_2, element_1, numCE, algorithm_index, dimension);

    }

}

void Ochiai_Dice_JaccardTestSuiteClusterPlugin::set_results(CCoverageMatrix* matrix, int index_1, int index_2, int element_1, int numCE,  int algorithm_index, bool dimension){

    int element_2 = int(matrix->getBitMatrix().getRow(IndexType(index_2)).count());

    float* v_pointer;

    if( !dimension ){
        v_pointer = floatRowVectors[index_1].data();
    } else {
        v_pointer = floatColsVectors[index_1].data();
    }

    float results=0.0;

    if(algorithm_index==0){   // ochiai calc.
        if( element_1*element_2 != 0 )
            results = intersections(matrix, index_1, index_2, numCE) / (std::sqrt( element_1*element_2));

    } else if(algorithm_index==1){    // dice calc.
        if( element_1+element_2 != 0 )
            results = 2*intersections(matrix, index_1, index_2, numCE) / float(element_1+element_2);

    } else if(algorithm_index==2){    // jaccard calc.
        int unions = unionsCalc(matrix, index_1, index_2, numCE);
        if( unions != 0 )
            results = intersections(matrix, index_1, index_2, numCE) / float(unions);

    } else {
        std::cout<<"Error"<<std::endl;
        return;
    }

    *(v_pointer+index_2) = results_vs_limit(results,float(limit)) ;

}



int Ochiai_Dice_JaccardTestSuiteClusterPlugin::intersections(CCoverageMatrix* matrix, int index_1, int index_2, int size){
    int intersections = 0;
    for(int a = 0 ; a < size ; a++)
        intersections += int( matrix->getBitMatrix().get(IndexType(index_1),IndexType(a)) && matrix->getBitMatrix().get(IndexType(index_2),IndexType(a)) );

    return intersections;
}


int Ochiai_Dice_JaccardTestSuiteClusterPlugin::unionsCalc(CCoverageMatrix* matrix, int index_1, int index_2, int size){
    int unions = 0;
    for(int a = 0 ; a < size ; a++)
        unions += int( matrix->getBitMatrix().get(IndexType(index_1),IndexType(a)) || matrix->getBitMatrix().get(IndexType(index_2),IndexType(a)) );

    return unions;
}

void Ochiai_Dice_JaccardTestSuiteClusterPlugin::matrixDensity(CCoverageMatrix* matrix){

    int size = matrix->getNumOfTestcases();
    int count = 0;
    for(int i = 0 ; i < size ; i++){
        count += matrix->getBitMatrix().getRow(IndexType(i)).count();
    }
    std::cout<<"Matrix density: "<<float(count)/float((size*matrix->getNumOfCodeElements()))<<std::endl<<std::endl;
}


void Ochiai_Dice_JaccardTestSuiteClusterPlugin::histogram(CCoverageMatrix* matrix, bool dimension){

    std::ofstream outHistogram;
    std::string dumpPath(testClusterDump);
    std::string fileName;
    std::vector<std::string> strs;
    boost::split(strs,dumpPath,boost::is_any_of("/"));

    for(int i = 0 ; i < strs.size()-1;i++)
        fileName += (strs[i]+"/");

    if(dimension){
        fileName += "histogram_"+getName()+"_cols_";
    } else {
        fileName += "histogram_"+getName()+"_rows_";
    }

    fileName += (boost::lexical_cast<std::string>(algorithm_index))+"_"+(boost::lexical_cast<std::string>(limit))+"_"+
            (boost::lexical_cast<std::string>(_0cluster_limit))+"_"+(boost::lexical_cast<std::string>(cluster_number))+".csv";
    outHistogram.open(fileName.c_str());

    std::map<int,int> frequency;

    int size = matrix->getNumOfTestcases();
    for(int i = 0 ; i < size ; i++)
        frequency[matrix->getBitMatrix().getRow(IndexType(i)).count()]++;

    for (std::map<int,int>::iterator it=frequency.begin(); it!=frequency.end(); ++it)
        outHistogram << it->first <<" ; "<< it->second << "\n";

}

extern "C" MSDLL_EXPORT void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteClusterPluginManager().addPlugin(new Ochiai_Dice_JaccardTestSuiteClusterPlugin());
}

} /* namespace soda */

