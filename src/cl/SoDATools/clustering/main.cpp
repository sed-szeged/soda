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


#define BOOST_FILESYSTEM_VERSION 3

#include <fstream>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "data/CSelectionData.h"
#include "engine/CKernel.h"
#include "io/CJsonReader.h"
#include "data/CClusterDefinition.h"

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
std::map<std::string, CClusterDefinition> clusterList;



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

    cout << "Json file:" << endl<<endl;
    cout<<"{ \"coverage-data\":\"/home/../sample.coverage.SoDA\","<<endl<<"\"algorithm\":\"hamming\","<<endl<<
          "\"hamming_dist_row(%)\":10,"<<endl<<"\"hamming_dist_cols(%)\":10,"<<endl<<"\"0cluster(%)\":90 }"<<endl<<endl<<"or"<<endl<<endl;


    cout<<"{ \"coverage-data\":\"/home/.../sample.coverage.SoDA\","<<endl<<"\"algorithm\":\"ochiai-dice-jaccard\","<<endl<<
          "\"alg.index\":2,"<<endl<<"\"limit\":0.3,"<<endl<<"\"0cluster(%)\":90,"<<endl<<"\"cluster-number\":3}"<<endl<<endl;
    cout<<"index: 0 - ochiai ; 1 - dice ; 2 - jaccard and limit=-1.0 == no limit"<<endl<<endl<<"or"<<endl<<endl;


    cout<<"{ \"coverage-data\":\"/home/../output.coverage.SoDA\","<<endl<<"\"algorithm\":\"matrix-generator\","<<endl<<
          "\"row-size\":100,"<<endl<<"\"cols-size\":200,"<<endl<<"\"cluster-number\":5,"<<endl<<"\"in(%)\":95,"<<endl<<
          "\"out(%)\":10 }"<<endl<<endl;
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

    std::cout<<clusterAlgorithmName<<std::endl;

    ITestSuiteClusterPlugin *clusterAlgorithm = kernel.getTestSuiteClusterPluginManager().getPlugin(clusterAlgorithmName);

    String covPath = reader["coverage-data"].GetString();
    if (covPath[0] == '.') {
        covPath = jsonPath.parent_path().string() + "/" + covPath;
    }

    clusterAlgorithm->init(reader);

    CSelectionData *selectionData = new CSelectionData();


    if( clusterAlgorithmName != "matrix-generator" )
        selectionData->loadCoverage(covPath);

    std::map<std::string, CClusterDefinition> clusterList;

    clusterAlgorithm->execute(*selectionData, clusterList);


    /* metrics calc. */

    if( clusterAlgorithmName != "matrix-generator" ){

        IndexType revision = 1;
        rapidjson::Document ures_doc;

        ITestSuiteMetricPlugin *metric = kernel.getTestSuiteMetricPluginManager().getPlugin("clustering-metrics");
        metric->init(selectionData, &clusterList, revision);

        metric->calculate(ures_doc);

    }


}
