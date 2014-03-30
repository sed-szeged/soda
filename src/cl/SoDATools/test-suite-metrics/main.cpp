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
         << "\ttest-suite-metrics [-hl]" << std::endl
         << "\ttest-suite-metrics json file path" << std::endl
         << "\ttest-suite-metrics directory which contains one or more json files" << std::endl << std::endl;
    std::cout << "Json configuration file format:" << std::endl
         << "{\n\t\"coverage-data\": \"coverage file path\",\n\t"
         << "\"results-data\": \"results file path\",\n\t"
         << "\"revision-list\": [1, 10, 100],\n\t"
         << "\"cluster-algorithm\": \"the name of the cluster algorithm to run before calculating the metrics\",\n\t"
         << "\"metrics\": [ \"list of metrics to calculate\" ],\n\t"
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
        CSelectionData selectionData;
        CJsonReader reader = CJsonReader(path);

        std::string clusterAlgorithmName = reader.getStringFromProperty("cluster-algorithm");
        ITestSuiteClusterPlugin *clusterAlgorithm = kernel.getTestSuiteClusterPluginManager().getPlugin(clusterAlgorithmName);

        if (exists(reader.getStringFromProperty("coverage-data")) &&
                exists(reader.getStringFromProperty("results-data"))) {
            (std::cerr << "[INFO] loading coverage from " << reader.getStringFromProperty("coverage-data") << " ...").flush();
            selectionData.loadCoverage(reader.getStringFromProperty("coverage-data"));
            (std::cerr << " done\n[INFO] loading results from " << reader.getStringFromProperty("results-data") << " ...").flush();
            selectionData.loadResults(reader.getStringFromProperty("results-data"));
            (std::cerr << " done" << std::endl).flush();
        } else {
            std::cerr << "[ERROR] Missing or invalid input files in config file " << path << "." << std::endl;
            return;
        }

        IntVector revisionList = reader.getIntVectorFromProperty("revision-list");
        std::string outputDir = reader.getStringFromProperty("output-dir");

        (std::cerr << "[INFO] Running cluster algorithm: " << clusterAlgorithm->getName() << " ...").flush();
        std::vector<CClusterDefinition> clusterList;
        clusterAlgorithm->execute(selectionData, clusterList);
        (std::cerr << " done" << std::endl).flush();


        StringVector metrics = reader.getStringVectorFromProperty("metrics");
        for (StringVector::iterator it = metrics.begin(); it != metrics.end(); it++) {
            ITestSuiteMetricPlugin *metric = kernel.getTestSuiteMetricPluginManager().getPlugin(*it);
            (std::cerr << "[INFO] Calculating metrics: " << metric->getName() << " ...").flush();
            metric->calculate(selectionData, clusterList, revisionList, outputDir);
            (std::cerr << " done" << std::endl).flush();
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
