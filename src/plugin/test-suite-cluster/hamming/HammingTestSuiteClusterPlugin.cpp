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
    // TODO: TAM-hoz kell majd
    return std::map<String, String>();
}


// Json-bol valo "parameter-inicializalas"
void HammingTestSuiteClusterPlugin::init(rapidjson::Document &doc)
{
    // ezek lesznek majd a parameterek
    m_hamm_diff_row = doc["hamming_dist_row(%)"].GetInt();
    m_hamm_diff_cols = doc["hamming_dist_cols(%)"].GetInt();
    _0cluster_limit = doc["0cluster(%)"].GetInt();
}

void HammingTestSuiteClusterPlugin::execute(CSelectionData &data, std::map<std::string, CClusterDefinition>& clusterList)
{
    int numTC = int(data.getCoverage()->getNumOfTestcases());      // a tesztek szama
    int numCE = int(data.getCoverage()->getNumOfCodeElements());      // a code elemenetek szama

    std::cout<< "TC: "<< numTC << " ; CE: " << numCE << std::endl;

    row_cluster_index = clustering_row(data,numTC);

    cols_cluster_index = clustering_cols(data,numCE);

}



std::vector<int> HammingTestSuiteClusterPlugin::clustering_row(CSelectionData &data, int size ){

    int actual_index = 0;  // az aktualis klasztercimket mutatja
    int tolerance = ( size * m_hamm_diff_row ) / 100 ; // a Hamming toleranciat mutatja
    int _0cluster_count = 0;
    std::vector<int> clusterindex_vector(size,-1);   // seged-cimke-vektor

    std::cout<<"Hamm row tol.: "<<tolerance<<std::endl;

    for(int index_1 = 0 ; index_1 < size ; index_1++ ){

        int element_1 = data.getCoverage()->getBitMatrix().getRow(IndexType(index_1)).count();

        if( element_1 > _0cluster_limit ){  // ha sok elem van benne akkor ...
            _0cluster_count++;
            clusterindex_vector[index_1] = 0; // ... a 0klaszterbe kerul

        } else if ( clusterindex_vector[index_1] == -1 ) {    // ha meg cimkezetlen ...

            actual_index++;
            clusterindex_vector[index_1] = actual_index; // ... megcimkezzuk az egyiket ...
            for(int index_2 = index_1 ; index_2 < size ; index_2++ ){

                int element_2 = data.getCoverage()->getBitMatrix().getRow(IndexType(index_2)).count();

                // ... es ha kelloen kozel van egy masik elemhez ...
                if( index_1 != index_2 && clusterindex_vector[index_2] == -1 && abs(element_1-element_2) < tolerance )
                    if( hamming_row(data, index_1, index_2, tolerance, size) ) clusterindex_vector[index_2] = actual_index; // ... akkor azonos lesz a cimkejuk

            }
        }
    }

    std::cout<<"cluster count: "<< actual_index << " ; 0cluster count: "<<_0cluster_count<<std::endl;
    return clusterindex_vector;

}

std::vector<int> HammingTestSuiteClusterPlugin::clustering_cols(CSelectionData &data, int size ){

    int actual_index = 0;  // az aktualis klasztercimket mutatja
    int tolerance = ( size * m_hamm_diff_cols ) / 100 ;
    int _0cluster_count = 0;
    std::vector<int> clusterindex_vector(size,-1);   // seged-cimke-vektor

    std::cout<<"Hamm cols tol.: "<<tolerance<<std::endl;

    for(int index_1 = 0 ; index_1 < size ; index_1++ ){

        int element_1 = data.getCoverage()->getBitMatrix().getCol(IndexType(index_1)).count();
        if( element_1 > _0cluster_limit ){  // ha sok elem van benne akkor ...
            clusterindex_vector[index_1] = 0; // ... a 0klaszterbe kerul
            _0cluster_count++;
        } else if ( clusterindex_vector[index_1] == -1 ) {    // ha meg cimkezetlen ...

            actual_index++;
            clusterindex_vector[index_1] = actual_index; // ... megcimkezzuk az egyiket ...

            for(int index_2 = index_1 ; index_2 < size ; index_2++ ){
                int element_2 = data.getCoverage()->getBitMatrix().getCol(IndexType(index_2)).count();
                // ... es ha kelloen kozel van egy masik elemhez ...
                if( index_1 != index_2 && clusterindex_vector[index_2] == -1 && abs(element_1-element_2) < tolerance )
                    if( hamming_cols(data, index_1, index_2, tolerance, size) ) clusterindex_vector[index_2] = actual_index; // ... akkor azonos lesz a cimkejuk

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
