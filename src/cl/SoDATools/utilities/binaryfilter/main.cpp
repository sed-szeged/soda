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
  * @file Main program of binary filter.
  *       The main capability of the tool is to filter already existing SoDA binaries.
  */

#include "util/CDataHandler.h"
#include "util/CCoverageDataManager.h"
#include "util/CResultsDataManager.h"
#include "boost/program_options.hpp"

using namespace std;
using namespace soda;
using namespace boost::program_options;

#define ERRO(X) cerr << "[*ERROR*] " << X << endl

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
    cout << "binaryFilter (SoDA tool)" << endl;
    options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("load-coverage,c", value<String>(), "The path to a coverage binary input file")
        ("save-coverage", value<String>(), "The path where the filtered coverage binary file will be stored")
        ("load-results,r", value<String>(), "The path to a results binary file")
        ("save-results", value<String>(), "The path where the filtered results binary file will be stored")
        ("filter-code-elements", value<String>(), "Path to a file which contains the name of code elements which should be removed from the binary")
        ("filter-tests", value<String>(), "Path to a file which contains the name of testcases which should be removed from the binary")
        ;

    if (argc < 2) {
        ERRO("There are no arguments!" << endl << desc);
        return 1;
    }
    return processArgs(desc, argc, argv);
}

int processArgs(options_description desc, int ac, char* av[])
{
    CDataHandler *handler = new CDataHandler();

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
        if (vm.count("load-coverage")) {
            String covPath = vm["load-coverage"].as<String>();
            handler->getCoverageDataMgr().load(covPath);
        }

        if (vm.count("load-results")) {
            String resPath = vm["load-results"].as<String>();
            handler->getResultsDataMgr().load(resPath);
        }

        /*
         * FILTER DATA
         */
        if (vm.count("filter-code-elements")) {
            String ceFilterPath = vm["filter-code-elements"].as<String>();
            handler->loadCodeElementFilter(ceFilterPath);
        }

        if (vm.count("filter-tests")) {
            String testFilterPath = vm["filter-tests"].as<String>();
            handler->loadTestcaseFilter(testFilterPath);
        }

        /*
         * SAVE DATA
         */
        if (vm.count("save-coverage")) {
            String covPath = vm["save-coverage"].as<String>();
            auto filteredCov = handler->filterCoverage(handler->getCoverage());
            filteredCov->save(covPath);
        }
        if (vm.count("save-results")) {
            String resPath = vm["save-results"].as<String>();
            auto filteredRes = handler->filterResults(handler->getResults());
            filteredRes->save(resPath);
        }
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

