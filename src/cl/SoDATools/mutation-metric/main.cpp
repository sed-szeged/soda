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

/**
 * @brief Process arguments.
 ** @param desc The parameter,value pairs.
 * @param ac The number of arguments.
 * @param av The arguments.
 * @return
 */
int processArgs(options_description desc, int ac, char* av[])
{
    std::ofstream out;
    rapidjson::Document results;
    rapidjson::Document args;

    variables_map vm;
    positional_options_description p;
    store(command_line_parser(ac, av).options(desc).positional(p).run(), vm);
    notify(vm);

    /*
     * LOAD DATA
     */

    if (!vm.count("load-mutation-results")) {
        ERRO("The mutation result binary is missing!" << std::endl << desc);
        return 1;
    }

    if (!vm.count("mutation-method-coverage")) {
        ERRO("The mutation method coverage binary is missing!" << std::endl << desc);
        return 1;
    }

    if (!vm.count("mutation-point-coverage")) {
        ERRO("The mutation point coverage binary is missing!" << std::endl << desc);
        return 1;
    }

    if (!vm.count("mutation-map")) {
        ERRO("The mutation map is missing!" << std::endl << desc);
        return 1;
    }

    if (!vm.count("output")) {
        ERRO("The output file is missing!" << std::endl << desc);
        return 1;
    }

    String tmp = vm["load-mutation-results"].as<String>();
    selectionData.loadResults(tmp);
    tmp = vm["mutation-method-coverage"].as<String>();
    selectionData.loadCoverage(tmp);

    results.SetObject();
    args.SetObject();

    // Mutation score metric
    IMutationMetricPlugin *resultScoreMetricPlugin = kernel.getMutationMetricPluginManager().getPlugin("results-score");
    resultScoreMetricPlugin->init(&selectionData, args, NULL);
    resultScoreMetricPlugin->calculate(results);

    rapidjson::Value v;
    v.SetString(vm["mutation-point-coverage"].as<String>().c_str(), args.GetAllocator());
    const char* MUTATION_COVERAGE_PATH = "mutation-coverage";
    const char* MUTATION_MAP_PATH = "mutation-map";
    args.AddMember(rapidjson::StringRef(MUTATION_COVERAGE_PATH), v, args.GetAllocator());
    v.SetString(vm["mutation-map"].as<String>().c_str(), args.GetAllocator());
    args.AddMember(rapidjson::StringRef(MUTATION_MAP_PATH), v, args.GetAllocator());

    IMutationMetricPlugin *mutationCategoryMetricPlugin = kernel.getMutationMetricPluginManager().getPlugin("mutation-category");
    mutationCategoryMetricPlugin->init(&selectionData, args, NULL);
    mutationCategoryMetricPlugin->calculate(results);

    out.open(vm["output"].as<String>());
    out << "pass-results-score;fail-results-score;type-1;type-2;type-3;type-4A;type-4B" << std::endl;

    const char* MUTATION_METRICS_TAG = "mutation-metrics";
    out << results[MUTATION_METRICS_TAG]["pass-results-score"].GetDouble() << ";";
    out << results[MUTATION_METRICS_TAG]["fail-results-score"].GetDouble() << ";";
    out << results[MUTATION_METRICS_TAG]["type-1"].GetInt64() << ";";
    out << results[MUTATION_METRICS_TAG]["type-2"].GetInt64() << ";";
    out << results[MUTATION_METRICS_TAG]["type-3"].GetInt64() << ";";
    out << results[MUTATION_METRICS_TAG]["type-4A"].GetInt64() << ";";
    out << results[MUTATION_METRICS_TAG]["type-4B"].GetInt64() << std::endl;

    out.close();

    return 0;
}

int main(int argc, char *argv[])
{
    std::cout << "mutation-metric (SoDA tool)" << std::endl;
    desc.add_options()
        ("help,h", "produce help message")
        ("load-mutation-results,r", value<String>(), "input mutation test result file")
        ("mutation-method-coverage,c", value<String>(), "input mutation method coverage binary file")
        ("mutation-point-coverage,p", value<String>(), "input mutation point coverage binary file")
        ("mutation-map,m", value<String>(), "input mutation test result file")
        ("output,o", value<String>(), "output file")
        ;

    if (argc < 2) {
        ERRO("There are no arguments!" << std::endl << desc);
        return 1;
    }

    return processArgs(desc, argc, argv);
}
