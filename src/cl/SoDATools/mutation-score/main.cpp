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

    if (!vm.count("output")) {
        ERRO("The output file is missing!" << std::endl << desc);
        return 1;
    }

    String tmp = vm["load-mutation-results"].as<String>();
    selectionData.loadResults(tmp);

    results.SetObject();
    args.SetObject();

    // Mutation score metric
    IMutationMetricPlugin *resultScoreMetricPlugin = kernel.getMutationMetricPluginManager().getPlugin("results-score");
    resultScoreMetricPlugin->init(&selectionData, args, NULL);
    resultScoreMetricPlugin->calculate(results);

    out.open(vm["output"].as<String>());
    out << "pass-results-score;fail-results-score" << std::endl;

    const char* MUTATION_METRICS_TAG = "mutation-metrics";
    out << results[MUTATION_METRICS_TAG]["pass-results-score"].GetDouble() << ";";
    out << results[MUTATION_METRICS_TAG]["fail-results-score"].GetDouble() << std::endl;

    out.close();
}

int main(int argc, char *argv[])
{
    std::cout << "mutation-score (SoDA tool)" << std::endl;
    desc.add_options()
        ("help,h", "produce help message")
        ("load-mutation-results,r", value<String>(), "input mutation test result file")
        ("output,o", value<String>(), "output file")
        ;

    if (argc < 2) {
        ERRO("There are no arguments!" << std::endl << desc);
        return 1;
    }

    return processArgs(desc, argc, argv);
}
