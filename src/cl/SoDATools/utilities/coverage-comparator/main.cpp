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
  * @file Main program of coverage copmarator.
  *       The main capability of the tool is to compare coverage matrices which were created from multiple test runs of a selected program.
  */

#include <iostream>
#include <set>
#include "data/CCoverageMatrix.h"
#include "boost/program_options.hpp"

using namespace std;
using namespace soda;
using namespace boost::program_options;

StringVector matrixPaths;
vector<CCoverageMatrix*> coverages;

void loadMatrices();
std::set<String> compareMatrices();

int main(int argc, char *argv[]) {
    cout << "coverage-comparator (SoDA tool)" << endl;
    options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("coverage,c", value<StringVector>(&matrixPaths)->required()->multitoken(), "List of input coverage matrices to compare. Assuming that matrix dimensions are equals.")
        ;

    if (argc < 2) {
        cerr << "There are no arguments!" << endl << desc;
        return 1;
    }

    try {
        positional_options_description p;
        variables_map vm;
        store(command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        notify(vm);

        loadMatrices();
        cout << "[INFO] Comparing matrices...";
        auto diff = compareMatrices();
        cout << "done" << endl;
        if (diff.size()) {
            cout << "Different tests:" << endl;
            for (auto &test : diff) {
                cout << "\t" << test << endl;
            }
        }
        cout << endl << "[INFO] Number of diffs: " << diff.size() << endl;

        for (auto cov : coverages) {
            delete cov;
        }
    }
    catch (exception& e) {
        cerr << e.what();
        return 1;
    }
    catch (...) {
        cerr << "Exception of unknown type while processsing command line arguments!";
    }

    return 0;
}

void loadMatrices() {
    coverages.resize(matrixPaths.size());
    for (size_t i = 0; i < matrixPaths.size(); ++i) {
        cout << "[INFO] Loading coverage from path: " << matrixPaths[i] << "...";
        coverages[i] = new CCoverageMatrix();
        coverages[i]->load(matrixPaths[i]);
        cout << "done" << endl;
    }
}

std::set<String> compareMatrices() {
    std::set<String> diff;
    // iterates over code elements and test cases
    for (auto &i : coverages[0]->getTestcases().getValueList()) {
        for (auto &j : coverages[0]->getCodeElements().getValueList()) {
            for (size_t matrixA = 1; matrixA < coverages.size(); ++matrixA) {
                if (coverages[0]->getRelation(i, j) != coverages[matrixA]->getRelation(i, j)) {
                    diff.insert(i);
                }
            }
        }
    }
    return diff;
}
