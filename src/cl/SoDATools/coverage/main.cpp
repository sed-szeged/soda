#include "data/CCoverageMatrix.h"
#include "boost/program_options.hpp"

#include "rapidjson/document.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/prettywriter.h"

#include "data/CClusterDefinition.h"
#include "data/CSelectionData.h"
#include "engine/CKernel.h"

#include <iostream>

using namespace soda;
using namespace boost::program_options;

#define ERRO(X)   std::cerr << "[*ERROR*] " << X << std::endl

options_description desc("Allowed options");

CKernel kernel;
CSelectionData selectionData;
std::map<std::string, CClusterDefinition> clusterList;

void printPluginNames(const String &type, const std::vector<String> &plugins)
{
    std::cout << "The available algorithm modes for algorithm type: " << type << std::endl;
    for (size_t i = 0; i < plugins.size(); i++) {
        std::cout << "  * " << plugins[i] << std::endl;
    }
}

/**
 * @brief Process arguments.
 ** @param desc The parameter,value pairs.
 * @param ac The number of arguments.
 * @param av The arguments.
 * @return
 */
int processArgs(options_description desc, int ac, char* av[])
{
    positional_options_description p;
    variables_map vm;
    store(command_line_parser(ac, av).options(desc).positional(p).run(), vm);
    notify(vm);

    if (vm.count("list-cluster-algorithms")) {
        printPluginNames("cluster", kernel.getTestSuiteClusterPluginManager().getPluginNames());
        return 0;
    }

    /*
     * LOAD DATA
     */
    if (!vm.count("load-coverage")) {
        ERRO("The coverage binary is missing!" << std::endl << desc);
        return 0;
    }


    String tmp = vm["load-coverage"].as<String>();
    selectionData.loadCoverage(tmp);

    rapidjson::Document parameters;
    parameters.SetObject();

    if (vm.count("cluster-test-list") && vm.count("cluster-code-elements-list")) {
        rapidjson::Value key;
        rapidjson::Value cluster;
        key.SetString("cluster-test-list", parameters.GetAllocator());
        cluster.SetString(vm["cluster-test-list"].as<String>().c_str(), parameters.GetAllocator());
        parameters.AddMember(key, cluster, parameters.GetAllocator());

        key.SetString("cluster-code-elements-list", parameters.GetAllocator());
        cluster.SetString(vm["cluster-code-elements-list"].as<String>().c_str(), parameters.GetAllocator());
        parameters.AddMember(key, cluster, parameters.GetAllocator());
    }


    String clusterAlgorithmName = vm["cluster-algorithm"].as<String>();
    ITestSuiteClusterPlugin *clusterAlgorithm = kernel.getTestSuiteClusterPluginManager().getPlugin(clusterAlgorithmName);
    clusterAlgorithm->init(parameters);

    (std::cerr << "[INFO] Running cluster algorithm: " << clusterAlgorithm->getName() << " ...").flush();
    clusterAlgorithm->execute(selectionData, clusterList);
    (std::cerr << " done" << std::endl).flush();

    rapidjson::Document results;
    results.SetObject();

    ITestSuiteMetricPlugin *metric = kernel.getTestSuiteMetricPluginManager().getPlugin("coverage");
    (std::cerr << "[INFO] Calculating metrics: " << metric->getName() << " ...").flush();
    metric->init(&selectionData, &clusterList, 0);
    metric->calculate(results);
    (std::cerr << " done." << std::endl).flush();

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
    results.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
    std::cout << sb.GetString() << std::endl;

}

int main(int argc, char *argv[])
{
    std::cout << "coverage (SoDA tool)" << std::endl;
    desc.add_options()
        ("help,h", "produce help message")
        ("load-coverage,c", value<String>(), "input file")
        ("list-cluster-algorithms,l", "Lists the cluster algorithms")
        ("cluster-algorithm", value<String>(), "cluster algorithm")
        ("cluster-test-list", value<String>(), "test list")
        ("cluster-code-elements-list", value<String>(), "code element list")
        ;

    if (argc < 2) {
        ERRO("There are no arguments!" << std::endl << desc);
        return 1;
    }

    return processArgs(desc, argc, argv);
}
