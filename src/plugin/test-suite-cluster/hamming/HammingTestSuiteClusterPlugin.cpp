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
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

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
    testClusterDump = doc["test_clusters_dump"].GetString();
    codeElementsClusterDump = doc["codeelement_clusters_dump"].GetString();
}

void HammingTestSuiteClusterPlugin::execute(CSelectionData &data, std::map<std::string, CClusterDefinition>& clusterList)
{
    int numTC = int(data.getCoverage()->getNumOfTestcases());
    int numCE = int(data.getCoverage()->getNumOfCodeElements());
    std::cout<<std::endl<< "TC: "<< numTC << " ; CE: " << numCE << std::endl<< std::endl;


    //std::cout<<"Row clustering..."<<std::endl;
    row_cluster_index = clustering( data.getCoverage(), m_hamm_diff_row, _0cluster_limit );


    // matrix transpose
    CIDManager idManagerTest, idManagerMethod;
    CBitMatrix* bitMatrix = new CBitMatrix();
    CCoverageMatrix* coverageTransposeMatrix = new CCoverageMatrix( &idManagerTest, &idManagerMethod, bitMatrix);


    // original matrix transpose
    matrixTranspose(data, coverageTransposeMatrix, bitMatrix, numTC, numCE);


    // cols clustering
    //std::cout<<"Cols clustering..."<<std::endl;
    cols_cluster_index = clustering( coverageTransposeMatrix, m_hamm_diff_cols, _0cluster_limit);


    // matrix density = | 1 points | / (matrix size)
    matrixDensity( data.getCoverage() );


    // histogram dump
    histogram( data.getCoverage(), false );
    histogram(coverageTransposeMatrix, true);


    // set clusterList
    setClusterList(numTC, numCE, clusterList);


    // clusters dump
    clusterDump(data.getCoverage(), row_cluster_index, testClusterDump, 0);
    clusterDump(data.getCoverage(), cols_cluster_index, codeElementsClusterDump, 1);


    // code elements inform.
    ep_np_ef_nf(data);


}


// code elements inform.
void HammingTestSuiteClusterPlugin::ep_np_ef_nf(CSelectionData &data){

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

        fileName += "EP-NP-EF-NF-"+getName()+"-"+(boost::lexical_cast<std::string>(m_hamm_diff_row))+"-"+(boost::lexical_cast<std::string>(m_hamm_diff_cols))+"-"+
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
                if( it_cov->first == it_res->first ) tcMap[it_cov->second] = it_res->second;
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



void HammingTestSuiteClusterPlugin::setClusterList(int numTC, int numCE, std::map<std::string, CClusterDefinition>& clusterList){

    std::vector<int>::iterator biggest = std::max_element(row_cluster_index.begin(),row_cluster_index.end());

    for(int a= 0 ; a <= *biggest ; a++)
        clusterList[ boost::lexical_cast<std::string>(a) ] = CClusterDefinition();

    for(int i = 0 ; i < numTC ; i++)
        clusterList[ boost::lexical_cast<std::string>( row_cluster_index[i]) ].addTestCase(IndexType(i));
    
    for(int i = 0 ; i < numCE ; i++)
        clusterList[ boost::lexical_cast<std::string>( cols_cluster_index[i]) ].addCodeElement(IndexType(i));
}



// clustering alg.
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



// clusters dump ( -> file )
void HammingTestSuiteClusterPlugin::clusterDump(CCoverageMatrix* data, std::vector<int> labelVector, std::string outFile, int dimension){

    std::vector<int>::iterator biggest = std::max_element( labelVector.begin(), labelVector.end() );
    int size = labelVector.size();
    std::ofstream outClusters;
    outClusters.open(outFile.c_str());

    for(int a = 0 ; a <= *biggest ; a++)
        outClusters << a << ". cluster: "<<std::count(labelVector.begin(),labelVector.end(),a)<<" elements:\n";
    outClusters<<"\n";
    for(int b = 0 ; b < 10 ; b++) outClusters<<"=======";
    outClusters<<"\n\n";

    for(int a = 0 ; a <= *biggest ; a++){
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



// matrix density calc.
void HammingTestSuiteClusterPlugin::matrixDensity(CCoverageMatrix* matrix){

    int size = matrix->getNumOfTestcases();
    int count = 0;
    for(int i = 0 ; i < size ; i++)
        count += matrix->getBitMatrix().getRow(IndexType(i)).count();

    std::cout<<"Matrix density: "<<float(count)/float((size*matrix->getNumOfCodeElements()))<<std::endl<<std::endl;
}



// histogram calc.
void HammingTestSuiteClusterPlugin::histogram(CCoverageMatrix* matrix, bool dimension){

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

    fileName += (boost::lexical_cast<std::string>(m_hamm_diff_row))+"_"+(boost::lexical_cast<std::string>(m_hamm_diff_cols))+"_"+
            (boost::lexical_cast<std::string>(_0cluster_limit))+".csv";

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
    kernel.getTestSuiteClusterPluginManager().addPlugin(new HammingTestSuiteClusterPlugin());
}

} /* namespace soda */
