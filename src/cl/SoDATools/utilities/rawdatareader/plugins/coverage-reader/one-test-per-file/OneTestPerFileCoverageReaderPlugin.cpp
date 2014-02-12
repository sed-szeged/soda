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

#include "CRawDataReaderPluginManager.h"
#include "OneTestPerFileCoverageReaderPlugin.h"

namespace soda {

OneTestPerFileCoverageReaderPlugin::OneTestPerFileCoverageReaderPlugin()
{

}

OneTestPerFileCoverageReaderPlugin::~OneTestPerFileCoverageReaderPlugin()
{

}

std::string OneTestPerFileCoverageReaderPlugin::getName()
{
    return "one-test-per-file";
}

std::string OneTestPerFileCoverageReaderPlugin::getDescription()
{
    return "The coverage information of a test case is in one file and the file name is the test case name.";
}

CCoverageMatrix* OneTestPerFileCoverageReaderPlugin::read(const std::string &path)
{
    CCoverageMatrix *matrix = new CCoverageMatrix();
    std::cout << "Reading " << path << std::endl;
    return matrix;
}

extern "C" void registerPlugin(CRawDataReaderPluginManager &manager) {
    manager.addCoverageReaderPlugin(new OneTestPerFileCoverageReaderPlugin());
}

} /* namespace soda */
