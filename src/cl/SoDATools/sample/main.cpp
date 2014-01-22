/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Authors: David Tengeri <dtengeri@inf.u-szeged.hu>
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

#include <iostream>
#include "boost/program_options.hpp"
#include "CPrioritizationGeneralIgnore.h"

using namespace soda;

namespace po = boost::program_options;

po::options_description desc("Allowed options");

int processArgs(int ac, char *av[])
{
    po::positional_options_description p;
    po::variables_map vm;
    po::store(po::command_line_parser(ac, av).options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 0;
    }

    // Process arguments.
    if (!vm.count("coverage-data")) {
        std::cout << "Missing options!" << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }

    // Load the binary.
    CCoverageMatrix *matrix = new CCoverageMatrix();
    matrix->load(vm["coverage-data"].as<String>());

    // Prioritize test cases and select the desired amount.
    IndexType selectionSize = matrix->getNumOfTestcases();
    if (vm.count("selection-size")) {
        selectionSize = vm["selection-size"].as<IndexType>();
    }
    IntVector result;
    CPrioritizationGeneralIgnore algorithm(matrix);
    algorithm.fillSelection(result, selectionSize);

    for (IndexType i = 0; i < selectionSize; i++) {
        IndexType tcid = result[i];
        std::cout << matrix->getTestcases().getValue(tcid) << std::endl;
    }

    delete matrix;
}

int main(int argc, char *argv[])
{
    // Usage
    desc.add_options()
            ("help,h", "help message")
            ("coverage-data,c", po::value<String>(), "input file containing the coverage matrix")
            ("selection-size,s", po::value<IndexType>(), "number of tests to select")
    ;

    if (argc < 2) {
        std::cerr << "ERROR: There are no arguments!" << std::endl << desc << std::endl;
        return 1;
    }

    return processArgs(argc, argv);
}
