/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors:
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

#include <algorithm>
#include <iostream>

#include "boost/filesystem.hpp"
#include "boost/program_options.hpp"
#include "CDataManager.h"
#include "exception/CException.h"


using namespace std;
using namespace soda;
using namespace boost::program_options;

options_description desc("Allowed options");

int processArgs(int ac, char *av[]);

int main(int argc, char *argv[])
{
    cout << "test-selection-statistics (SoDA tool)" << endl;

    // Usage
    desc.add_options()
            ("help,h", "help message")
            ("coverage-data,c", value<String>(), "input file containing coverage matrix")
            ("changes-data,x", value<String>(), "input file containing change matrix")
            ("results-data,r", value<String>(), "input file containing test execution results")
            ("bugs-data,b", value<String>(), "input file containing bug reports")
            ("output-dir,o", value<String>(), "output dir, if specified .dat files will be generated")
            ("testcase-coverage", "computes only the test case coverage related statistics")
            ("function-coverage", "computes only the function related statistics")
            ("changes", "computes ontly the change related statistics")
            ("fails", "computes only the failed test case related statistics")
            ("bugs", "computes only the bug report related statistics")
            ("coverage-result-summary", "computes coverage and result summary")
            ("globalize,g", "globalize selection data")
            ("filter-to-coverage,f", "filter data to coverage")
    ;

    if (argc < 2) {
        cerr << "ERROR: There are no arguments!" << endl << desc << endl;
        return 1;
    }

    return processArgs(argc, argv);
}

int processArgs(int ac, char *av[])
{
    try {
        CDataManager mgr;
        positional_options_description p;
        variables_map vm;

        store(command_line_parser(ac, av).options(desc).positional(p).run(), vm);
        notify(vm);

        if (vm.count("help")) {
            cout << desc << endl;
            return 0;
        }

        // Process arguments.
        if (vm.count("output-dir")) {
            mgr.setOutputDir(vm["output-dir"].as<String>());
        } else {
            cout << "The output-dir parameter is missing!" << endl;
            cout << desc << endl;
            return 1;
        }

        if (vm.count("testcase-coverage"))
            mgr.setTestMask(tmTestcaseCoverage);
        if (vm.count("function-coverage"))
            mgr.setTestMask(tmFunctionCoverage);
        if (vm.count("changes"))
            mgr.setTestMask(tmChanges);
        if (vm.count("fails"))
            mgr.setTestMask(tmFails);
        if (vm.count("coverage-result-summary"))
            mgr.setTestMask(tmCoverageResultSummary);
        if (vm.count("bugs"))
            mgr.setTestMask(tmBugs);

        if (mgr.getTestMask() == tmNone) {
            mgr.setTestMask(tmTestcaseCoverage | tmFunctionCoverage | tmChanges | tmFails | tmCoverageResultSummary | tmBugs);
        }

        bool loadCoverage     = false;
        bool loadChanges      = false;
        bool loadResults      = false;
        bool loadBugs         = false;
        bool globalize        = false;
        bool filterToCoverage = false;

        if ((mgr.getTestMask() & (tmTestcaseCoverage | tmFunctionCoverage | tmCoverageResultSummary)) && vm.count("coverage-data")) {
            loadCoverage = true;
        } else if (mgr.getTestMask() & (tmTestcaseCoverage | tmFunctionCoverage | tmCoverageResultSummary)) {
            cout << "The coverage-data parameter is missing!" << endl;
            cout << desc << endl;
            return 1;
        }

        if ((mgr.getTestMask() & (tmChanges | tmCoverageResultSummary)) && vm.count("changes-data")) {
            loadChanges = true;
        } else if (mgr.getTestMask() & tmChanges) {
            cout << "The changes-data parameter is missing!" << endl;
            cout << desc << endl;
            return 1;
        }

        if ((mgr.getTestMask() & (tmFails | tmCoverageResultSummary)) && vm.count("results-data")) {
            loadResults = true;
        } else if (mgr.getTestMask() & tmFails) {
            cout << "The results-data parameter is missing!" << endl;
            cout << desc << endl;
            return 1;
        }


        if ((mgr.getTestMask() & tmCoverageResultSummary) || vm.count("globalize")) {
            globalize = true;
        }
        if (vm.count("filter-to-coverage")) {
            globalize = true;
            filterToCoverage = true;
        }

        if (loadCoverage && (!boost::filesystem::exists(vm["coverage-data"].as<String>()))) {
            cout << "[ERROR] Invalid coverage-data file path!" << endl;
            return 1;
        }

        if (loadResults && (!boost::filesystem::exists(vm["results-data"].as<String>()))) {
            cout << "[ERROR] Invalid results-data file path!" << endl;
            return 1;
        }

        if (loadChanges && (!boost::filesystem::exists(vm["changes-data"].as<String>()))) {
            cout << "[ERROR] Invalid changes-data file path!" << endl;
            return 1;
        }

        if (loadBugs && (!boost::filesystem::exists(vm["bugs-data"].as<String>()))) {
            cout << "[ERROR] Invalid bugs-data file path!" << endl;
            return 1;
        }

        // Load data.
        if (loadCoverage) {
            mgr.loadCoverageData(vm["coverage-data"].as<String>());
        }
        if (loadChanges) {
            mgr.loadChangesetData(vm["changes-data"].as<String>());
        }
        if (loadResults) {
            mgr.loadResultsData(vm["results-data"].as<String>());
        }
        if (loadBugs) {
            mgr.loadBugReportData(vm["bugs-data"].as<String>());
        }
        if (globalize) {
            mgr.getSelectionData()->globalize();
            if (filterToCoverage) {
                mgr.getSelectionData()->filterToCoverage();
            }
        }

        mgr.calcStatistics();
    } catch(std::exception& e) {
        cerr << "error: " << e.what() << endl;
        return 1;
    } catch(...) {
        cerr << "Exception of unknown type while processsing command line arguments!" << endl;
        return 1;
    }
    return 0;
}
