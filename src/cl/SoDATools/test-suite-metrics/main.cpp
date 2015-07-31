/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Tengeri <dtengeri@inf.u-szeged.hu>
 *          David Havas <havasd@inf.u-szeged.hu>
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
#include <stdio.h>
#include <set>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "data/CClusterDefinition.h"
#include "data/CSelectionData.h"
#include "engine/CKernel.h"

#include <cstdio>
#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/prettywriter.h"


using namespace soda;
using namespace soda::io;
using namespace boost::filesystem;
using namespace boost::program_options;

void processJsonFiles(String path);
int loadJsonFiles(String path);
void printPluginNames(const String &type, const std::vector<String> &plugins);
void printHelp();
std::string getJsonString();
void createJsonFile();

CKernel kernel;
IndexType revision;
std::map<std::string, CClusterDefinition> clusterList;
std::string outputDir;
std::string projectName;

std::set<std::string> baseMetrics;
std::set<std::string> metricsCalculated;

int main(int argc, char* argv[]) {
    std::cout << "test-suite-metrics (SoDA tool)" << std::endl;

    options_description desc("Options");
    desc.add_options()
            ("help,h", "Prints help message")
            ("create-json-file,j", "Creates a sample json file")
            ("list-cluster-algorithms,c", "Lists the cluster algorithms")
            ("list-metric-plugins,m", "Lists the metric plugins");

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
    if (vm.count("list-metric-plugins")) {
        printPluginNames("metric", kernel.getTestSuiteMetricPluginManager().getPluginNames());
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
              << "\ttest-suite-metrics [-h|c|m|j]" << std::endl
              << "\ttest-suite-metrics json file path" << std::endl
              << "\ttest-suite-metrics directory which contains one or more json files" << std::endl << std::endl;
    std::cout << "Json configuration file format:" << std::endl
              << getJsonString();
}

std::string getJsonString()
{
    std::stringstream ss;
    ss << "{\n\t\"coverage-data\": \"coverage file path\",\n\t"
       << "\"results-data\": \"results file path\",\n\t"
       << "\"revision\": 1,\n\t"
       << "\"cluster-algorithm\": \"the name of the cluster algorithm to run before calculating the metrics\",\n\t"
       << "\"metrics\": [ \"list of metrics to calculate\" ],\n\t"
       << "\"base-metrics\": [ \"coverage\", \"partition-metric\", \"tpce\" ],\n\t"
       << "\"project-name\": \"name of the project, this string will be the part of the output file names\",\n\t"
       << "\"globalize\": false,\n\t"
       << "\"filter-to-coverage\": false,\n\t"
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

void calculateMetric(CSelectionData *selectionData, const std::string &name, rapidjson::Document &results)
{
    ITestSuiteMetricPlugin *metric = kernel.getTestSuiteMetricPluginManager().getPlugin(name);

    StringVector dependencies = metric->getDependency();
    for (StringVector::iterator it = dependencies.begin(); it != dependencies.end(); it++) {
        if (metricsCalculated.find(*it) == metricsCalculated.end()) {
            calculateMetric(selectionData, *it, results);
        }
    }

    (std::cerr << "[INFO] Calculating metrics: " << metric->getName() << " ...").flush();
    metric->init(selectionData, &clusterList, revision);
    metric->calculate(results);
    metricsCalculated.insert(name);
    (std::cerr << " done." << std::endl).flush();
}

void saveResults(rapidjson::Document &results)
{
    std::stringstream base;
    std::stringstream ext;
    bool hasBase = false, hasExt = false;
    base << ";";
    ext << ";";
    for (std::set<std::string>::iterator it = metricsCalculated.begin(); it != metricsCalculated.end(); it++) {
        if (baseMetrics.count(*it)) {
            hasBase = true;
            base << *it << ";";
        }
        else {
            hasExt = true;
            ext << *it << ";";
        }
    }
    base << std::endl;
    ext << std::endl;

    std::map<std::string, CClusterDefinition>::iterator clusterIt;
    for (clusterIt = clusterList.begin(); clusterIt != clusterList.end(); clusterIt++) {
        std::string clusterName = clusterIt->first;
        base << clusterName << ";";
        ext << clusterName << ";";
        for (std::set<std::string>::iterator it = metricsCalculated.begin(); it != metricsCalculated.end(); it++) {
            if (baseMetrics.count(*it)) {
                base << results[clusterName.c_str()][(*it).c_str()].GetDouble() << ";";
            }
            else {
                ext << results[clusterName.c_str()][(*it).c_str()].GetDouble() << ";";
            }
        }
        base << std::endl;
        ext << std::endl;
    }

    if (hasBase) {
        std::ofstream out(std::string(outputDir + "/" + projectName + "-metrics-base.csv").c_str());
        out << base.str();
        out.close();
    }

    if (hasExt) {
        std::ofstream out(std::string(outputDir + "/" + projectName + "-metrics-ext.csv").c_str());
        out << ext.str();
        out.close();
    }
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

        boost::filesystem::path jsonPath(path);
        std::string clusterAlgorithmName = reader["cluster-algorithm"].GetString();
        ITestSuiteClusterPlugin *clusterAlgorithm = kernel.getTestSuiteClusterPluginManager().getPlugin(clusterAlgorithmName);
        clusterAlgorithm->init(reader);

        CSelectionData *selectionData = new CSelectionData();

        for (rapidjson::Value::ConstValueIterator itr = reader["base-metrics"].Begin(); itr != reader["base-metrics"].End(); ++itr) {
            baseMetrics.insert(itr->GetString());
        }

        StringVector metrics;
        for (rapidjson::Value::ConstValueIterator itr = reader["metrics"].Begin(); itr != reader["metrics"].End(); ++itr) {
            metrics.push_back(itr->GetString());
        }

        if (metrics.empty()) {
            std::cerr << "[ERROR] Missing metrics parameter in config file " << path << "." << std::endl;
            return;
        } else {
            for (StringVector::const_iterator it = metrics.begin(); it != metrics.end(); ++it) {
                try {
                    kernel.getTestSuiteMetricPluginManager().getPlugin(*it);
                } catch (std::out_of_range &) {
                    std::cerr << "[ERROR] Invalid metric name(" << *it
                              << ") in configuration file: " << path << "." << std::endl;
                    return;
                }
            }
        }

        revision = reader["revision"].GetInt();
        projectName = reader["project-name"].GetString();
        outputDir = reader["output-dir"].GetString();
        if (outputDir.empty()) {
            std::cerr << "[ERROR] Missing output-dir parameter in config file " << path << "." << std::endl;
            return;
        }
        if (outputDir[0] == '.')
            outputDir = jsonPath.parent_path().string() + "/" + outputDir;
        if (!exists(outputDir))
            boost::filesystem::create_directory(boost::filesystem::path(outputDir));

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
            selectionData->loadCoverage(covPath);
            (std::cerr << " done\n[INFO] loading results from " << resPath << " ...").flush();
            selectionData->loadResults(resPath);
            (std::cerr << " done" << std::endl).flush();
        } else {
            std::cerr << "[ERROR] Missing or invalid input files in config file " << path << "." << std::endl;
            return;
        }

        if (reader["globalize"].GetBool()) {
            (std::cerr << "[INFO] Globalizing ...").flush();
            selectionData->globalize();
            (std::cerr << " done" << std::endl).flush();
        }

        if (reader["filter-to-coverage"].GetBool()) {
            (std::cerr << "[INFO] Filtering to coverage ...").flush();
            selectionData->filterToCoverage();
            (std::cerr << " done" << std::endl).flush();
        }

        clusterList.clear();
        metricsCalculated.clear();

        (std::cerr << "[INFO] Running cluster algorithm: " << clusterAlgorithm->getName() << " ...").flush();
        clusterAlgorithm->execute(*selectionData, clusterList);
        (std::cerr << " done" << std::endl).flush();

        rapidjson::Document results;
        results.SetObject();
        for (StringVector::iterator it = metrics.begin(); it != metrics.end(); it++) {
            if (metricsCalculated.find(*it) == metricsCalculated.end()) {
                calculateMetric(selectionData, *it, results);
            }
        }

        saveResults(results);

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
