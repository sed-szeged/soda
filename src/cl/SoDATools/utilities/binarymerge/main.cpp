/*
 * Copyright (C): 2015 Department of Software Engineering, University of Szeged
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


/**
  * @file Main program of binary merge.
  *       The main capability of the tool is to merge already existing SoDA binaries.
  */

#include <iostream>
#include "data/CCoverageMatrix.h"
#include "data/CResultsMatrix.h"
#include "boost/program_options.hpp"

using namespace std;
using namespace soda;
using namespace boost::program_options;

#define INFO(X) cout << "[INFO] " << X << endl
#define WARN(X) cout << "[WARNING] " << X << endl
#define ERRO(X) cerr << "[*ERROR*] " << X << endl

StringVector coveragePaths;
StringVector resultPaths;
vector<CCoverageMatrix*> coverageMatrices;
vector<CResultsMatrix*> resultsMatrices;

template<class T>
void loadMatrices(const StringVector& paths, vector<T*>& matrices)
{
    matrices.resize(paths.size());
    for (size_t i = 0; i < paths.size(); ++i) {
        cout << "[INFO] Loading data from path: " << paths[i] << " ... ";
        matrices[i] = new T();
        matrices[i]->load(paths[i]);
        cout << "done." << endl;
    }
}

CCoverageMatrix* mergeCoverage()
{
    auto merged = new CCoverageMatrix();
    IndexType sum_ce = 0, sum_tc = 0;
    for (auto cov : coverageMatrices) {
        auto num_ce = cov->getNumOfCodeElements();
        auto num_tc = cov->getNumOfTestcases();
        INFO("Merging " << num_ce << " code element(s) and " << num_tc << " testcase(s).");
        auto ces = cov->getCodeElements().getValueList();
        for (auto ce : ces) {
            merged->addCodeElementName(ce);
        }
        auto tcs = cov->getTestcases().getValueList();
        for (auto tc : tcs) {
            merged->addTestcaseName(tc);
        }
        sum_ce += num_ce;
        sum_tc += num_tc;
    }
    merged->refitMatrixSize();
    auto c = merged->getNumOfCodeElements();
    auto t = merged->getNumOfTestcases();
    INFO("Dimensions :: merged: " << c << "×" << t << " sum: " << sum_ce << "×" << sum_tc << " (code-elements×testcases).");
    for (auto cov : coverageMatrices) {
        auto ces = cov->getCodeElements().getValueList();
        auto tcs = cov->getTestcases().getValueList();
        for (auto ce : ces) {
            for (auto tc : tcs) {
                merged->setRelation(tc, ce, cov->getRelation(tc, ce));
            }
        }
    }
    return merged;
}

CResultsMatrix* mergeResults()
{
    auto merged = new CResultsMatrix();
    IndexType sum_rev = 0, sum_tc = 0;
    for (auto res : resultsMatrices) {
        auto num_rev = res->getNumOfRevisions();
        auto num_tc = res->getNumOfTestcases();
        INFO("Merging " << num_rev << " revision(s) and " << num_tc << " testcase(s).");
        auto revs = res->getRevisionNumbers();
        for (auto rev : revs) {
            merged->addRevisionNumber(rev);
        }
        auto tcs = res->getTestcases().getValueList();
        for (auto tc : tcs) {
            merged->addTestcaseName(tc);
        }
        sum_rev += num_rev;
        sum_tc += num_tc;
    }
    merged->refitMatrixSize();
    auto r = merged->getNumOfRevisions();
    auto t = merged->getNumOfTestcases();
    INFO("Dimensions :: merged: " << r << "×" << t << " sum: " << sum_rev << "×" << sum_tc << " (revisions×testcases).");
    for (auto res : resultsMatrices) {
        auto revs = res->getRevisionNumbers();
        auto tcs = res->getTestcases().getValueList();
        for (auto rev : revs) {
            for (auto tc : tcs) {
                merged->setResult(rev, tc, res->getResult(rev, tc));
            }
        }
    }
    return merged;
}

/**
 * @brief Process arguments.
 ** @param desc The parameter,value pairs.
 * @param ac The number of arguments.
 * @param av The arguments.
 * @return
 */
int processArgs(options_description desc, int ac, char* av[]);

int main(int argc, char *argv[])
{
    cout << "binaryMerge (SoDA tool)" << endl;
    options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("coverage,c", value<StringVector>(&coveragePaths)->multitoken(), "The path to one or more coverage binaries")
        ("save-coverage,C", value<String>(), "The path where the filtered coverage binary file will be stored")
        ("results,r", value<StringVector>(&resultPaths)->multitoken(), "The path to one or more results binaries")
        ("save-results,R", value<String>(), "The path where the filtered results binary file will be stored")
        ;

    if (argc < 2) {
        ERRO("There are no arguments!" << endl << desc);
        return 1;
    }
    return processArgs(desc, argc, argv);
}

int processArgs(options_description desc, int ac, char* av[])
{
    try {
        positional_options_description p;
        variables_map vm;
        store(command_line_parser(ac, av).options(desc).positional(p).run(), vm);
        notify(vm);

        /*
         * OPTIONS
         */
        if (vm.count("help")) {
            cout << desc << endl;
            return 0;
        }

        /*
         * LOAD DATA
         */
        if (vm.count("coverage")) {
            loadMatrices<CCoverageMatrix>(coveragePaths, coverageMatrices);
        }

        if (vm.count("results")) {
            loadMatrices<CResultsMatrix>(resultPaths, resultsMatrices);
        }

        /*
         * SAVE DATA
         */
        if (vm.count("save-coverage")) {
            String covPath = vm["save-coverage"].as<String>();
            auto cov = mergeCoverage();
            cov->save(covPath);
            delete cov;
        }
        if (vm.count("save-results")) {
            String resPath = vm["save-results"].as<String>();
            auto res = mergeResults();
            res->save(resPath);
            delete res;
        }

        /*
         * CLEAN UP
         */
        for (auto cov : coverageMatrices)
            delete cov;
        for (auto res : resultsMatrices)
            delete res;
    }
    catch (exception& e) {
        ERRO(e.what());
        return 1;
    }
    catch (...) {
        ERRO("Exception of unknown type occured while processing command line arguments!");
        return 1;
    }
    return 0;
}

