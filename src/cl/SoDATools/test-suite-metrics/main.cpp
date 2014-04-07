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
#include <set>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include "data/CClusterDefinition.h"
#include "data/CSelectionData.h"
#include "engine/CKernel.h"
#include "io/CJsonReader.h"


using namespace soda;
using namespace soda::io;
using namespace boost::filesystem;
using namespace boost::program_options;

void processJsonFiles(String path);
int loadJsonFiles(String path);
void printPluginNames(const String &type, const std::vector<String> &plugins);
void printHelp();

CKernel kernel;


IndexType revision;
std::vector<CClusterDefinition> clusterList;
std::string outputDir;
std::vector<ITestSuiteMetricPlugin::MetricResults> results;

std::set<std::string> metricsCalculated;

int main(int argc, char* argv[]) {
    std::cout << "test-suite-metrics (SoDA tool)" << std::endl;

    options_description desc("Options");
    desc.add_options()
            ("help,h", "Prints help message")
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

    return loadJsonFiles(String(argv[1]));
}

void printHelp()
{
    std::cout << "This application measures the given test suites with the specified options in a json config files."
         << std::endl << std::endl;
    std::cout << "USAGE:" << std::endl
         << "\ttest-suite-metrics [-hcm]" << std::endl
         << "\ttest-suite-metrics json file path" << std::endl
         << "\ttest-suite-metrics directory which contains one or more json files" << std::endl << std::endl;
    std::cout << "Json configuration file format:" << std::endl
         << "{\n\t\"coverage-data\": \"coverage file path\",\n\t"
         << "\"results-data\": \"results file path\",\n\t"
         << "\"revision\": 1,\n\t"
         << "\"cluster-algorithm\": \"the name of the cluster algorithm to run before calculating the metrics\",\n\t"
         << "\"metrics\": [ \"list of metrics to calculate\" ],\n\t"
         << "\"globalize\": true,\n\t"
         << "\"output-dir\": \"output directory\"\n"
         << "}"
         << std::endl;
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

void calculateMetric(CSelectionData *selectionData, const std::string &name)
{
    ITestSuiteMetricPlugin *metric = kernel.getTestSuiteMetricPluginManager().getPlugin(name);

    StringVector dependencies = metric->getDependency();
    for (StringVector::iterator it = dependencies.begin(); it != dependencies.end(); it++) {
        if (metricsCalculated.find(*it) == metricsCalculated.end()) {
            calculateMetric(selectionData, *it);
        }
    }

    (std::cerr << "[INFO] Calculating metrics: " << metric->getName() << " ...").flush();
    metric->init(selectionData, &clusterList, revision);
    metric->calculate(outputDir, results);
    metricsCalculated.insert(name);
    (std::cerr << " done." << std::endl).flush();
}

void processJsonFiles(String path)
{
    try {
        std::cout << "[INFO] Processing " << path << " configuration file." << std::endl;

        CJsonReader reader = CJsonReader(path);

        std::string clusterAlgorithmName = reader.getStringFromProperty("cluster-algorithm");
        ITestSuiteClusterPlugin *clusterAlgorithm = kernel.getTestSuiteClusterPluginManager().getPlugin(clusterAlgorithmName);

        CSelectionData *selectionData = new CSelectionData();

        if (exists(reader.getStringFromProperty("coverage-data")) &&
                exists(reader.getStringFromProperty("results-data"))) {
            (std::cerr << "[INFO] loading coverage from " << reader.getStringFromProperty("coverage-data") << " ...").flush();
            selectionData->loadCoverage(reader.getStringFromProperty("coverage-data"));
            (std::cerr << " done\n[INFO] loading results from " << reader.getStringFromProperty("results-data") << " ...").flush();
            selectionData->loadResults(reader.getStringFromProperty("results-data"));
            (std::cerr << " done" << std::endl).flush();
        } else {
            std::cerr << "[ERROR] Missing or invalid input files in config file " << path << "." << std::endl;
            return;
        }

        if (reader.getBoolFromProperty("globalize")) {
            (std::cerr << "[INFO] Globalizing ...").flush();
            selectionData->globalize();
            (std::cerr << " done" << std::endl).flush();
        }

        revision = reader.getIntFromProperty("revision");
        outputDir = reader.getStringFromProperty("output-dir");

        clusterList.clear();
        metricsCalculated.clear();
        results.clear();

        (std::cerr << "[INFO] Running cluster algorithm: " << clusterAlgorithm->getName() << " ...").flush();
        clusterAlgorithm->execute(*selectionData, clusterList);
        (std::cerr << " done" << std::endl).flush();

        results.resize(clusterList.size());

        StringVector metrics = reader.getStringVectorFromProperty("metrics");

        for (StringVector::iterator it = metrics.begin(); it != metrics.end(); it++) {
            calculateMetric(selectionData, *it);
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
