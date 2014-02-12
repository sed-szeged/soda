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
#include "DejaGNUOneRevisionPerFileResultsReaderPlugin.h"

namespace soda {

DejaGNUOneRevisionPerFileResultsReaderPlugin::DejaGNUOneRevisionPerFileResultsReaderPlugin()
{

}

DejaGNUOneRevisionPerFileResultsReaderPlugin::~DejaGNUOneRevisionPerFileResultsReaderPlugin()
{

}

std::string DejaGNUOneRevisionPerFileResultsReaderPlugin::getName()
{
    return "dejagnu-one-revision-per-file";
}

std::string DejaGNUOneRevisionPerFileResultsReaderPlugin::getDescription()
{
    return "The results information of a revision in one file and the format is: TODO.";
}

CResultsMatrix* DejaGNUOneRevisionPerFileResultsReaderPlugin::read(const std::string &path)
{
    CResultsMatrix *results = new CResultsMatrix();
    std::cout << "Reading " << path << std::endl;
    return results;
}

extern "C" void registerPlugin(CRawDataReaderPluginManager &manager) {
    manager.addResultsReaderPlugin(new DejaGNUOneRevisionPerFileResultsReaderPlugin());
}

} /* namespace soda */
