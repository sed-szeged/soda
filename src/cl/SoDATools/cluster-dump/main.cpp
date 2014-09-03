/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Tengeri <dtengeri@inf.u-szeged.hu>
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

#include "engine/CKernel.h"
#include "data/CSelectionData.h"

#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/prettywriter.h"

using namespace soda;
using namespace soda::io;
using namespace boost::filesystem;
using namespace boost::program_options;

CKernel kernel;
std::map<std::string, CClusterDefinition> clusterList;
std::string outputDir;

void processJsonFiles(String path);
int loadJsonFiles(String path);
void printPluginNames(const String &type, const std::vector<String> &plugins);
void printHelp();
std::string getJsonString();
void createJsonFile();

int main(int argc, char *argv[])
{
    std::cout << "cluster dump (SoDA tool)" << std::endl;

    options_description desc("Options");
    desc.add_options()
            ("help,h", "Prints help message")
            ("create-json-file,j", "Creates a sample json file")
            ("list-cluster-algorithms,c", "Lists the cluster algorithms");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (argc < 2) {
        std::cerr << "[ERROR] There are no arguments!" << std::endl;
        printHelp();
        return 1;
    }

    if (vm.count("help")) {
        printHelp();
        std::cout << desc << std::endl;
        return 0;
    }

    if (vm.count("list-cluster-algorithms")) {
        printPluginNames("cluster", kernel.getTestSuiteClusterPluginManager().getPluginNames());
        return 0;
    }

    if (vm.count("create-json-file")) {
        createJsonFile();
        return 0;
    }

    return loadJsonFiles(String(argv[1]));
}

void createJsonFile()
{
    std::ofstream of("sample.json");
    of << getJsonString();
    of.close();
}

void printHelp()
{
    std::cout << "This application measures the given test suites with the specified options in a json config files."
              << std::endl << std::endl;
    std::cout << "USAGE:" << std::endl
              << "\ttest-suite-metrics [--help|--list-cluster-algorithms|--create-json-file]" << std::endl
              << "\ttest-suite-metrics json file path" << std::endl
              << "\ttest-suite-metrics directory which contains one or more json files" << std::endl << std::endl;
    std::cout << "Json configuration file format:" << std::endl
              << getJsonString();
}

std::string getJsonString()
{
    std::stringstream ss;
    ss << "{\n\t\"coverage-data\": \"coverage file path\",\n\t"
       << "\"cluster-algorithm\": \"the name of the cluster algorithm to run before calculating the metrics\",\n\t"
       << "\"output-dir\": \"output directory\"\n"
       << "}"
       << std::endl;

    return ss.str();
}

void printPluginNames(const String &type, const std::vector<String> &plugins)
{
    std::cout << "The available algorithm modes for algorithm type: " << type << std::endl;
    for (size_t i = 0; i < plugins.size(); i++) {
        std::cout << "  * " << plugins[i] << std::endl;
    }
}

int loadJsonFiles(String path)
{
    if (!exists(path)) {
        std::cerr << "[ERROR] Invalid configuration file path." << std::endl;
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

void processJsonFiles(String path)
{
    try {
        std::cout << "[INFO] Processing " << path << " configuration file." << std::endl;

        rapidjson::Document reader;
        {
            FILE *in = fopen (path.c_str(), "r");
            char readBuffer[65536];
            rapidjson::FileReadStream is(in, readBuffer, sizeof(readBuffer));
            reader.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);
            fclose(in);
        }


        CSelectionData *selectionData = new CSelectionData();
        boost::filesystem::path jsonPath(path);
        std::string clusterAlgorithmName = reader["cluster-algorithm"].GetString();
        ITestSuiteClusterPlugin *clusterAlgorithm = kernel.getTestSuiteClusterPluginManager().getPlugin(clusterAlgorithmName);
        clusterAlgorithm->init(reader);

        outputDir = reader["output-dir"].GetString();
        if (outputDir.empty()) {
            std::cerr << "[ERROR] Missing output-dir parameter in config file " << path << "." << std::endl;
            return;
        }
        if (outputDir[0] == '.') {
            outputDir = jsonPath.parent_path().string() + "/" + outputDir;
        }
        if (!exists(outputDir)) {
            boost::filesystem::create_directory(boost::filesystem::path(outputDir));
        }

        String covPath = reader["coverage-data"].GetString();
        if (covPath[0] == '.') {
            covPath = jsonPath.parent_path().string() + "/" + covPath;
        }

        if (exists(covPath)) {
            (std::cerr << "[INFO] loading coverage from " << covPath << " ...").flush();
            selectionData->loadCoverage(covPath);
            (std::cerr << " done." << std::endl).flush();
        } else {
            std::cerr << "[ERROR] Missing or invalid input files in config file " << path << "." << std::endl;
            return;
        }

        clusterList.clear();

        (std::cerr << "[INFO] Running cluster algorithm: " << clusterAlgorithm->getName() << " ...").flush();
        clusterAlgorithm->execute(*selectionData, clusterList);
        (std::cerr << " done" << std::endl).flush();

        std::map<std::string, CClusterDefinition>::iterator it;

        const IBitMatrix& m = selectionData->getCoverage()->getBitMatrix();
        for (it = clusterList.begin(); it != clusterList.end(); it++) {

            std::cerr << "[INFO] Dumping: " << it->first << std::endl;

            std::ofstream O;
            O.open(std::string(outputDir + "/" + it->first + ".csv").c_str());

            IndexType nrOfCodeElements = it->second.getCodeElements().size();
            IndexType nrOfTestcases = it->second.getTestCases().size();

            for (IndexType i = 0; i < nrOfTestcases; i++) {
                IndexType tcid = it->second.getTestCases().at(i);
                O << m[tcid][0];

                for (IndexType j = 1; j < nrOfCodeElements; j++) {
                    IndexType cid = it->second.getCodeElements().at(j);
                    O << ";" << (m[tcid][cid] ? '1' : '0');
                }
                O << std::endl;
            }
            O.close();
        }


        delete selectionData;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return;
    } catch (...) {
        std::cerr << "Exception of unknown type while processsing configuration file(" << path << ") arguments."
                  << std::endl;
        return;
    }
    return;
}

