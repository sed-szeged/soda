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

#include "data/CSelectionData.h"
#include "exception/CException.h"
#include "engine/CKernel.h"
#include "io/CJsonReader.h"

using namespace std;
using namespace soda;
using namespace soda::io;
using namespace boost::filesystem;
using namespace boost::program_options;

void processJsonFiles(String path);
int loadJsonFiles(String path);
void printPluginNames(const std::vector<String> &plugins);
void printHelp();
std::string getJsonString();
void createJsonFile();

CKernel kernel;

int main(int argc, char* argv[]) {
    cout << "test-suite-reduction (SoDA tool)" << endl;

    options_description desc("Options");
    desc.add_options()
            ("help,h", "Prints help message")
            ("create-json-file,j", "Creates a sample json file")
            ("list-algorithms,l", "Lists the reduction algorithms");

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
        printPluginNames(kernel.getTestSuiteReductionPluginManager().getPluginNames());
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
    cout << "The application reduces the given coverage data with specified reduction algorithms."
         << endl << endl;
    cout << "USAGE:" << endl
         << "\ttest-suite-reduction [-h|l|j]" << endl
         << "\ttest-suite-reduction json file path" << endl
         << "\ttest-suite-reduction directory which contains one or more json files" << endl << endl;
    cout << "Json configuration file format:" << endl
         << getJsonString();
}

std::string getJsonString()
{
    std::stringstream ss;
    ss << "{\n\t\"coverage-data\": \"coverage file path\",\n\t"
       << "\"results-data\": \"results file path\",\n\t"
       << "\"iteration\": 15,\n\t"
       << "\"reduction-sizes\": [1000, 5000, 10000, 25000, 30000],\n\t"
       << "\"reduction-method\": [\"reduction methods\"],\n\t"
       << "\"program-name\": \"program name\",\n\t"
       << "\"output-dir\": \"output dir\",\n"
       << "\t\"globalize\": true\n}" << endl;

    return ss.str();
}

void printPluginNames(const std::vector<String> &plugins)
{
    std::cout << "The available reduction plugins: " << std::endl;
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

        boost::filesystem::path jsonPath(path);
        CSelectionData selectionData;
        CJsonReader reader = CJsonReader(path);

        String programName = reader.getStringFromProperty("program-name");
        if (programName.empty()) {
            std::cerr << "[ERROR] Program name is missing in configuration file:" << path << std::endl;
            return;
        }

        StringVector reductionList = reader.getStringVectorFromProperty("reduction-method");
        if (reductionList.empty()) {
            std::cerr << "[ERROR] reduction-algorithm is missing from the configuration file("
                      << path << ")." << std::endl;
            printPluginNames(kernel.getTestSuiteReductionPluginManager().getPluginNames());
            return;
        } else {
            int iteration = reader.getIntFromProperty("iteration");
            for (StringVector::const_iterator it = reductionList.begin(); it != reductionList.end(); ++it) {
                if (*it == "duplation" && !iteration) {
                    std::cerr << "[ERROR] Missing iteration parameter for duplation reduction method in configuration file: "
                              << path << "." << std::endl;
                    return;
                }

                if (*it == "random" && !iteration && reader.getIntVectorFromProperty("reduction-sizes").empty()) {
                    std::cerr << "[ERROR] Missing iteration or reduction-sizes parameter for random reduction method in configuration file: "
                              << path << "." << std::endl;
                    return;
                }

                try {
                    kernel.getTestSuiteReductionPluginManager().getPlugin(*it);
                } catch (std::out_of_range &e) {
                    std::cerr << "[ERROR] Invalid reduction algorithm name(" << *it
                              << ") in configuration file: " << path << "." << std::endl;
                    return;
                }
            }
        }

        String covPath = reader.getStringFromProperty("coverage-data");
        if (covPath[0] == '.') {
            covPath = jsonPath.parent_path().string() + "/" + covPath;
        }

        String resPath = reader.getStringFromProperty("results-data");
        if (resPath[0] == '.') {
            resPath = jsonPath.parent_path().string() + "/" + resPath;
        }

        if (exists(covPath)
                && exists(resPath)) {
            (std::cerr << "[INFO] loading coverage from " << covPath << " ...").flush();
            selectionData.loadCoverage(covPath);
            (std::cerr << " done\n[INFO] loading results from " << resPath << " ...").flush();
            selectionData.loadResults(resPath);
            (std::cerr << " done" << std::endl).flush();
        } else {
            std::cerr << "[ERROR] Missing or invalid input files in config file " << path << "." << std::endl;
            return;
        }

        if (reader.getBoolFromProperty("globalize")) {
            // Globalize data.
            (std::cerr << "[INFO] Globalizing ... ").flush();
            selectionData.globalize();
            selectionData.filterToCoverage();
            (std::cerr << " done" << std::endl).flush();
        }

        String dirPath = reader.getStringFromProperty("output-dir");
        if (dirPath[0] == '.') {
            dirPath = jsonPath.parent_path().string() + "/" + dirPath;
            reader.setProperty("output-dir", dirPath);
        }

        if (!(boost::filesystem::exists(dirPath))) {
            boost::filesystem::create_directory(dirPath);
        }

        boost::filesystem::path p = boost::filesystem::path(covPath).filename();

        while (!reductionList.empty()) {
            string reductionMethod = reductionList.back();
            reductionList.pop_back();
            ITestSuiteReductionPlugin *plugin = NULL;

            std::ofstream outStream((dirPath + "/" + reductionMethod + "-" + p.string() + ".reduced").c_str());
            if (!outStream.good()) {
                throw CException("Reduction output file error.", reader.getStringFromProperty("coverage-data") + ".reduced");
            }

            try {
                plugin = kernel.getTestSuiteReductionPluginManager().getPlugin(reductionMethod);
                plugin->init(&selectionData, reader);
            } catch (std::out_of_range &e) {
                std::cerr << "[ERROR] Unknown reduction mode. " << std::endl;
                printPluginNames(kernel.getTestSuiteReductionPluginManager().getPluginNames());
                return;
            }
            plugin->reduction(outStream);
            outStream.close();
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
