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

#include <exception>
#include <iostream>

#include "boost/program_options.hpp"

#include "CRawDataReaderPluginManager.h"

using namespace boost::program_options;
using namespace soda;

CRawDataReaderPluginManager pluginManager;
int processArgs(options_description desc, int ac, char* av[]);

int main(int argc, char *argv[])
{
    std::cout << "rawDataReader (SoDA tool)" << std::endl;
    options_description desc("Allowed options");
    desc.add_options()
            ("help,h",                    "produce help message")
            ("type,t",   value<String>(), "type of reading. Could be: coverage, results, changeset")
            ("mode,m",   value<String>(), "the reading mode")
            ("list,l",                    "list the available reading modes")
            ("output,o", value<String>(), "output file")
            ("path,p",   value<String>(), "path to input directory")
    ;

    if(argc < 2) {
        std::cout << "There are no arguments!" << std::endl << desc;
        return 1;
    }
    return processArgs(desc, argc, argv);
}

void printPluginNames(const std::string &type, const std::vector<std::string> &plugins)
{
    std::cout << "The available read modes for reading type: " << type << std::endl;
    for (size_t i = 0; i < plugins.size(); i++) {
        std::cout << "  * " << plugins[i] << std::endl;
    }
}

int processArgs(options_description desc, int ac, char* av[])
{
    try {
        positional_options_description p;
        variables_map vm;
        store(command_line_parser(ac, av).options(desc).positional(p).run(), vm);
        notify(vm);

        if (!vm.count("type")) {
            std::cerr << "[ERROR] Type is missing." << std::endl << desc;
            return 1;
        }
        if (vm.count("mode") && !vm.count("path")) {
            std::cerr << "[ERROR] Path to input directory is missing." << std::endl << desc;
            return 1;
        }
        std::string type = vm["type"].as<std::string>();


        if (type == "coverage") {
            pluginManager.loadCoverageReaderPlugins();
            if (vm.count("list")) {
                printPluginNames("coverage", pluginManager.getCoverageReaderPluginNames());
                return 0;
            }
            if (vm.count("mode")) {
                std::string mode = vm["mode"].as<std::string>();
                std::string path = vm["path"].as<std::string>();
                try {
                    ICoverageReaderPlugin *plugin = pluginManager.getCoverageReaderPlugin(mode);
                    CCoverageMatrix *matrix = plugin->read(path);
                    if (vm.count("output")) {
                        std::string output = vm["output"].as<std::string>();
                        matrix->save(output);
                    }
                    if (matrix) {
                        delete matrix;
                    }
                } catch (std::out_of_range &e) {
                    std::cerr << "[ERROR] Unknown read mode. " << std::endl;
                    printPluginNames("coverage", pluginManager.getCoverageReaderPluginNames());
                    return 1;
                }
                return 0;
            }
        } else if (type == "results") {
            pluginManager.loadResultsReaderPlugins();
            if (vm.count("list")) {
                printPluginNames("results", pluginManager.getResultsReaderPluginNames());
                return 0;
            }
            if (vm.count("mode")) {
                std::string mode = vm["mode"].as<std::string>();
                std::string path = vm["path"].as<std::string>();
                try {
                    IResultsReaderPlugin *plugin = pluginManager.getResultsReaderPlugin(mode);
                    CResultsMatrix *results = plugin->read(path);
                    if (vm.count("output")) {
                        std::string output = vm["output"].as<std::string>();
                        results->save(output);
                    }
                    if (results) {
                        delete results;
                    }
                } catch (std::out_of_range &e) {
                    std::cerr << "[ERROR] Unknown read mode. " << std::endl;
                    printPluginNames("results", pluginManager.getResultsReaderPluginNames());
                    return 1;
                }
                return 0;
            }
        } else if (type == "changeset") {
            pluginManager.loadChangesetReaderPlugins();
            if (vm.count("list")) {
                printPluginNames("changeset", pluginManager.getChangesetReaderPluginNames());
                return 0;
            }
            if (vm.count("mode")) {
                std::string mode = vm["mode"].as<std::string>();
                std::string path = vm["path"].as<std::string>();
                try {
                    IChangesetReaderPlugin *plugin = pluginManager.getChangesetReaderPlugin(mode);
                    CChangeset *changeset = plugin->read(path);
                    if (vm.count("output")) {
                        std::string output = vm["output"].as<std::string>();
                        changeset->save(output);
                    }
                    if (changeset) {
                        delete changeset;
                    }
                } catch (std::out_of_range &e) {
                    std::cerr << "[ERROR] Unknown read mode. " << std::endl;
                    printPluginNames("changeset", pluginManager.getChangesetReaderPluginNames());
                    return 1;
                }
                return 0;
            }
        } else {
            std::cerr << "[ERROR] Unknown type." << std::endl << desc;
            return 1;
        }

    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch(...) {
        std::cerr << "Exception of unknown type while processsing command line arguments!" << std::endl;
        return 1;
    }
    return 0;
}
