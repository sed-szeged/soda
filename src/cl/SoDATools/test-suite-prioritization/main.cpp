/*
 * Copyright (C): 2013-2016 Department of Software Engineering, University of Szeged
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

#include <boost/program_options.hpp>
#include <fstream>
#include <sstream>
#include <vector>

#include "algorithm/CPartitionAlgorithm.h"
#include "data/CSelectionData.h"
#include "data/SoDALibDefs.h"
#include "engine/CKernel.h"


namespace po = boost::program_options;
using namespace soda;

#define ERRO(X)   std::cerr << "[*ERROR*] " << X << std::endl

po::options_description desc("Allowed options");
CKernel kernel;
CSelectionData selectionData;

void printPluginNames(const String &type, const std::vector<String> &plugins)
{
    std::cout << "The available algorithm modes for algorithm type: " << type << std::endl;
    for (size_t i = 0; i < plugins.size(); i++) {
        std::cout << "  * " << plugins[i] << std::endl;
    }
}

// TODO move this to SoDA algorithms
double coverage(IntVector &testIds)
{
    CCoverageMatrix *matrix = selectionData.getCoverage();

    IndexType nrOfCodeElements = matrix->getNumOfCodeElements();
    IndexType nrOfTestcases = testIds.size();
    IndexType nrOfCoveredCodeElements = 0;
    std::set<IndexType> notCoveredElementIds;

    for (IndexType cid = 0; cid < nrOfCodeElements; cid++) {
        notCoveredElementIds.insert(cid);
    }

    for (IndexType i = 0; i < nrOfTestcases; i++) {
        IndexType tcid = testIds[i];
        std::set<IndexType>::iterator it = notCoveredElementIds.begin();
        while (it != notCoveredElementIds.end()) {
            IndexType cid = *it;
            if (matrix->getBitMatrix().get(tcid, cid)) {
                nrOfCoveredCodeElements++;
                notCoveredElementIds.erase(it++);
            } else {
                it++;
            }
        }
    }

    return (double)nrOfCoveredCodeElements / (double)matrix->getNumOfCodeElements();
}

// TODO move this to SoDA algorithms
double partitionMetric(CClusterDefinition &cluster)
{
    CPartitionAlgorithm algorithm;
    algorithm.compute(selectionData, cluster);

    CPartitionAlgorithm::PartitionInfo &partitionInfo = algorithm.getPartitionInfo();
    CPartitionAlgorithm::PartitionData &partitions = algorithm.getPartitions();

    IndexType nrOfCodeElementsInPartition = partitionInfo.size();
    double flMetric = 0.0;

    for (CPartitionAlgorithm::PartitionData::iterator partIt = partitions.begin(); partIt != partitions.end(); partIt++) {
        IndexType size = partIt->second.size();

        flMetric += size * (size - 1);
    }

    if(nrOfCodeElementsInPartition == 0){
        flMetric = 0;
    } else if(nrOfCodeElementsInPartition == 1){
        flMetric /= nrOfCodeElementsInPartition * (nrOfCodeElementsInPartition);
    } else {
        flMetric /= nrOfCodeElementsInPartition * (nrOfCodeElementsInPartition - 1);
    }

    return 1.0 - flMetric;
}

int processArgs(options_description desc, int ac, char* av[])
{
    po::variables_map vm;
    //po::positional_options_description p;
    po::parsed_options parsed = po::command_line_parser(ac, av).options(desc).allow_unregistered().run();
    po::store(parsed, vm);
    //po::notify(vm);

    if (vm.count("list-prioritization-plugins")) {
        printPluginNames("prioritization", kernel.getTestSuitePrioritizationPluginManager().getPluginNames());
        return 0;
    }

    if (!vm.count("mode")) {
        ERRO("Mode is missing!" << std::endl << desc);
        return 1;
    }

    if (!vm.count("load-coverage")) {
        ERRO("Coverage SoDA binary is missing!" << std::endl << desc);
        return 1;
    }

    if (!vm.count("plugin")) {
        ERRO("Prioritization plugin is missing!" << std::endl);
        printPluginNames("prioritization", kernel.getTestSuitePrioritizationPluginManager().getPluginNames());
        std::cerr << desc;
        return 1;
    }

    String coveragePath = vm["load-coverage"].as<String>();
    (std::cout << "[INFO] Loading coverage ... ").flush();
    selectionData.loadCoverage(coveragePath);
    (std::cout << "done." << std::endl ).flush();

    // Get the plugin
    String pluginName = vm["plugin"].as<String>();
    ITestSuitePrioritizationPlugin *plugin;
    plugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin(pluginName);
    plugin->init(&selectionData, &kernel);

    String mode = vm["mode"].as<String>();
    if (mode == "size") {
        po::options_description sizeDesc;
        sizeDesc.add_options()
            ("sizes,s", po::value<std::vector<size_t> >()->multitoken(), "The number of tests selected e.g: 10 20 30")
            ;
        // Collect all the unrecognized options from the first pass. This will include the
        // (positional) command name, so we need to erase that.
        std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);

        // Parse again...
        po::store(po::command_line_parser(opts).options(sizeDesc).run(), vm);

        if (!vm.count("sizes")) {
            ERRO("Missing selection sizes!" << std::endl << sizeDesc);
        }
        std::vector<size_t> sizes = vm["sizes"].as<std::vector<size_t> >();
        for (auto size : sizes) {
            std::stringstream fileName;
            fileName << "test-prioritization-" << pluginName << "-" << size << ".csv";
            std::ofstream out;
            out.open(fileName.str().c_str());
            (out << "tcid;test name" << std::endl).flush();
            IntVector result;
            plugin->fillSelection(result, size);
            for (IndexType i = 0; i < size && i < selectionData.getCoverage()->getNumOfTestcases(); i++) {
                IndexType tcid = plugin->next();
                (out << tcid << ";" << selectionData.getCoverage()->getTestcases().getValue(tcid) << std::endl).flush();
            }
            out.close();
        }
    } else if (mode == "max-coverage") {
        std::stringstream fileName;
        fileName << "test-prioritization-" << pluginName << "-max-coverage" << ".csv";
        std::ofstream out;
        out.open(fileName.str().c_str());
        (out << "tcid;test name" << std::endl).flush();

        IntVector allTests = selectionData.getCoverage()->getTestcases().getIDList();
        double maxCoverage = coverage(allTests);
        double selectedCoverage = 0.0;
        IntVector result;
        while (selectedCoverage < maxCoverage && result.size() < selectionData.getCoverage()->getNumOfTestcases()) {
            IndexType tcid = plugin->next();
            result.push_back(tcid);
            (out << tcid << ";" << selectionData.getCoverage()->getTestcases().getValue(tcid) << std::endl).flush();
            selectedCoverage = coverage(result);
        }
        out.close();
    } else if (mode == "max-partition") {
        std::stringstream fileName;
        fileName << "test-prioritization-" << pluginName << "-max-partition" << ".csv";
        std::ofstream out;
        out.open(fileName.str().c_str());
        (out << "tcid;test name" << std::endl).flush();

        CClusterDefinition cluster;
        cluster.addTestCases(selectionData.getCoverage()->getTestcases().getIDList());
        cluster.addCodeElements(selectionData.getCoverage()->getCodeElements().getIDList());

        double maxPartitionMetric = partitionMetric(cluster);
        double selectedPartitionMetric = 0.0;
        // Clear tests from cluser
        cluster.clearTestCases();

        while (selectedPartitionMetric < maxPartitionMetric && cluster.getNumOfTestCases() < selectionData.getCoverage()->getNumOfTestcases()) {
            IndexType tcid = plugin->next();
            cluster.addTestCase(tcid);
            (out << tcid << ";" << selectionData.getCoverage()->getTestcases().getValue(tcid) << std::endl).flush();
            selectedPartitionMetric = partitionMetric(cluster);
        }
        out.close();
    } else {
        ERRO("Unknown mode!" << std::endl << desc);
    }
}


int main(int argc, char *argv[])
{
    std::cout << "test-suite-prioritization (SoDA tool)" << std::endl;
    desc.add_options()
        ("help,h", "produce help message")
        ("load-coverage,c", value<String>(), "input coverage SoDA file")
        ("load-results,r", value<String>(), "input results SoDA file")
        ("plugin,p", value<String>(), "Name of the prioritization plugin to use")
        ("list-prioritization-plugins,l", "Lists the prioritization plugins")
        ("mode,m", value<String>(), "Can be: size, max-coverage, max-partition")
        ;

    if (argc < 2) {
        ERRO("There are no arguments!" << std::endl << desc);
        return 1;
    }

    return processArgs(desc, argc, argv);
}
