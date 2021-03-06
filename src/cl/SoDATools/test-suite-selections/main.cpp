/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Havas <havasd@inf.u-szeged.hu>
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

#include "CRevisionFilters.h"
#include "CComputeSelectionMetrics.h"
#include "data/CSelectionData.h"
#include "engine/CKernel.h"
#include "io/CJsonReader.h"

using namespace std;
using namespace soda;
using namespace soda::io;
using namespace boost::filesystem;
using namespace boost::program_options;

void processJsonFiles(String path);
int loadJsonFiles(String path);
void printPluginNames(const String &type, const std::vector<String> &plugins);
void printHelp();
string getJsonString();
void createJsonFile();

CKernel kernel;

int main(int argc, char* argv[]) {
    cout << "test-suite-selections (SoDA tool)" << endl;

    options_description desc("Options");
    desc.add_options()
            ("help,h", "Prints help message")
            ("create-json-file,j", "Creates a sample json file")
            ("list-algorithms,l", "Lists the prioritization algorithms");

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

    if (vm.count("list-algorithms")) {
        printPluginNames("prioritization", kernel.getTestSuitePrioritizationPluginManager().getPluginNames());
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
    cout << "This application measures the given selection data with the specified options in json config files."
         << endl << endl;
    cout << "USAGE:" << endl
         << "\ttest-suite-selections [-h|l|j]" << endl
         << "\ttest-suite-selections json file path" << endl
         << "\ttest-suite-selections directory which contains one or more json files" << endl << endl;
    cout << "Json configuration file format:" << endl
         << getJsonString();
}

string getJsonString()
{
    stringstream ss;
    ss << "{\n\t\"coverage-data\": \"coverage file path\",\n\t"
       << "\"results-data\": \"results file path\",\n"
       << "\t\"changeset\": \"changeset file path\",\n\t"
       << "\"selection-sizes\": [1, 10, 100],\n\t"
       << "\"prioritization-algorithms\": [ \"list of prioritization algorithms\" ],\n\t"
       << "\"output-file\": \"output file name\",\n\t"
       << "\"filter\": {\n"
       << "\t\t\"revision\": {\n"
       << "\t\t\t\"non-changed\": false,\n"
       << "\t\t\t\"non-failed\": false\n"
       << "\t\t\t},\n"
       << "\t\t\"revision-range\": [ ]\n"
       << "\t\t},\n\t"
       << "\"globalize\": false,\n\t"
       << "\"print-details\": false,\n"
       << "\t\"progress-level\": 0\n}" << endl;
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

void processJsonFiles(String path)
{
    try {
        std::cout << "[INFO] Processing " << path << " configuration file." << endl;
        CSelectionData selectionData;
        CJsonReader reader = CJsonReader(path);
        boost::filesystem::path jsonPath(path);

        String output = reader.getStringFromProperty("output-file");
        if (output.empty()) {
            std::cerr << "[ERROR] Missing output-file parameter in json file.("
                      << path << ")." << std::endl;
            return;
        }

        IntVector sizelist = reader.getIntVectorFromProperty("selection-sizes");
        if (sizelist.empty()) {
            std::cerr << "[ERROR] Missing selection-sizes parameter in json file("
                      << path << ")." << std::endl;
            return;
        }

        StringVector priolist = reader.getStringVectorFromProperty("prioritization-algorithms");
        if (priolist.empty()) {
            std::cerr << "[ERROR] prioritization-algorithm is missing from the configuration file("
                      << path << ")." << std::endl;
            printPluginNames("prioritization", kernel.getTestSuitePrioritizationPluginManager().getPluginNames());
            return;
        } else {
            for (StringVector::const_iterator it = priolist.begin(); it != priolist.end(); ++it) {
                try {
                    kernel.getTestSuitePrioritizationPluginManager().getPlugin(*it);
                } catch (std::out_of_range &) {
                    std::cerr << "[ERROR] Invalid prioritization algorithm name(" << *it
                              << ") in configuration file: " << path << "." << std::endl;
                    return;
                }
            }
        }

        if (output[0] == '.')
            output = jsonPath.parent_path().string() + "/" + output;

        String covPath = reader.getStringFromProperty("coverage-data");
        if (covPath[0] == '.') {
            covPath = jsonPath.parent_path().string() + "/" + covPath;
        }

        String resPath = reader.getStringFromProperty("results-data");
        if (resPath[0] == '.') {
            resPath = jsonPath.parent_path().string() + "/" + resPath;
        }

        if (exists(covPath) &&
                exists(resPath)) {
            (cerr << "[INFO] loading coverage from " << covPath << " ...").flush();
            selectionData.loadCoverage(covPath);
            (cerr << " done\n[INFO] loading results from " << resPath << " ...").flush();
            selectionData.loadResults(resPath);
            (cerr << " done" << endl).flush();
        } else {
            std::cerr << "[ERROR] Missing input files in config file " << path << "." << std::endl;
            return;
        }

        String changPath = reader.getStringFromProperty("changeset");
        if (changPath[0] == '.') {
            changPath = jsonPath.parent_path().string() + "/" + changPath;
        }

        if (reader.getBoolFromProperty("filter.revision.non-changed")
                && exists(changPath)) {
            (cerr << "[INFO] loading changes from " << changPath << " ...").flush();
            selectionData.loadChangeset(changPath);
            (cerr << " done." << endl).flush();
        } else if (reader.getBoolFromProperty("filter.revision.non-changed")) {
            std::cerr << "[ERROR] Missing changeset files in config file " << path << "." << std::endl;
            return;
        }

        IntVector revisionlist = selectionData.getResults()->getRevisionNumbers();

        revisionlist = CRevisionFilters().filterNonChangedOrNonFailed(revisionlist, &selectionData, !reader.getBoolFromProperty("filter.revision.non-changed"), !reader.getBoolFromProperty("filter.revision.non-failed"));
        if (reader.existsProperty("filter.revision-range")) {
            IntVector tmp = reader.getIntVectorFromProperty("filter.revision-range");
            if (tmp.size() == 2) {
                (cerr << "[INFO] Filtering revisions to [" << tmp[0] << "," << tmp[1] << ")" << endl).flush();
                revisionlist = CRevisionFilters().filterRange(revisionlist, tmp[0], tmp[1]);
            } else {
                (cerr << "[WARN] Invalid revision range" << endl).flush();
            }
        }

        if (reader.getBoolFromProperty("globalize")) {
            (cerr << "[INFO] Globalizing...").flush();
            selectionData.globalize();
            (cerr << " done" << endl).flush();
        }

        while(!priolist.empty()) {
            string t = priolist.back();
            priolist.pop_back();
            std::ofstream of((output + "-" + t + ".csv").c_str());

            ITestSuitePrioritizationPlugin *plugin = NULL;
            try {
                plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin(t);
                plugin->init(&selectionData, &kernel);
            } catch (std::out_of_range &) {
                std::cerr << "[ERROR] Unknown algorithm mode. " << std::endl;
                printPluginNames("prioritization", kernel.getTestSuitePrioritizationPluginManager().getPluginNames());
                return;
            }

            CComputeSelectionMetrics *selectionstat = new CComputeSelectionMetrics(&selectionData, plugin, &revisionlist, &sizelist, reader.getIntFromProperty("progress-level"));
            (cerr << "[INFO] Measurements on " << revisionlist.size() << " revisions ...").flush();
            selectionstat->runMeasurement();
            (cerr << " done." << endl).flush();

            if (reader.getBoolFromProperty("print-details")) {
                if (!output.empty())
                    of << selectionstat->getDetailedData();
                else {
                    std::cout.precision(5);
                    std::cout << selectionstat->getDetailedData();
                }
            } else {
                if (!output.empty())
                    of << selectionstat->getData();
                else {
                    std::cout.precision(5);
                    std::cout << selectionstat->getData();
                }
            }
            of.close();
            delete selectionstat;
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
