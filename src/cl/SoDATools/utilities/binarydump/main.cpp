/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
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
  * @file Main program of binary dump.
  *       The binary dump program dumps the specified data from the specified data file.
  */

#include "CDataHandler.h"
#include "CCoverageDataManager.h"
#include "CResultsDataManager.h"
#include "CChangesDataManager.h"
#include "boost/program_options.hpp"

using namespace soda;
using namespace boost::program_options;

/**
 * @brief Process arguments.
 ** @param desc The parameter,value pairs.
 * @param ac The number of arguments.
 * @param av The arguments.
 * @return
 */
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

        if (vm.count("quiet")) {
            handler->setPrintInfo(false);
        }

        if (vm.count("with-names")) {
            handler->setWithNames(true);
        }

        if (vm.count("revision")) {
            handler->setRevision(vm["revision"].as<IndexType>());
        }

        if (vm.count("revision-timestamp")) {
            handler->setRevisionTimestamp(vm["revision-timestamp"].as<time_t>());
        }
        if (vm.count("selection-data")) {
            if (handler->createSelection()) {
                INFO(handler->getPrintInfo(), "Working with selection data");
            }
            else {
                WARN("Can't create selection data");
            }
        }

        /*
         * LOAD DATA
         */
        if (vm.count("load-coverage")) {
            String tmp = vm["load-coverage"].as<String>();
            handler->getCoverageDataMgr().load(tmp);
        }

        if (vm.count("load-results")) {
            String tmp = vm["load-results"].as<String>();
            handler->getResultsDataMgr().load(tmp);
        }

        if (vm.count("load-changes")) {
            String tmp = vm["load-changes"].as<String>();
            handler->getChangesDataMgr().load(tmp);
        }

        if (vm.count("load-bugs")) {
            String tmp = vm["load-bugs"].as<String>();
            handler->getSelection()->loadBugs(tmp);
        }

        /*
        * DUMP DATA
        */
        if (vm.count("dump-coverage-data")) {
            String tmp = vm["dump-coverage-data"].as<String>();
            handler->getCoverageDataMgr().dumpData(tmp);
        }
        if (vm.count("dump-coverage-testcases")) {
            String tmp = vm["dump-coverage-testcases"].as<String>();
            handler->getCoverageDataMgr().dumpTestcases(tmp);
        }
        if (vm.count("dump-coverage-code-elements")) {
            String tmp = vm["dump-coverage-code-elements"].as<String>();
            handler->getCoverageDataMgr().dumpCodeElements(tmp);
        }

        if (vm.count("dump-coverage-image")) {
            String tmp = vm["dump-coverage-image"].as<String>();
            int scale = 1;
            if (vm.count("image-scale")) {
                scale = (vm["image-scale"].as<int>());
            }
            handler->getCoverageDataMgr().dumpImage(tmp, scale);
        }

        if (vm.count("dump-code-element-coverage")) {
            String tmp = vm["dump-code-element-coverage"].as<String>();
            handler->getCoverageDataMgr().dumpCodeElementCoverage(tmp);
        }

        if (vm.count("dump-test-coverage")) {
            String tmp = vm["dump-test-coverage"].as<String>();
            handler->getCoverageDataMgr().dumpTestCoverage(tmp);
        }

        if (vm.count("dump-results-testcases")) {
            String tmp = vm["dump-results-testcases"].as<String>();
            handler->getResultsDataMgr().dumpTestcases(tmp);
        }
        if (vm.count("dump-results-revisions")) {
            String tmp = vm["dump-results-revisions"].as<String>();
            handler->getResultsDataMgr().dumpRevisions(tmp);
        }
        if (vm.count("dump-results-execution")) {
            String tmp = vm["dump-results-execution"].as<String>();
            handler->getResultsDataMgr().dumpExecution(tmp);
        }
        if (vm.count("dump-results-passfail")) {
            String tmp = vm["dump-results-passfail"].as<String>();
            handler->getResultsDataMgr().dumpPassFail(tmp);
        }
        if (vm.count("dump-results-timeline")) {
            String tmp = vm["dump-results-timeline"].as<String>();
            handler->getResultsDataMgr().dumpTimeline(tmp);
        }

        if (vm.count("dump-changes-code-elements")) {
            String tmp = vm["dump-changes-code-elements"].as<String>();
            handler->getChangesDataMgr().dumpCodeElements(tmp);
        }

        if (vm.count("dump-changes")) {
            String tmp = vm["dump-changes"].as<String>();
            handler->getChangesDataMgr().dumpChanges(tmp);
        }
    }
    catch (exception& e) {
        ERRO(e.what());
        return 1;
    }
    catch (...) {
        ERRO("Exception of unknown type while processsing command line arguments!");
    }

    delete handler;
    return 0;
}

int main(int argc, char *argv[])
{
    cout << "binaryDump (SoDA tool)" << endl;
    options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("selection-data,S", "create and work with selection data")
        ("load-coverage,c", value<String>(), "input file")
        ("dump-coverage-data", value<String>(), "output file")
        ("dump-coverage-testcases", value<String>(), "output file")
        ("dump-coverage-code-elements", value<String>(), "output file")
        ("dump-code-element-coverage", value<String>(), "output file")
        ("dump-test-coverage", value<String>(), "output file")
        ("dump-coverage-image", value<String>(), "output file")
        ("image-scale,i", value<int>(), "image scale")
        ("load-results,r", value<String>(), "input file")
        ("dump-results-testcases", value<String>(), "output file")
        ("dump-results-revisions", value<String>(), "output file")
        ("dump-results-execution", value<String>(), "output file")
        ("dump-results-passfail", value<String>(), "output file")
        ("dump-results-timeline", value<String>(), "output file")
        ("load-changes,x", value<String>(), "input file")
        ("dump-changes-code-elements", value<String>(), "output file")
        ("dump-changes", value<String>(), "output file")
        ("load-bugs,b", value<String>(), "input file")
        ("revision", value<IndexType>(), "revision number")
        ("revision-timestamp", value<time_t>(), "revision timestamp TODO: remove when timestamps are merged with revision number")
        ("with-names,w", "dump coverage data,results data or changeset data with names")
        ("quiet,q", "silent mode")
        ;

    if (argc < 2) {
        ERRO("There are no arguments!" << endl << desc);
        return 1;
    }
    return processArgs(desc, argc, argv);
}
