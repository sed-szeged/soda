#include "Ochiai_Dice_JaccardTestSuiteClusterPlugin.h"
#include "cluster.hpp"

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
    // TODO: TAM-hoz kell majd
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

    // N X N -es matrix letrehozasa
    row_results(data, algorithm_index);

    // M X M -es matrix letrehozasa
    cols_results(data, algorithm_index);

    kMeans_row();

    kMeans_cols();

}



void Ochiai_Dice_JaccardTestSuiteClusterPlugin::row_results(CSelectionData &data, int alg_index){

    int numTC = int(data.getCoverage()->getNumOfTestcases());      // a tesztek szama
    int numCE = int(data.getCoverage()->getNumOfCodeElements());      // a code elemenetek szama

    //std::cout<<"row (size: "<<numTC<<" "<<numCE<<")"<<std::endl;


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

    if(alg_index==0){   // ochiai szamitas
        if( element_1*element_2 != 0 )
            results = metszet_row(data, index_1, index_2, numCE) / (std::sqrt( element_1*element_2));

    } else if(alg_index==1){    // dice szamitas
        if( element_1+element_2 != 0 )
            results = 2*metszet_row(data, index_1, index_2, numCE) / float(element_1+element_2);

    } else if(alg_index==2){    // jaccard
        int unio = unio_row(data, index_1, index_2, numCE);
        if( unio != 0 )
            results = metszet_row(data, index_1, index_2, numCE) / float(unio);

    } else {
        std::cout<<"HIBAS ALG. INDEX"<<std::endl;
        return;
    }
    *(v_pointer+index_2) = results_vs_limit(results,float(limit)) ;
}






void Ochiai_Dice_JaccardTestSuiteClusterPlugin::cols_results(CSelectionData &data, int alg_index){

    int numTC = int(data.getCoverage()->getNumOfTestcases());      // a tesztek szama
    int numCE = int(data.getCoverage()->getNumOfCodeElements());      // a code elemenetek szama

    //std::cout<<"cols (size: "<<numTC<<" "<<numCE<<")"<<std::endl;

    floatColsVectors.resize(numCE, std::vector<float>(numCE));

    for(int index_1 = 0 ; index_1 < numCE ; index_1++){
        int element_1 = int(data.getCoverage()->getBitMatrix().getCol(IndexType(index_1)).count());

        for(int index_2 = 0 ; index_2 < numCE ; index_2++ )
            set_cols_results(data, index_1, index_2, element_1, numTC, alg_index);

    }
}


void Ochiai_Dice_JaccardTestSuiteClusterPlugin::set_cols_results(CSelectionData &data, int index_1, int index_2, int element_1, int numTC, int alg_index){

    int element_2 = int(data.getCoverage()->getBitMatrix().getCol(IndexType(index_2)).count());
    float results=0.0;
    float* v_pointer = floatRowVectors[index_2].data();

    if(alg_index==0){   // ochiai szamitas
        if( element_1*element_2 != 0 )
            results = metszet_cols(data, index_1, index_2, numTC) / (std::sqrt( element_1*element_2));

    } else if(alg_index==1){    // dice szamitas
        if( element_1+element_2 != 0 )
            results = 2*metszet_cols(data, index_1, index_2, numTC) / float(element_1+element_2);

    } else if(alg_index==2){    // jaccard
        int unio = unio_cols(data, index_1, index_2, numTC);
        if( unio != 0 )
            results = metszet_cols(data, index_1, index_2, numTC) / float(unio);

    } else {
        std::cout<<"HIBAS ALG. INDEX"<<std::endl;
        return;
    }

    *(v_pointer+index_1)= results_vs_limit(results,float(limit)) ;
}







// ha a json-ben "limit":-1.0 van akkor a tenyleges ertek kerul bele, kulonben 0 vagy 1 -a limittol fuggoen
float Ochiai_Dice_JaccardTestSuiteClusterPlugin::results_vs_limit( float results, float limit ){
    if ( limit != -1.0 ){
        return results > limit ? 1.0 : 0.0;
    }
    return results;
}





