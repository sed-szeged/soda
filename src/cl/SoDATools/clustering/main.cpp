
#define BOOST_FILESYSTEM_VERSION 3

#include <fstream>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "data/CSelectionData.h"
#include "engine/CKernel.h"
#include "io/CJsonReader.h"

#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/prettywriter.h"

using namespace std;
using namespace soda;
using namespace soda::io;
using namespace boost::filesystem;
using namespace boost::program_options;

void processJsonFiles(String path);
int loadJsonFiles(String path);
void printPluginNames(const String &type, const std::vector<String> &plugins);
void printHelp();

CKernel kernel;





int main(int argc, char* argv[]) {


        options_description desc("Options");
        desc.add_options()
                ("help,h", "Prints help message");

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (argc < 2) {
            cerr << "[ERROR] There are no arguments!" << endl;
            printHelp();
            return 1;
        }

        if (vm.count("help")) {
            printHelp();
            cout << desc << endl;
            return 0;
        }

        return loadJsonFiles(String(argv[1]));

}

void printHelp()
{

    cout << "Json file:" << endl;
    cout<<"{ \"coverage-data\":\"/home/../sample.coverage.SoDA\","<<endl<<"\"algorithm\":\"hamming\","<<endl<<
          "\"hamming_dist_row\":10,"<<endl<<"\"hamming_dist_cols\":10,"<<endl<<"\"0cluster\":90 }"<<endl;
}




int loadJsonFiles(String path)
{
    if (!exists(path)) {
        cerr << "[ERROR] Invalid configuration file path." << endl;
        return 1;
    }

    if (is_regular_file(path)) {
        processJsonFiles(path);
    } else if (is_directory(path)) {
        directory_iterator endIt;
        for (directory_iterator it(path); it != endIt; it++) {
            if (is_directory(it->status())) {
                loadJsonFiles(it->path().string());
                continue;
            }
            if (!is_regular_file(it->status())) {
                continue;
            }
            if (it->path().extension() != ".json") {
                continue;
            }
            processJsonFiles(it->path().string());
        }
    }
    return 0;
}

void processJsonFiles(String path){

    /*CSelectionData selectionData;
    selectionData.loadCoverage(String(argv[1]));

    std::map<std::string, CClusterDefinition> clusterList;

    rapidjson::Document json_file;

    kernel.getTestSuiteClusterPluginManager().getPlugin("hamming")->init(json_file);
    kernel.getTestSuiteClusterPluginManager().getPlugin("hamming")->execute(selectionData,clusterList);*/



    std::cout << "[INFO] Processing " << path << " configuration file." << std::endl;

    rapidjson::Document reader;
    {
        FILE *in = fopen (path.c_str(), "r");
        char readBuffer[65536];
        rapidjson::FileReadStream is(in, readBuffer, sizeof(readBuffer));
        reader.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);
        fclose(in);
    }

    boost::filesystem::path jsonPath(path);

    std::string clusterAlgorithmName = reader["algorithm"].GetString();
    ITestSuiteClusterPlugin *clusterAlgorithm = kernel.getTestSuiteClusterPluginManager().getPlugin(clusterAlgorithmName);

    String covPath = reader["coverage-data"].GetString();
    if (covPath[0] == '.') {
        covPath = jsonPath.parent_path().string() + "/" + covPath;
    }

    clusterAlgorithm->init(reader);

    CSelectionData *selectionData = new CSelectionData();

    selectionData->loadCoverage(covPath);

    std::map<std::string, CClusterDefinition> clusterList;
    clusterAlgorithm->execute(*selectionData, clusterList);


}
