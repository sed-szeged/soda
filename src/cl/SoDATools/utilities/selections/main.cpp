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

#define BOOST_FILESYSTEM_VERSION 3

#include <iostream>

#include "data/SoDALibDefs.h"
#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"

using namespace soda;
using namespace std;
using namespace boost::program_options;
using namespace boost::filesystem;

int processArgs(int ac, char* av[]);

options_description desc("Allowed options");

int main(int argc, char* argv[]) {
    cout << "selections (SoDA tool)" << endl;
  //USEAGE
    desc.add_options()
        ("help,h", "produce help message")
        ("coverage-data,c",            value<String>(), "input file containing coverage matrix")
        ("changes-data,x",             value<String>(), "input file containing change matrix")
        ("results-data,r",             value<String>(), "input file containing test execution results")
    ;

    if(argc < 2) {
        cerr << "ERROR: There is no arguments!" << endl << desc << endl;
        return 1;
    }

    return processArgs(argc,argv);
}

int processArgs(int ac, char *av[])
{
    return 0;
}