int Ochiai_Dice_JaccardTestSuiteClusterPlugin::metszet_row(CSelectionData &data, int index_1, int index_2, int size){
    int metszet = 0;
    for(int a = 0 ; a < size ; a++)
        metszet += int( int(data.getCoverage()->getBitMatrix().getRow(index_1)[a]) && int(data.getCoverage()->getBitMatrix().getRow(index_2)[a]) );

    return metszet;
}


int Ochiai_Dice_JaccardTestSuiteClusterPlugin::metszet_cols(CSelectionData &data, int index_1, int index_2, int size){
    int metszet = 0;
    for(int a = 0 ; a < size ; a++)
        metszet += int( int(data.getCoverage()->getBitMatrix().getCol(index_1)[a]) && int(data.getCoverage()->getBitMatrix().getCol(index_2)[a]) );

    return metszet;
}





int Ochiai_Dice_JaccardTestSuiteClusterPlugin::unio_row(CSelectionData &data, int index_1, int index_2, int size){
    int unio = 0;
    for(int a = 0 ; a < size ; a++)
        unio += int( int(data.getCoverage()->getBitMatrix().getRow(index_1)[a]) || int(data.getCoverage()->getBitMatrix().getRow(index_2)[a]) );

    return unio;
}


int Ochiai_Dice_JaccardTestSuiteClusterPlugin::unio_cols(CSelectionData &data, int index_1, int index_2, int size){
    int unio = 0;
    for(int a = 0 ; a < size ; a++)
        unio += int( int(data.getCoverage()->getBitMatrix().getCol(index_1)[a]) || int(data.getCoverage()->getBitMatrix().getCol(index_2)[a]) );

    return unio;
}



void Ochiai_Dice_JaccardTestSuiteClusterPlugin::kMeans_row(){

    Clustering::ClusterId num_clusters = cluster_number;
    Clustering::PointId num_points = floatRowVectors[0].size();
    Clustering::Dimensions num_dimensions = floatRowVectors[0].size();
    Clustering::PointsSpace ps_row(num_points, num_dimensions, Ochiai_Dice_JaccardTestSuiteClusterPlugin::floatRowVectors );
    Clustering::Clusters clusters_row(num_clusters, ps_row);

    //std::cout<<floatRowVectors[0].size()<<std::endl;

    clusters_row.k_means();

    Ochiai_Dice_JaccardTestSuiteClusterPlugin::RowIndexVector = clusters_row.points_to_clusters__;

    /*
    int a = 0;
    for (std::vector<ClusterId>::iterator it = RowIndexVector.begin() ; it != RowIndexVector.end(); ++it){
        std::cout<<a<<" - "<<*it<<std::endl;
        a++;
    }
    std::cout<<std::endl;
    */


}

void Ochiai_Dice_JaccardTestSuiteClusterPlugin::kMeans_cols(){

    Clustering::ClusterId num_clusters = cluster_number;
    Clustering::PointId num_points = floatColsVectors[0].size();
    Clustering::Dimensions num_dimensions = floatColsVectors[0].size();
    Clustering::PointsSpace ps_cols(num_points, num_dimensions, Ochiai_Dice_JaccardTestSuiteClusterPlugin::floatColsVectors );
    Clustering::Clusters clusters_cols(num_clusters, ps_cols);

    //std::cout<<floatColsVectors[0].size()<<std::endl;

    clusters_cols.k_means();

    Ochiai_Dice_JaccardTestSuiteClusterPlugin::ColsIndexVector = clusters_cols.points_to_clusters__;


    /*
    int a = 0;
    for (std::vector<ClusterId>::iterator it = ColsIndexVector.begin() ; it != ColsIndexVector.end(); ++it){
        std::cout<<a<<" - "<<*it<<std::endl;
        a++;
    }
    std::cout<<std::endl;
    */

}



extern "C" void registerPlugin(CKernel &kernel)
{
    kernel.getTestSuiteClusterPluginManager().addPlugin(new Ochiai_Dice_JaccardTestSuiteClusterPlugin());
}

} /* namespace soda */
