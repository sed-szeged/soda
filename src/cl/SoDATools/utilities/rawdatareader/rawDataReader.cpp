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
  * @file Main program of raw data reader.
  *         The raw data reader program converts coverage, results, changes raw datas to binary files.
  */

#include "datamanager/CDataHandler.h"
#include "datamanager/CCoverageDataManager.h"
#include "datamanager/CChangesDataManager.h"
#include "datamanager/CResultsDataManager.h"
#include "boost/program_options.hpp"

using namespace soda;
using namespace sodatools;
using namespace boost::program_options;

/**
 * @brief Process arguments.
 * @param desc The parameter,value pairs.
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

        if (vm.count("OneTestPerFile")) {
            if (handler->getReadFormat() == rfUnknown) {
                handler->setReadFormat(rfOneTestPerFile);
            } else {
                WARN("discarding option --OneTestPerFile.");
            }
        }

        if (vm.count("coverage-is-without-results")) {
            handler->setWithPassFail(false);
        }

        /*
         * READ DATA
         */
        if (vm.count("read-coverage")) {
            String tmp = vm["read-coverage"].as<String>();
            handler->getCoverageDataMgr().read(tmp);
        }

        if (vm.count("read-results")) {
            String tmp = vm["read-results"].as<String>();
            handler->getResultsDataMgr().read(tmp);
        }

        if (vm.count("read-changes")) {
            String tmp = vm["read-changes"].as<String>();
            handler->getChangesDataMgr().read(tmp);
        }

        /*
         * SAVE DATA
         */
        if (vm.count("save-coverage")) {
            String tmp = vm["save-coverage"].as<String>();
            handler->getCoverageDataMgr().save(tmp);
        }

        if (vm.count("save-results")) {
            String tmp = vm["save-results"].as<String>();
            handler->getResultsDataMgr().save(tmp);
        }

        if (vm.count("save-changes")) {
            String tmp = vm["save-changes"].as<String>();
            handler->getChangesDataMgr().save(tmp);
        }
    } catch(exception& e) {
        ERRO(e.what());
        return 1;
    } catch(...) {
        ERRO("Exception of unknown type while processsing command line arguments!");
    }

    if (handler != NULL) delete handler;
}

int main(int argc, char *argv[])
{
    cout << "rawDataReader (SoDA tool)" << endl;
    options_description desc("Allowed options");
    desc.add_options()
        ("help,h",                                           "produce help message")
        ("OneTestPerFile,O",                                 "read OneTestPerFile formats")
        ("coverage-is-without-results",                      "pass/fail info is not encoded in filenames")
        ("read-coverage",                   value<String>(), "input directory")
        ("save-coverage",                   value<String>(), "output file")
        ("read-results",                    value<String>(), "input directory")
        ("save-results",                    value<String>(), "output file")
        ("read-changes",                    value<String>(), "input directory")
        ("save-changes",                    value<String>(), "output file")
        ("quiet,q", "silent mode")
    ;

    if(argc < 2) {
        ERRO("There are no arguments!" << endl << desc);
        return 1;
    }
    return processArgs(desc, argc, argv);
}
