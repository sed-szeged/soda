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

#include "ClusteringMetricsTestSuiteMetricPlugin.h"

namespace soda {

ClusteringMetricsTestSuiteMetricPlugin::ClusteringMetricsTestSuiteMetricPlugin():m_data(NULL),m_clusterList(NULL)
{
}

ClusteringMetricsTestSuiteMetricPlugin::~ClusteringMetricsTestSuiteMetricPlugin()
{
}

std::string ClusteringMetricsTestSuiteMetricPlugin::getName()
{
    return "clustering-metrics";
}

std::string ClusteringMetricsTestSuiteMetricPlugin::getDescription()
{
    return "Calculating cluster metrics";
}

void ClusteringMetricsTestSuiteMetricPlugin::init(CSelectionData *data, std::map<std::string, CClusterDefinition> *clusterList, IndexType revision)
{
    m_data = data ;
    m_clusterList = clusterList ;
}

std::vector<std::string> ClusteringMetricsTestSuiteMetricPlugin::getDependency()
{
    return std::vector<std::string>();
}

void ClusteringMetricsTestSuiteMetricPlugin::calculate(rapidjson::Document& results)
{

    row_clusters = rowClusterCount();
    cols_clusters = colsClusterCount();
    std::cout<<std::endl<<"Cluster count: rows - "<<row_clusters<<" , cols - "<<cols_clusters<<std::endl<<std::endl;


    ones.resize( row_clusters+1, std::vector<int>(cols_clusters+1));
    zeros.resize( row_clusters+1, std::vector<int>(cols_clusters+1));

    // ones and zeros vectors init
    vectorInit();


    // write cluster-information
    clusterInfo();


    /* 1 and 0 in clusterpairs */
    clusterIntersection();


    /* defining clusterpairs */
    row_clusters >= cols_clusters ? clusterPairsRow( row_clusters, cols_clusters) : clusterPairsCols(row_clusters,cols_clusters);


    /*
     ************** metrics1 **************
     * ("good 1 points") / (sum 1 points) *
     **************************************
     */
    metrika1Calc();


    /*
     ********************* metrics2 ********************
     * [("good points")-("bad points")] / (sum points) *
     ***************************************************
     */
    metrika2Calc();


    /*
     ************ metrics3 ************
     * ("good points") / (sum points) *
     **********************************
     */
    metrika3Calc();


    /*
     ************* metrics4 *************
     * ("good points") / ("bad points") *
     ************************************
     */
    metrika4Calc();


    // origin and sort matrix convert to image
    //imageWrite(m_data);

}



void ClusteringMetricsTestSuiteMetricPlugin::vectorInit(){
    for(int i = 0 ; i <= row_clusters ; i++ ){
        for(int j = 0 ; j <= cols_clusters ; j++){
            ones[i][j]=0;
            zeros[i][j]=0;
        }
    }
}



/*
 ************* metrics4 *************
 * ("good points") / ("bad points") *
 ************************************
 */
void ClusteringMetricsTestSuiteMetricPlugin::metrika4Calc(){

    metrics4.resize(row_clusters+1);

    int global_good = 0, global_bad = 0;
    for(int i = 1 ; i <= row_clusters ; i++ ){
        int local_bad = 0, local_good = 0;
        for(int j = 1 ; j <= cols_clusters ; j++ ){
            if( pairs[i].count(j)!=0 ){
                local_good += ones[i][j] ;
                local_bad += zeros[i][j];
            } else {
                local_bad += ones[i][j];
                local_good += zeros[i][j];
            }
        }
        global_good += local_good;
        global_bad += local_bad;
        metrics4[i] = float(local_good)/float(local_bad);
    }

    std::cout<<"Global metrics4 ((\"good points\") / (\"bad points\")) : "<<float(global_good)/float(global_bad)<<std::endl;

    printVector(metrics4);
    std::cout<<std::endl;
}



/*
 ************ metrics3 ************
 * ("good points") / (sum points) *
 **********************************
 */
void ClusteringMetricsTestSuiteMetricPlugin::metrika3Calc(){

    metrics3.resize(row_clusters+1);

    int global_good = 0, global_all = 0;
    for(int i = 1 ; i <= row_clusters ; i++ ){
        int all = 0, good = 0;
        for(int j = 1 ; j <= cols_clusters ; j++ ){
            if( pairs[i].count(j)!=0 ){
                good += ones[i][j] ;
            } else {
                good += zeros[i][j];
            }
            all += ones[i][j];
            all += zeros[i][j];
        }
        global_good += good;
        global_all += all;
        metrics3[i] = float(good)/float(all);
    }
    std::cout<<"Global metrics3 ((\"good points\") / (sum points)) : "<<float(global_good)/float(global_all)<<std::endl;

    printVector(metrics3);
    std::cout<<std::endl;
}



/*
 ********************* metrics2 ********************
 * [("good points")-("bad points")] / (sum points) *
 ***************************************************
 */
void ClusteringMetricsTestSuiteMetricPlugin::metrika2Calc(){

    metrics2.resize(row_clusters+1);

    int global_good = 0, global_all = 0;
    for(int i = 1 ; i <= row_clusters ; i++ ){
        int all = 0, good = 0;
        for(int j = 1 ; j <= cols_clusters ; j++ ){
            if( pairs[i].count(j)!=0 ){
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
        metrics2[i] = float(good)/float(all);
    }
    std::cout<<"Global metrics2 ([(\"good points\")-(\"bad points\")] / (sum points)) : "<<float(global_good)/float(global_all)<<std::endl;

    printVector(metrics2);
    std::cout<<std::endl;
}



/*
 ************** metrics1 **************
 * ("good 1 points") / (sum 1 points) *
 **************************************
 */
void ClusteringMetricsTestSuiteMetricPlugin::metrika1Calc(){

    metrics1.resize(row_clusters+1);

    int global_good = 0, global_all = 0 ;
    for(int i = 1 ; i <= row_clusters ; i++ ){
        int all = 0, good = 0;
        for(int j = 1 ; j <= cols_clusters ; j++ ){
                if( pairs[i].count(j)!=0 )
                    good += ones[i][j] ;
                all += ones[i][j];

        }
        global_good += good;
        global_all += all;
        metrics1[i] = float(good)/float(all);
    }

    std::cout<<"Global metrics1 ((\"good 1 points\") / (|1 points|)) : "<<float(global_good)/float(global_all)<<std::endl;

    printVector(metrics1);
    std::cout<<std::endl;
}



void ClusteringMetricsTestSuiteMetricPlugin::clusterPairsRow(int size_row, int size_cols){

    std::cout<<std::endl<<"row-cols pairs:"<<std::endl;
    std::set<int> not_excluded_1, not_excluded_2;

    for(int i = 1 ; i <= size_row  ; i++ ){
        not_excluded_1.insert(i);
    }

    for(int i = 1 ; i <= size_cols  ; i++ ){
        not_excluded_2.insert(i);
    }

    for(int a = 1 ; a <= size_row  ; a++ ){
        int max_count = -1 , max_index_j=0, max_index_i=0;

        if( not_excluded_2.empty() ){
            for(int i = 1 ; i <= size_cols  ; i++ ){
                not_excluded_2.insert(i);
            }
        }


        for(int i = 1 ; i <= size_row  ; i++ ){
            for(int j = 1 ; j <= size_cols  ; j++ ){
                if( ones[i][j] > max_count && not_excluded_2.count(j)!=0 && not_excluded_1.count(i)!=0 ){
                    max_count = ones[i][j];
                    max_index_j = j;
                    max_index_i = i;
                }
            }
        }

        std::cout<<max_index_i<<"-"<<max_index_j<<" : "<<max_count<<std::endl;
        not_excluded_2.erase(max_index_j);
        not_excluded_1.erase(max_index_i);

        pairs[max_index_i].insert(max_index_j);
    }
    std::cout<<std::endl<<std::endl;
}



void ClusteringMetricsTestSuiteMetricPlugin::clusterPairsCols(int size_row, int size_cols){

    std::cout<<std::endl<<"row-cols pairs:"<<std::endl;
    std::set<int> not_excluded_1, not_excluded_2;

    for(int i = 1 ; i <= size_row  ; i++ ){
        not_excluded_1.insert(i);
    }

    for(int i = 1 ; i <= size_cols  ; i++ ){
        not_excluded_2.insert(i);
    }

    for(int a = 1 ; a <= size_cols  ; a++ ){
        int max_count = -1 , max_index_j, max_index_i;

        if(not_excluded_1.empty()){
            for(int i = 1 ; i <= size_row  ; i++ )
                not_excluded_1.insert(i);
        }


        for(int i = 1 ; i <= size_row  ; i++ ){
            for(int j = 1 ; j <= size_cols  ; j++ ){
                if( ones[i][j] > max_count && not_excluded_2.count(j)!=0 && not_excluded_1.count(i)!=0 ){
                    max_count = ones[i][j];
                    max_index_j = j;
                    max_index_i = i;
                }
            }
        }

        std::cout<<max_index_i<<"-"<<max_index_j<<" : "<<max_count<<std::endl;
        not_excluded_2.erase(max_index_j);
        not_excluded_1.erase(max_index_i);
        pairs[max_index_i].insert(max_index_j);
    }
    std::cout<<std::endl<<std::endl;
}



void ClusteringMetricsTestSuiteMetricPlugin::clusterIntersection(){

    std::map<int, int> newRowIndexMap, newColsIndexMap;
    int index_row = 1, index_cols = 1;

    for(int i = 0 ; i < m_clusterList->size() ; i++ ){
        if( m_clusterList->operator [](boost::lexical_cast<std::string>(i)).getTestCases().size() > 0 ){
            newRowIndexMap[i]=index_row;
            index_row++;
        }
    }

    for(int i = 0 ; i < m_clusterList->size() ; i++ ){
        if( m_clusterList->operator [](boost::lexical_cast<std::string>(i)).getCodeElements().size() > 0 ){
            newColsIndexMap[i]=index_cols;
            index_cols++;
        }
    }


    for(int i = 0 ; i < m_clusterList->size() ; i++ ){
        std::vector<IndexType> test = m_clusterList->operator [](boost::lexical_cast<std::string>(i)).getTestCases();

        for ( std::vector<IndexType>::iterator it_t = test.begin() ; it_t != test.end(); ++it_t){
            for(int j = 0 ; j < m_clusterList->size() ; j++ ){

                std::vector<IndexType> fn = m_clusterList->operator [](boost::lexical_cast<std::string>(j)).getCodeElements();

                for ( std::vector<IndexType>::iterator it_f = fn.begin() ; it_f != fn.end(); ++it_f)
                    int(m_data->getCoverage()->getBitMatrix().getRow(int(*it_t))[int(*it_f)] ) > 0 ? ones[newRowIndexMap[i]][newColsIndexMap[j]]++ : zeros[newRowIndexMap[i]][newColsIndexMap[j]]++ ;
            }
        }
    }

    std::cout<<"|1| : "<<std::endl;
    for(int i = 0 ; i <= row_clusters ; i++ ){
        for(int j = 0 ; j <= cols_clusters ; j++){
            std::cout<<ones[i][j]<<"\t";
        } std::cout<<std::endl;
    }
    std::cout<<std::endl;


    std::cout<<"|0| : "<<std::endl;
    for(int i = 0 ; i <= row_clusters ; i++ ){
        for(int j = 0 ; j <= cols_clusters ; j++){
            std::cout<<zeros[i][j]<<"\t";
        } std::cout<<std::endl;
    }
}



int ClusteringMetricsTestSuiteMetricPlugin::rowClusterCount(){
    int count = 0;
    for(int i = 0 ; i < m_clusterList->size() ; i++ )
        if ( m_clusterList->operator [](boost::lexical_cast<std::string>(i)).getTestCases().size() > 0 ) count++;

    return count;
}



int ClusteringMetricsTestSuiteMetricPlugin::colsClusterCount(){
    int count = 0;
    for(int i = 0 ; i < m_clusterList->size() ; i++ )
        if ( m_clusterList->operator [](boost::lexical_cast<std::string>(i)).getCodeElements().size() > 0 ) count++;

    return count;
}



void ClusteringMetricsTestSuiteMetricPlugin::printVector( std::vector<float> metrika ){

    for(int i = 0 ; i<= row_clusters ; i++)
        std::cout<<i<<". value: "<<metrika[i]<<std::endl;
}



void ClusteringMetricsTestSuiteMetricPlugin::imageWrite(CSelectionData *data){

    int sor = int(data->getCoverage()->getNumOfTestcases());
    int oszlop = int(data->getCoverage()->getNumOfCodeElements());

    std::ofstream eredeti;
    eredeti.open ("/home/user/Asztal/image_original.pgm");
    eredeti<<"P2"<<std::endl;
    eredeti<<oszlop<<" "<<sor<<std::endl<<"255"<<std::endl;
    for (int i = 0; i < sor; i++){
        for(int j=0;j<oszlop;j++){
            if( data->getCoverage()->getBitMatrix().get(i,j) ){
                eredeti<<"0"<<std::endl;
            } else {
                eredeti<<"255"<<std::endl;
            }
        }
    }

    std::ofstream rendezett;
    rendezett.open ("/home/user/Asztal/image_sort.pgm");
    rendezett<<"P2"<<std::endl;
    rendezett<<oszlop<<" "<<sor<<std::endl<<"255"<<std::endl;

    std::vector<IndexType> tmp_row(sor);
    std::vector<IndexType> tmp_cols(oszlop);

    int sorindex=0;
    for(int a=0 ; a <= row_clusters ; a++){
        std::vector<IndexType> tc = m_clusterList->operator [](boost::lexical_cast<std::string>(a)).getTestCases();
        for( std::vector<IndexType>::iterator it = tc.begin() ; it != tc.end() ; ++it){
            tmp_row[sorindex]=*it;
            sorindex++;
        }
    }

    int oszlopindex=0;
    for(int a=0 ; a <= cols_clusters ; a++){
        std::vector<IndexType> ce = m_clusterList->operator [](boost::lexical_cast<std::string>(a)).getCodeElements();
        for( std::vector<IndexType>::iterator it = ce.begin() ; it != ce.end() ; ++it){
            tmp_cols[oszlopindex]=*it;
            oszlopindex++;
        }
    }

    for(std::vector<IndexType>::iterator it_sor=tmp_row.begin() ; it_sor!=tmp_row.end() ; ++it_sor){
        for(std::vector<IndexType>::iterator it_oszlop=tmp_cols.begin() ; it_oszlop!=tmp_cols.end() ; ++it_oszlop){
            if( data->getCoverage()->getBitMatrix().get(*it_sor,*it_oszlop) ){
                rendezett<<"0"<<std::endl;
            } else {
                rendezett<<"255"<<std::endl;
            }
        }
    }

}



void ClusteringMetricsTestSuiteMetricPlugin::clusterInfo(){
    int _write_local_row_index = 1, _write_local_cols_index = 1;
    for(int i = 0 ; i < m_clusterList->size() ; i++ ){
        if(m_clusterList->operator [](boost::lexical_cast<std::string>(i)).getTestCases().size() > 0){
            std::cout<<_write_local_row_index<<". row-cluster : "<<m_clusterList->operator [](boost::lexical_cast<std::string>(i)).getTestCases().size()<<" test cases"<<std::endl;
            _write_local_row_index++;
        }
    }std::cout<<std::endl<<std::endl;

    for(int i = 0 ; i < m_clusterList->size() ; i++ ){
        if(m_clusterList->operator [](boost::lexical_cast<std::string>(i)).getCodeElements().size() > 0){
            std::cout<<_write_local_cols_index<<". cols-cluster : "<<m_clusterList->operator [](boost::lexical_cast<std::string>(i)).getCodeElements().size()<<" test cases"<<std::endl;
            _write_local_cols_index++;
        }
    }std::cout<<std::endl<<std::endl;

}



extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteMetricPluginManager().addPlugin(new ClusteringMetricsTestSuiteMetricPlugin());
}

} /* namespace soda */
