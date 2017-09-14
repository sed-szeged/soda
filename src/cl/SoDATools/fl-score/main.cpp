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
#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/ostreamwrapper.h>

#include "data/CClusterDefinition.h"
#include "data/CSelectionData.h"
#include "engine/CKernel.h"
#include "util/CTestSuiteScore.h"


using namespace soda;
using namespace soda::io;
using namespace boost::program_options;

namespace fs = boost::filesystem;

void processJsonFiles(variables_map &vm);
int  loadJsonFiles(String path);
void printPluginNames(const String &type, const std::vector<String> &plugins);
void printHelp();
String getJsonString();
void createJsonFile();

CKernel kernel;

IndexType revision;
fs::path outputDir;
ClusterMap clusterList;

int main(int argc, char* argv[]) {
    std::cerr << "fl-score (SoDA tool)" << std::endl;

    options_description desc("Options");
    desc.add_options()
            ("help,h", "Prints help message")
            ("list-cluster-algorithms", "Lists the cluster algorithms")
            ("list-fault-localization-techniques", "Lists the fault localization technique plugins")
            ("coverage-matrix,c", value<String>(), "Coverage matrix binary")
            ("results-matrix,r", value<String>(), "Results matrix binary")
            ("changeset,d", value<String>(), "Changeset binary")
            ("output-dir,o", value<String>(), "Output directory")
            ("globalize,g", "Globalize")
            ("filter-to-coverage,f", "Filter to coverage");

    variables_map vm;
    positional_options_description p;
    store(command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    notify(vm);

    if (argc < 2) {
        std::cout << desc << std::endl;
        std::cerr << "[ERROR] There are no arguments." << std::endl;
        return 1;
    }

    if (vm.count("help")) {
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

    if (!vm.count("output-dir")) {
        std::cerr << "[ERROR] Path to output directory is missing." << std::endl << desc << std::endl;
        return 1;
    }
    outputDir = fs::path(vm["output-dir"].as<String>());

    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }

    if (!vm.count("coverage-matrix")) {
        std::cerr << "[ERROR] Path to coverage matrix binary is missing." << std::endl << desc << std::endl;
        return 1;
    }
    String covPath = vm["coverage-matrix"].as<String>();

    if (!vm.count("results-matrix")) {
        std::cerr << "[ERROR] Path to results matrix binary is missing." << std::endl << desc << std::endl;
        return 1;
    }
    String resPath = vm["results-matrix"].as<String>();

    if (!vm.count("changeset")) {
        std::cerr << "[ERROR] Path to changeset binary is missing." << std::endl << desc << std::endl;
        return 1;
    }
    String chPath = vm["changeset"].as<String>();

    clusterList.clear();

    CSelectionData selectionData;
    rapidjson::Document reader;

    ITestSuiteClusterPlugin *clusterAlgorithm = kernel.getTestSuiteClusterPluginManager().getPlugin("one-cluster");
    clusterAlgorithm->init(reader);

    (std::cerr << "[INFO] loading coverage from " << covPath << " ...").flush();
    selectionData.loadCoverage(covPath);
    (std::cerr << " done\n[INFO] loading results from " << resPath << " ...").flush();
    selectionData.loadResults(resPath);
    (std::cerr << " done" << std::endl).flush();
    selectionData.loadChangeset(chPath);


    if (vm.count("globalize")) {
        selectionData.globalize();
    }

    if (vm.count("filter-to-coverage")) {
        selectionData.filterToCoverage();
    }

    (std::cerr << "[INFO] Running cluster algorithm: " << clusterAlgorithm->getName() << " ...").flush();
    clusterAlgorithm->execute(selectionData, clusterList);
    (std::cerr << " done." << std::endl).flush();

    StringVector faultLocalizationTechniques;
    faultLocalizationTechniques.push_back("dstar");
    faultLocalizationTechniques.push_back("tarantula");
    faultLocalizationTechniques.push_back("ochiai");

    IntVector revisions = selectionData.getResults()->getRevisionNumbers();

    for (IntVector::iterator revIt = revisions.begin(); revIt != revisions.end(); revIt++) {
        RevNumType revision = *revIt;
        if (!selectionData.getChangeset()->exists(revision)) {
            std::cerr << "[WARNING] Revision '" << revision << " is missing from changeset." << std::endl;
            continue;
        }
        rapidjson::Document result;
        result.SetObject();

        IntVector failedCodeElements;

        std::cerr << "[INFO] Calculating scores for revision: " << revision << std::endl;

        for (IndexType cid = 0; cid < selectionData.getChangeset()->getCodeElements().size(); cid++) {
            if (selectionData.getChangeset()->isChanged(revision, selectionData.getChangeset()->getCodeElements().getValue(cid))) {
                failedCodeElements.push_back(selectionData.translateCodeElementIdFromChangesetToCoverage(cid));
                //break;
            }
        }

        IFaultLocalizationTechniquePlugin *commonTechnique = kernel.getFaultLocalizationTechniquePluginManager().getPlugin("common");

        commonTechnique->init(&selectionData, &clusterList, revision, failedCodeElements);
        commonTechnique->calculate(result);

        for (IndexType i = 0; i < faultLocalizationTechniques.size(); i++) {
            std::string flTechniqueName = faultLocalizationTechniques[i];
            IFaultLocalizationTechniquePlugin *technique = kernel.getFaultLocalizationTechniquePluginManager().getPlugin(flTechniqueName);

            technique->init(&selectionData, &clusterList, revision, failedCodeElements);
            technique->calculate(result);

            IFaultLocalizationTechniquePlugin::FLScore flScores = technique->getFlScore();
            std::map<String, CClusterDefinition>::iterator it;
            for (it = clusterList.begin(); it != clusterList.end(); it++) {
                for (IndexType j = 0; j < failedCodeElements.size(); j++) {
                    IndexType cid = failedCodeElements[j];
                    if (std::find(it->second.getCodeElements().begin(), it->second.getCodeElements().end(), cid) != it->second.getCodeElements().end()) {
                        std::cout << revision << ";" << technique->getName() << ";" << it->first << ";" << selectionData.getCoverage()->getCodeElements().getValue(cid) << ";" << flScores[it->first][cid] << std::endl;
                    }
                }
            }
        }

        fs::path file((boost::format{"result-%s.json"} % revision).str());
        std::ofstream outputFileStream((outputDir / file).string(), std::ofstream::out);

        if (!outputFileStream.is_open()) {
            std::cerr << "[ERROR] Cannot open output file '" << file << "'." << std::endl;
            return 1;
        }

        rapidjson::OStreamWrapper os(outputFileStream);
        rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(os);

        result.Accept(writer);
    }

    return 0;
}

void printPluginNames(const String &type, const std::vector<String> &plugins)
{
    std::cout << "The available algorithm modes for algorithm type: " << type << std::endl;
    for (size_t i = 0; i < plugins.size(); i++) {
        std::cout << "  * " << plugins[i] << std::endl;
    }
}
