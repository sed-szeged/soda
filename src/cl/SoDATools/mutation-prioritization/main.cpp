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
#include <fstream>
#include <vector>

using namespace soda;
using namespace boost::program_options;

#define ERRO(X)   std::cerr << "[*ERROR*] " << X << std::endl

options_description desc("Allowed options");

CKernel kernel;
CSelectionData selectionData;
std::ofstream out;
variables_map vm;

void calculate(String prioAlgorithm, std::vector<int> sizes)
{
    for (auto size : sizes) {
        // General coverage
        IntVector tcidList;
        rapidjson::Document results;
        results.SetObject();

        // Prio algorithm
        ITestSuitePrioritizationPlugin *prioritizationPlugin = kernel.getTestSuitePrioritizationPluginManager().getPlugin(prioAlgorithm);
        prioritizationPlugin->init(&selectionData);
        prioritizationPlugin->fillSelection(tcidList, size);

        // Coverage metric
        std::map<std::string, CClusterDefinition> clusterList;
        CClusterDefinition cluster;
        cluster.addTestCases(tcidList);
        cluster.addCodeElements(selectionData.getCoverage()->getCodeElements().getIDList());

        clusterList["list"] = cluster;

        ITestSuiteMetricPlugin *coverageMetric = kernel.getTestSuiteMetricPluginManager().getPlugin("coverage");
        coverageMetric->init(&selectionData, &clusterList, 0);
        coverageMetric->calculate(results);

        // Mutation category metric
        rapidjson::Document args;
        args.SetObject();

        bool hasCategories = false;
        if (vm.count("mutation-coverage") && vm.count("mutation-map")) {
            hasCategories = true;
            rapidjson::Value v;
            v.SetString(vm["mutation-coverage"].as<String>().c_str(), args.GetAllocator());
            const char* MUTATION_COVERAGE_PATH = "mutation-coverage";
            const char* MUTATION_MAP_PATH = "mutation-map";
            args.AddMember(rapidjson::StringRef(MUTATION_COVERAGE_PATH), v, args.GetAllocator());
            v.SetString(vm["mutation-map"].as<String>().c_str(), args.GetAllocator());
            args.AddMember(rapidjson::StringRef(MUTATION_MAP_PATH), v, args.GetAllocator());

            IMutationMetricPlugin *mutationCategoryMetricPlugin = kernel.getMutationMetricPluginManager().getPlugin("mutation-category");
            mutationCategoryMetricPlugin->init(&selectionData, args, &tcidList);
            mutationCategoryMetricPlugin->calculate(results);
        }

        // Mutation score metric
        IMutationMetricPlugin *resultScoreMetricPlugin = kernel.getMutationMetricPluginManager().getPlugin("results-score");
        resultScoreMetricPlugin->init(&selectionData, args, &tcidList);
        resultScoreMetricPlugin->calculate(results);

        // Save output
        const char* MUTATION_METRICS_TAG = "mutation-metrics";
        out << prioAlgorithm << ";";
        out << size << ";";
        out << results["list"]["coverage"].GetDouble() << ";"; // Coverage
        out << results[MUTATION_METRICS_TAG]["pass-results-score"].GetDouble() << ";";
        out << results[MUTATION_METRICS_TAG]["fail-results-score"].GetDouble() << ";";
        if (hasCategories) {
            out << results[MUTATION_METRICS_TAG]["type-1"].GetInt64() << ";";
            out << results[MUTATION_METRICS_TAG]["type-2"].GetInt64() << ";";
            out << results[MUTATION_METRICS_TAG]["type-3"].GetInt64() << ";";
            out << results[MUTATION_METRICS_TAG]["type-4A"].GetInt64() << ";";
            out << results[MUTATION_METRICS_TAG]["type-4B"].GetInt64() << ";";
        } else {
            out << "-;-;-;-;-";
        }
        out << std::endl;
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
    store(command_line_parser(ac, av).options(desc).positional(p).run(), vm);
    notify(vm);

    /*
     * LOAD DATA
     */
    if (!vm.count("load-coverage")) {
        ERRO("The coverage binary is missing!" << std::endl << desc);
        return 0;
    }

    if (!vm.count("load-mutation-results")) {
        ERRO("The mutation result binary is missing!" << std::endl << desc);
        return 0;
    }

    if (!vm.count("sizes")) {
        ERRO("The sizes are missing!" << std::endl << desc);
        return 0;
    }


    String tmp = vm["load-coverage"].as<String>();
    selectionData.loadCoverage(tmp);
    tmp = vm["load-mutation-results"].as<String>();
    selectionData.loadResults(tmp);

    std::vector<int> sizes = vm["sizes"].as<std::vector<int> >();

    out.open(vm["output"].as<String>());
    out << "prio-algorithm;size;COV;pass-results-score;fail-results-score;type-1;type-2;type-3;type-4A;type-4B" << std::endl;

    calculate("general-ignore", sizes);
    calculate("additional-general-ignore", sizes);
    calculate("random-ignore", sizes);

    out.close();

    return 0;
}

int main(int argc, char *argv[])
{
    std::cout << "mutation-prioritization (SoDA tool)" << std::endl;
    desc.add_options()
        ("help,h", "produce help message")
        ("load-coverage,c", value<String>(), "input program coverage file")
        ("load-mutation-results,r", value<String>(), "input mutation test result file")
        ("mutation-coverage", value<String>(), "input mutation test result file")
        ("mutation-map", value<String>(), "input mutation test result file")
        ("sizes,s",value<std::vector<int> >()->multitoken(), "The number of selection sizes")
        ("output,o", value<String>(), "output file")
        ;

    if (argc < 2) {
        ERRO("There are no arguments!" << std::endl << desc);
        return 1;
    }

    return processArgs(desc, argc, argv);
}
