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
#include <rapidjson/prettywriter.h>
#include <rapidjson/filewritestream.h>

#include "data/CClusterDefinition.h"
#include "data/CSelectionData.h"
#include "engine/CKernel.h"
#include "util/CTestSuiteScore.h"


using namespace soda;
using namespace soda::io;
using namespace boost::filesystem;
using namespace boost::program_options;

void processJsonFiles(String path);
int  loadJsonFiles(String path);
void printPluginNames(const String &type, const std::vector<String> &plugins);
void printHelp();
String getJsonString();
void createJsonFile();

CKernel kernel;

IndexType revision;
String projectName;
String outputDir;
ClusterMap clusterList;
std::set<String> flTechniquesCalculated;

// Code element -> technique -> value
typedef std::map<IndexType, std::map<String, double> > FLScoreValues;

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

String getJsonString()
{
    String json = R"template({
    "coverage-data": "coverage file path",
    "results-data": "results file path",
    "bug-data": "bugset path",
    "cluster-algorithm": "one-cluster",
    "fault-localization-techniques": [ )template";
    bool first = true;
    for (auto &plugin : kernel.getFaultLocalizationTechniquePluginManager().getPluginNames()) {
        if (!first) {
            json += ", ";
        }
        json += "\"" + plugin + "\"";
        if (first) {
            first = false;
        }
    }
    json += R"template(],
    "globalize": false,
    "filter-to-coverage": true,
    "project-name": "prefix of the output files",
    "output-dir": "output directory",
    "revision": 51,
    "revision-timestamp": 1348408576,
    "failed-code-elements": [],
    "total-failed-testcases": 10
}
)template";

    return json;
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

void calculateFlTechnique(CSelectionData &selectionData, const String &name, rapidjson::Document &results)
{
    IFaultLocalizationTechniquePlugin *flTechnique = kernel.getFaultLocalizationTechniquePluginManager().getPlugin(name);

    StringVector dependencies = flTechnique->getDependency();
    for (StringVector::iterator it = dependencies.begin(); it != dependencies.end(); it++) {
        if (flTechniquesCalculated.find(*it) == flTechniquesCalculated.end()) {
            calculateFlTechnique(selectionData, *it, results);
        }
    }

    (std::cerr << "[INFO] Calculating fault localization values: " << flTechnique->getName() << " ...").flush();
    flTechnique->init(&selectionData, &clusterList, revision);
    flTechnique->calculate(results);
    flTechniquesCalculated.insert(name);
    (std::cerr << " done." << std::endl).flush();
}

void saveResults(CSelectionData &selectionData, rapidjson::Document & results, StringVector &buggedCEs, bool full) {
    std::stringstream sbflStream;

    // header of the output file
    // TODO: Changeable header for statements
    sbflStream << "Function;";
    for (rapidjson::Value::MemberIterator metricIt = results.MemberBegin()->value.MemberBegin()->value.MemberBegin(); metricIt != results.MemberBegin()->value.MemberBegin()->value.MemberEnd(); ++metricIt) {
        sbflStream << metricIt->name.GetString() << ";";
    }
    // append bugged header if exists
    sbflStream << "Bugged;";
    sbflStream << std::endl;

    if (full) {
        for (rapidjson::Value::MemberIterator ceIt = results["full"].MemberBegin(); ceIt != results["full"].MemberEnd(); ceIt++) {
            auto ceName = selectionData.getCoverage()->getCodeElements().getValue(std::stoul(ceIt->name.GetString()));
            sbflStream << ceName << ";";
            for (rapidjson::Value::MemberIterator valueIt = ceIt->value.MemberBegin(); valueIt != ceIt->value.MemberEnd(); ++valueIt) {

                if (valueIt->value.IsUint64()) {
                    sbflStream << valueIt->value.GetUint64();
                }
                else if (valueIt->value.IsDouble()) {
                    sbflStream << valueIt->value.GetDouble();
                }
                sbflStream << ";";

            }

            sbflStream << (std::find(buggedCEs.begin(), buggedCEs.end(), ceName) != buggedCEs.end() ? "1" : "0");
            sbflStream << std::endl;
        }
    }
    else {
        // TODO: implement output generation for multiple clusters
    }

    {
        std::ofstream out(String(outputDir + "/" + projectName + "-sbfl.csv").c_str());
        out << sbflStream.str();
        out.close();
    }
}

