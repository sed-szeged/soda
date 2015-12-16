#include "data/CCoverageMatrix.h"
#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"
#include <fstream>
#include <iostream>
#include <utility>

using namespace soda;
using namespace boost::program_options;

#define ERRO(X)   std::cerr << "[*ERROR*] " << X << std::endl

CCoverageMatrix baseCov;
CCoverageMatrix comparableCov;
String programName { "default" };

int processArgs(options_description &desc, int ac, char* av[]);
void globalize();
std::pair<double, IdxStrMap> avgDiffAndHamming();
void save(double value);
void save(IdxStrMap &values);

int main(int argc, char *argv[])
{
    options_description desc("Allowed options");
    std::cout << "coverage (SoDA tool)" << std::endl;
    desc.add_options()
        ("help,h", "produce help message")
        ("load-coverage,c", value<String>(), "base coverage matrix")
        ("load-second-cov", value<String>(), "second coverage matrix")
        ("hamming-dist,h", value<String>(), "calculates hamming distance between the code elements vectors from the two matrices")
        ("average-difference,a", value<String>(), "calculates the number of different elements in the B matrix compared to A and divides with the size of the A matrix")
        ("program-name", value<String>(), "prefix of the output files")
        ("output-dir", value<String>(), "output directory")
        ;

    if (argc < 2) {
        ERRO("There are no arguments!" << std::endl << desc);
        return 1;
    }

    return processArgs(desc, argc, argv);
}

/**
 * @brief Process arguments.
 ** @param desc The parameter,value pairs.
 * @param ac The number of arguments.
 * @param av The arguments.
 * @return
 */
int processArgs(options_description &desc, int ac, char* av[])
{
    positional_options_description p;
    variables_map vm;
    store(command_line_parser(ac, av).options(desc).positional(p).run(), vm);
    notify(vm);

    /*
     * LOAD DATA
     */
    if (!vm.count("load-coverage") && !vm.count("load-second-cov")) {
        ERRO("Path of the coverage matrices are missing!" << std::endl << desc);
        return 1;
    }


    String basePath = vm["load-coverage"].as<String>();
    String comparePath = vm["load-second-cov"].as<String>();
    if (!boost::filesystem::exists(basePath)) {
        ERRO("The first coverage path (" << basePath << ") is not exists.");
        return 1;
    }

    if (!boost::filesystem::exists(comparePath)) {
        ERRO("The second coverage path (" << comparePath << ") is not exists.");
        return 1;
    }

    if (vm.count("program-name")) {
        programName = vm["program-name"].as<String>();
    }

    if (vm.count("output-dir")) {
        boost::filesystem::create_directories(vm["output-dir"].as<String>());
        programName = vm["output-dir"].as<String>() + programName;
    }

    enum SaveMask {
        NONE,
        AVG,
        HAMM,
        ALL
    };

    int saveMask = 0;
    if (vm.count("average-difference")) {
        saveMask |= AVG;
    }
    if (vm.count("hamming-dist")) {
        saveMask |= HAMM;
    }
    if (saveMask == NONE) {
        saveMask = ALL;
    }

    baseCov.load(basePath);
    comparableCov.load(comparePath);
    globalize();
    auto res = avgDiffAndHamming();
    if (saveMask & AVG) {
        save(res.first);
    }
    if (saveMask  & HAMM) {
        save(res.second);
    }
}

// Fills the second coverage matrix with the not existing test and code elements from the first coverage matrix.
void globalize() {
    for (const auto &ce : baseCov.getCodeElements().getValueList()) {
        comparableCov.addCodeElementName(ce);
    }
    for (const auto &tc : baseCov.getTestcases().getValueList()) {
        comparableCov.addTestcaseName(tc);
    }
    comparableCov.refitMatrixSize();
}

std::pair<double, IdxStrMap> avgDiffAndHamming() {
    IdxStrMap methodCovDiffs;
    double diff = 0;
    for (const auto &ce : baseCov.getCodeElements().getValueList()) {
        methodCovDiffs[ce] = 0;
        for (const auto &test : baseCov.getTestcases().getValueList()) {
            if (baseCov.getRelation(test, ce) != comparableCov.getRelation(test, ce)) {
                diff++;
                methodCovDiffs[ce]++;
            }
        }
    }
    diff /= (baseCov.getNumOfCodeElements() * baseCov.getNumOfTestcases());
    return std::make_pair(diff, methodCovDiffs);
}

void save(double value) {
    std::ofstream out((programName + "-cov-metric.csv").c_str());
    out << "Average difference;" << std::endl << value << std::endl;
}

void save(IdxStrMap &values) {
    std::ofstream out((programName + "-hamming-dist.csv").c_str());
    out << "Code element name;Number of different coverage values;" << std::endl;
    for (const auto &val : values) {
        out << val.first << ";" << val.second << std::endl;
    }
}
