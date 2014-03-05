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
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include "data/CSelectionData.h"
#include "io/CJsonReader.h"
#include "CRevisionFilters.h"
#include "CSelectionsPluginManager.h"
#include "CComputeSelectionMetrics.h"
#include "plugins/prioritization/IPrioritizationPlugin.h"

using namespace soda;
using namespace soda::io;
using namespace std;
using namespace boost::filesystem;

void runMeasurement(String path);
int loadJsonFiles(String path);
void printPluginNames(const std::string &type, const std::vector<std::string> &plugins);

CSelectionsPluginManager pluginManager;

int main(int argc, char* argv[]) {
    cout << "selections (SoDA tool)" << endl;

    if (argc < 2) {
        cerr << "ERROR: There are no arguments!" << endl;
        return 1;
    }

    return loadJsonFiles(String(argv[1]));
}

void printPluginNames(const std::string &type, const std::vector<std::string> &plugins)
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
        runMeasurement(path);
    } else if (is_directory(path)) {
        directory_iterator endIt;
        for (directory_iterator it(path); it != endIt; it++) {
            if (!is_regular_file(it->status())) {
                continue;
            }
            if (it->path().extension() != ".json") {
                continue;
            }
            runMeasurement(it->path().string());
        }
    }
    return 0;
}

void runMeasurement(String path)
{
    try {
        CSelectionData selectionData;
        CJsonReader reader = CJsonReader(path);
        pluginManager.loadPrioritizationPlugins();

        if (!reader.existsProperty("prioritization-algorithms")) {
            std::cerr << "[ERROR] prioritization-algorithms is missing from the configuration file." << std::endl;
            printPluginNames("prioritization", pluginManager.getPrioritizationPluginNames());
            return;
        }

        if (reader.existsProperty("coverage-data") && reader.existsProperty("changeset") && reader.existsProperty("results-data")) {
            (cerr << "[INFO] loading coverage from " << reader.getStringFromProperty("coverage-data") << " ...").flush();
            selectionData.loadCoverage(reader.getStringFromProperty("coverage-data"));
            (cerr << " done\n[INFO] loading changes from " << reader.getStringFromProperty("changeset") << " ...").flush();
            selectionData.loadChangeset(reader.getStringFromProperty("changeset"));
            (cerr << " done\n[INFO] loading results from " << reader.getStringFromProperty("results-data") << " ...").flush();
            selectionData.loadResults(reader.getStringFromProperty("results-data"));
            (cerr << " done" << endl).flush();
        } else {
            std::cerr << "[ERROR] Missing input files." << std::endl;
            return;
        }

        IntVector sizelist = reader.getIntVectorFromProperty("selection-sizes");

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

        StringVector priolist = reader.getStringVectorFromProperty("prioritization-algorithms");

        std::ofstream of;
        if (reader.existsProperty("output-file")) {
            of.open((reader.getStringFromProperty("output-file") + ".csv").c_str());
        }

        while(!priolist.empty()) {
            string t = priolist.back();
            priolist.pop_back();

            IPrioritizationPlugin *plugin = NULL;
            try {
                plugin = pluginManager.getPrioritizationPlugin(t);
                plugin->init(&selectionData);
            } catch (std::out_of_range &e) {
                std::cerr << "[ERROR] Unknown algorithm mode. " << std::endl;
                printPluginNames("prioritization", pluginManager.getPrioritizationPluginNames());
                return;
            }

            CComputeSelectionMetrics *selectionStat = new CComputeSelectionMetrics(&selectionData, plugin, &revisionlist, &sizelist, reader.getIntFromProperty("progress-level"));
            (cerr << "[INFO] Measurements on " << revisionlist.size() << " revisions ...").flush();
            selectionStat->runMeasurement();
            (cerr << " done." << endl).flush();

            if (reader.getBoolFromProperty("print-details")) {
                if (reader.existsProperty("output-file"))
                    of << selectionStat->getDetailedData();
                else {
                    std::cout.precision(5);
                    std::cout << selectionStat->getDetailedData();
                }
            } else {
                if (reader.existsProperty("output-file"))
                    of << selectionStat->getData();
                else {
                    std::cout.precision(5);
                    std::cout << selectionStat->getData();
                }
            }
            delete selectionStat;
        }
        of.close();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return;
    } catch (...) {
        std::cerr << "Exception of unknown type while processsing command line arguments!" << std::endl;
        return;
    }
    return;
}
