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
#include <stdio.h>
#include <set>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "data/CClusterDefinition.h"
#include "data/CSelectionData.h"
#include "engine/CKernel.h"
#include "io/CJsonReader.h"

#include <cstdio>
#include "rapidjson/filestream.h"
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
std::map<std::string, ITestSuiteMetricPlugin::MetricResults> results;

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
       << "\"globalize\": true,\n\t"
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

void processJsonFiles(String path)
{
    try {
        std::cout << "[INFO] Processing " << path << " configuration file." << std::endl;

        FILE *in = fopen (path.c_str(), "r");
        rapidjson::FileStream is(in); // C FILE objectum
        rapidjson::Document reader; // json amibe beolvassuk
        reader.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileStream>(is); // beolvassa a jsont meg kell adni, hogy a flageket kódolást, streamet
        fclose(in);

        std::string clusterAlgorithmName = reader["cluster-algorithm"].GetString();
        ITestSuiteClusterPlugin *clusterAlgorithm = kernel.getTestSuiteClusterPluginManager().getPlugin(clusterAlgorithmName);
        clusterAlgorithm->init(reader);

        CSelectionData *selectionData = new CSelectionData();

        StringVector metrics;
        for (rapidjson::Value::ConstValueIterator itr = reader["metrics"].Begin(); itr != reader["metrics"].End(); ++itr)
            metrics.push_back(itr->GetString());

        if (metrics.empty()) {
            std::cerr << "[ERROR] Missing metrics parameter in config file " << path << "." << std::endl;
            return;
        } else {
            for (StringVector::const_iterator it = metrics.begin(); it != metrics.end(); ++it) {
                try {
                    kernel.getTestSuiteMetricPluginManager().getPlugin(*it);
                } catch (std::out_of_range &e) {
                    std::cerr << "[ERROR] Invalid metric name(" << *it
                              << ") in configuration file: " << path << "." << std::endl;
                    return;
                }
            }
        }

        revision = reader["revision"].GetInt();
        outputDir = reader["output-dir"].GetString();
        if (outputDir.empty()) {
            std::cerr << "[ERROR] Missing output-dir parameter in config file " << path << "." << std::endl;
            return;
        }

        if (exists(reader["coverage-data"].GetString()) &&
                exists(reader["results-data"].GetString())) {
            (std::cerr << "[INFO] loading coverage from " << reader["coverage-data"].GetString() << " ...").flush();
            selectionData->loadCoverage(reader["coverage-data"].GetString());
            (std::cerr << " done\n[INFO] loading results from " << reader["results-data"].GetString() << " ...").flush();
            selectionData->loadResults(reader["results-data"].GetString());
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

        clusterList.clear();
        metricsCalculated.clear();
        results.clear();

        (std::cerr << "[INFO] Running cluster algorithm: " << clusterAlgorithm->getName() << " ...").flush();
        clusterAlgorithm->execute(*selectionData, clusterList);
        (std::cerr << " done" << std::endl).flush();

        rapidjson::Document results;
        results.SetObject();
        for (StringVector::iterator it = metrics.begin(); it != metrics.end(); it++) {
            calculateMetric(selectionData, *it, results);
        }

        // TODO process results.
        FILE *out = fopen(std::string(outputDir + "/test.suite.metrics.json").c_str(), "w");
        rapidjson::FileStream f(out); // filestream csak C szintű fájlkezelést támogat / stdout
        rapidjson::PrettyWriter<rapidjson::FileStream> writer(f); // prettywriter formázza a jsont
        results.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
        fclose(out);

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
