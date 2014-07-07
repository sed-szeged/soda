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
#include <sstream>
#include <set>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>


#include "data/CClusterDefinition.h"
#include "data/CSelectionData.h"
#include "engine/CKernel.h"
#include "util/CTestSuiteScore.h"


using namespace soda;
using namespace soda::io;
using namespace boost::filesystem;
using namespace boost::program_options;

void processJsonFiles(std::string path);
int  loadJsonFiles(std::string path);
void printPluginNames(const String &type, const std::vector<String> &plugins);
void printHelp();
std::string getJsonString();
void createJsonFile();

CKernel kernel;

// Code element -> technique -> value
typedef std::map<IndexType, std::map<std::string, double> > FLScoreValues;

int main(int argc, char* argv[]) {
    std::cout << "test-suite-score (SoDA tool)" << std::endl;

    options_description desc("Options");
    desc.add_options()
            ("help,h", "Prints help message")
            ("create-json-file,j", "Creates a sample json file")
            ("list-cluster-algorithms,c", "Lists the cluster algorithms")
            ("list-fault-localization-techniques,f", "Lists the fault localization technique plugins");

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
    if (vm.count("list-fault-localization-techniques")) {
        printPluginNames("metric", kernel.getFaultLocalizationTechniquePluginManager().getPluginNames());
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
    std::cout << "This application calculates the score of given test suites in terms fault localization and detection with the specified options in a json config files."
              << std::endl << std::endl;
    std::cout << "USAGE:" << std::endl
              << "\ttest-suite-score [-h|c|f|j]" << std::endl
              << "\ttest-suite-score json file path" << std::endl
              << "\ttest-suite-score directory which contains one or more json files" << std::endl << std::endl;
    std::cout << "Json configuration file format:" << std::endl
              << getJsonString();
}

std::string getJsonString()
{
    std::stringstream ss;
    ss << "{\n\t\"coverage-data\": \"coverage file path\",\n\t"
       << "\"results-data\": \"results file path\",\n\t"
       << "\"cluster-algorithm\": \"the name of the cluster algorithm to run before calculating the scores\",\n\t"
       << "\"fault-localization-techniques\": [ \"list of fault localization techniques to use\" ],\n\t"
       << "\"globalize\": true,\n\t"
       << "\"output-dir\": \"output directory\",\n\t"
       << "\"selected-revisions\":\n\t"
       << "[ // multiple revisions with the required parameters\n\t\t{\n\t\t\t"
       << "\"revision\": 1,\n\t\t\t"
       << "\"failed-code-elements\": [ \"list of code elements that casue the failure of the test cases (by their string representation)\" ],\n\t\t\t"
       << "\"total-failed-testcases\": 125 //The total number of failing testcases in the whole test suite (without any reduction or selection and clusterization).\n\t\t"
       << "}\n\t]\n"
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

int loadJsonFiles(std::string path)
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

void processJsonFiles(std::string path)
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

        boost::filesystem::path jsonPath(path);
        CSelectionData selectionData;
        std::map<std::string, CClusterDefinition> clusterList;

        std::string clusterAlgorithmName = reader["cluster-algorithm"].GetString();
        ITestSuiteClusterPlugin *clusterAlgorithm = kernel.getTestSuiteClusterPluginManager().getPlugin(clusterAlgorithmName);
        clusterAlgorithm->init(reader);

        std::string outputDir = reader["output-dir"].GetString();
        if (outputDir.empty()) {
            std::cerr << "[ERROR] Missing output-dir parameter in config file " << path << "." << std::endl;
            return;
        }
        if (outputDir[0] == '.')
            outputDir = jsonPath.parent_path().string() + "/" + outputDir;

        StringVector faultLocalizationTechniques;
        for (rapidjson::Value::ConstValueIterator itr = reader["fault-localization-techniques"].Begin(); itr != reader["fault-localization-techniques"].End(); ++itr)
            faultLocalizationTechniques.push_back(itr->GetString());

        if (faultLocalizationTechniques.empty()) {
            std::cerr << "[ERROR] Missing fault-localization-techniques parameter in config file " << path << "." << std::endl;
            return;
        } else {
            for (StringVector::const_iterator it = faultLocalizationTechniques.begin(); it != faultLocalizationTechniques.end(); ++it) {
                try {
                    kernel.getFaultLocalizationTechniquePluginManager().getPlugin(*it);
                } catch (std::out_of_range &e) {
                    std::cerr << "[ERROR] Invalid fault localization technique name(" << *it
                              << ") in configuration file: " << path << "." << std::endl;
                    return;
                }
            }
        }

        if (!reader["selected-revisions"].Size()) {
            std::cerr << "[ERROR] Missing selected-revisions parameter in config file " << path << "." << std::endl;
            return;
        }

        String covPath = reader["coverage-data"].GetString();
        if (covPath[0] == '.') {
            covPath = jsonPath.parent_path().string() + "/" + covPath;
        }

        String resPath = reader["results-data"].GetString();
        if (resPath[0] == '.') {
            resPath = jsonPath.parent_path().string() + "/" + resPath;
        }

        if (exists(covPath) && exists(resPath)) {
            (std::cerr << "[INFO] loading coverage from " << covPath << " ...").flush();
            selectionData.loadCoverage(covPath);
            (std::cerr << " done\n[INFO] loading results from " << resPath << " ...").flush();
            selectionData.loadResults(resPath);
            (std::cerr << " done" << std::endl).flush();
        } else {
            std::cerr << "[ERROR] Missing or invalid input files in config file " << path << "." << std::endl;
            return;
        }

        if (reader["globalize"].GetBool()) {
            (std::cerr << "[INFO] Globalizing ...").flush();
            selectionData.globalize();
            (std::cerr << " done" << std::endl).flush();
        }

        (std::cerr << "[INFO] Running cluster algorithm: " << clusterAlgorithm->getName() << " ...").flush();
        clusterAlgorithm->execute(selectionData, clusterList);
        (std::cerr << " done." << std::endl).flush();

        for (rapidjson::Value::ConstValueIterator itr = reader["selected-revisions"].Begin(); itr != reader["selected-revisions"].End(); ++itr) {
            std::map<std::string, FLScoreValues> scoresByCluster;
            std::vector<IndexType> failedCodeElements;

            IndexType revision = (*itr)["revision"].GetInt();
            IndexType totalFailedTestcases = (*itr)["total-failed-testcases"].GetInt();

            std::cout << "[INFO] Calculating scores for revision: " << revision << std::endl;
            for (rapidjson::Value::ConstValueIterator failIt = (*itr)["failed-code-elements"].Begin(); failIt != (*itr)["failed-code-elements"].End(); ++failIt) {
                if (!selectionData.getCoverage()->getCodeElements().containsValue(failIt->GetString())) {
                    std::cerr << "[ERROR] Not existing code element name in json file: " << failIt->GetString() << std::endl;
                    continue;
                }
                IndexType cid = selectionData.getCoverage()->getCodeElements().getID(failIt->GetString());
                failedCodeElements.push_back(cid);
            }

            // FD score
            std::map<std::string, CClusterDefinition>::iterator clusterIt;
            for (clusterIt = clusterList.begin(); clusterIt != clusterList.end(); clusterIt++) {
                // Prepare directory for the output.
                std::stringstream ss;
                ss << outputDir << "/" << revision << "/" << clusterIt->first;
                boost::filesystem::path dir(ss.str().c_str());
                boost::filesystem::create_directories(dir);

                ss << "/fd.score.csv";

                std::ofstream fdScoreStream;
                fdScoreStream.open(ss.str().c_str());
                fdScoreStream << "#fd score;" << std::endl;

                double fdScore = CTestSuiteScore::fdScore(selectionData, clusterIt->second, revision, totalFailedTestcases);
                fdScoreStream << fdScore << std::endl;

                fdScoreStream.close();
            }

            for (IndexType i = 0; i < faultLocalizationTechniques.size(); i++) {
                std::string flTechniqueName = faultLocalizationTechniques[i];
                IFaultLocalizationTechniquePlugin *technique = kernel.getFaultLocalizationTechniquePluginManager().getPlugin(flTechniqueName);

                technique->init(&selectionData, revision);
                for (clusterIt = clusterList.begin(); clusterIt != clusterList.end(); clusterIt++) {
                    // Calculate FL score
                    std::stringstream ss;
                    ss << outputDir << "/" << revision << "/" << clusterIt->first;
                    technique->calculate(clusterIt->second, ss.str());

                    IFaultLocalizationTechniquePlugin::FLValues values = technique->getValues();

                    for (IndexType k = 0; k < failedCodeElements.size(); k++) {
                        IndexType cid = failedCodeElements[k];
                        double score = CTestSuiteScore::flScore(clusterIt->second, values[cid], technique->getDistribution());
                        scoresByCluster[clusterIt->first][cid][flTechniqueName] = score;
                    }
                }
            }
            // Save the score values
            std::map<std::string, FLScoreValues>::iterator scoreIt;
            for (scoreIt = scoresByCluster.begin(); scoreIt != scoresByCluster.end(); scoreIt++) {
                std::stringstream ss;
                ss << outputDir << "/" << revision << "/" << scoreIt->first << "/fl.score.csv";

                std::ofstream flScoreStream;
                flScoreStream.open(ss.str().c_str());
                flScoreStream << "#cluster;code element id;";
                for (IndexType j = 0; j < faultLocalizationTechniques.size(); j++) {
                    flScoreStream << faultLocalizationTechniques[j] << ";";
                }
                flScoreStream << std::endl;

                for (IndexType j = 0; j < failedCodeElements.size(); j++) {
                    IndexType cid = failedCodeElements[j];
                    flScoreStream << scoreIt->first << ";" << cid << ";";
                    for (IndexType k = 0; k < faultLocalizationTechniques.size(); k++) {
                        std::string flTechniqueName = faultLocalizationTechniques[k];
                        flScoreStream << scoresByCluster[scoreIt->first][cid][flTechniqueName] << ";";
                    }
                    flScoreStream << std::endl;
                }
                flScoreStream.close();
            }
        }
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