void processJsonFiles(String path)
{
    try {
        std::cout << "[INFO] Processing " << path << " configuration file." << std::endl;

        flTechniquesCalculated.clear();
        clusterList.clear();

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

        String clusterAlgorithmName = reader["cluster-algorithm"].GetString();
        ITestSuiteClusterPlugin *clusterAlgorithm = kernel.getTestSuiteClusterPluginManager().getPlugin(clusterAlgorithmName);
        clusterAlgorithm->init(reader);

        projectName = reader["project-name"].GetString();
        if (projectName.empty()) {
            std::cerr << "[ERROR] Missing project-name parameter in config file " << path << "." << std::endl;
            return;
        }
        outputDir = reader["output-dir"].GetString();
        if (outputDir.empty()) {
            std::cerr << "[ERROR] Missing output-dir parameter in config file " << path << "." << std::endl;
            return;
        }
        if (outputDir[0] == '.')
            outputDir = jsonPath.parent_path().string() + "/" + outputDir;
        if (!exists(outputDir))
            boost::filesystem::create_directories(boost::filesystem::path(outputDir));

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
                } catch (std::out_of_range &) {
                    std::cerr << "[ERROR] Invalid fault localization technique name(" << *it
                              << ") in configuration file: " << path << "." << std::endl;
                    return;
                }
            }
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

        String bugPath = reader["bug-data"].GetString();
        if (exists(bugPath)) {
            (std::cerr << "[INFO] loading bugs from " << bugPath << " ...").flush();
            selectionData.loadBugs(bugPath);
            (std::cerr << " done\n").flush();
        }
        else if (!bugPath.empty()) {
            std::cerr << "[WARNING] Not existing bug data path in config file " << path << "." << std::endl;
        }

        if (reader["globalize"].GetBool()) {
            (std::cerr << "[INFO] Globalizing ...").flush();
            selectionData.globalize();
            (std::cerr << " done" << std::endl).flush();
        }

        if (reader["filter-to-coverage"].GetBool()) {
            (std::cerr << "[INFO] Filtering to coverage ...").flush();
            selectionData.filterToCoverage();
            (std::cerr << " done" << std::endl).flush();
        }

        (std::cerr << "[INFO] Running cluster algorithm: " << clusterAlgorithm->getName() << " ...").flush();
        clusterAlgorithm->execute(selectionData, clusterList);
        (std::cerr << " done." << std::endl).flush();

        std::map<String, FLScoreValues> scoresByCluster;
        std::vector<IndexType> failedCodeElements;

        revision = reader["revision"].GetInt();
        time_t revTime = reader["revision-timestamp"].GetUint64();
        IndexType totalFailedTestcases = reader["total-failed-testcases"].GetInt();

        std::cout << "[INFO] Calculating scores for revision: " << revision << std::endl;
        for (rapidjson::Value::ConstValueIterator failIt = reader["failed-code-elements"].Begin(); failIt != reader["failed-code-elements"].End(); ++failIt) {
            if (!selectionData.getCoverage()->getCodeElements().containsValue(failIt->GetString())) {
                std::cerr << "[ERROR] Not existing code element name in json file: " << failIt->GetString() << std::endl;
                continue;
            }
            IndexType cid = selectionData.getCoverage()->getCodeElements().getID(failIt->GetString());
            failedCodeElements.push_back(cid);
        }

        // FD score
        // FIXME: Refactor
        /*for (clusterIt = clusterList.begin(); clusterIt != clusterList.end(); clusterIt++) {
            // Prepare directory for the output.
            Stringstream ss;
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
        }*/

        rapidjson::Document results;
        results.SetObject();
        for (auto flTechnique : faultLocalizationTechniques) {
            calculateFlTechnique(selectionData, flTechnique, results);

            /*for (clusterIt = clusterList.begin(); clusterIt != clusterList.end(); clusterIt++) {
                // Calculate FL score
                Stringstream ss;
                ss << outputDir << "/" << revision << "/" << clusterIt->first;

                IFaultLocalizationTechniquePlugin::FLValues *values = &technique->getValues();
                for (IndexType k = 0; k < failedCodeElements.size(); k++) {
                    IndexType cid = failedCodeElements[k];
                    double value = (*values)[static_cast<std::ostringstream*>( &(std::ostringstream() << cid) )->str().c_str()].GetDouble();
                    double score = CTestSuiteScore::flScore(clusterIt->second, value, technique->getDistribution());
                    scoresByCluster[clusterIt->first][cid][flTechniqueName] = score;
                }

                std::ofstream flScoreStream;
                flScoreStream.open((ss.str() + "/" + flTechniqueName + ".csv").c_str());
                flScoreStream << "#revision; code element;" << flTechniqueName << std::endl;
                for (IFaultLocalizationTechniquePlugin::FLValues::ConstMemberIterator it = values->MemberBegin(); it != values->MemberEnd(); ++it)
                    flScoreStream << revision << ";" << it->name.GetString() << ";" << it->value.GetDouble() << std::endl;
                flScoreStream.close();
            }*/


        }

        auto buggedCEs = selectionData.getBugs()->getBuggedCodeElements(revTime);
        saveResults(selectionData, results, buggedCEs, clusterAlgorithmName == "one-cluster");

        // Save the score values
        // FIXME: Refactor
        /*std::map<String, FLScoreValues>::iterator scoreIt;
        for (scoreIt = scoresByCluster.begin(); scoreIt != scoresByCluster.end(); scoreIt++) {
            Stringstream ss;
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
                    String flTechniqueName = faultLocalizationTechniques[k];
                    flScoreStream << scoresByCluster[scoreIt->first][cid][flTechniqueName] << ";";
                }
                flScoreStream << std::endl;
            }
            flScoreStream.close();
        }*/
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
