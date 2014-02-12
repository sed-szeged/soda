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
#include "OneRevisionPerFileChangesetReaderPlugin.h"

namespace soda {

OneRevisionPerFileChangesetReaderPlugin::OneRevisionPerFileChangesetReaderPlugin()
{

}

OneRevisionPerFileChangesetReaderPlugin::~OneRevisionPerFileChangesetReaderPlugin()
{

}

std::string OneRevisionPerFileChangesetReaderPlugin::getName()
{
    return "one-revision-per-file";
}

std::string OneRevisionPerFileChangesetReaderPlugin::getDescription()
{
    return "The change information is in one file for a revision and the format is: TODO.";
}

CChangeset* OneRevisionPerFileChangesetReaderPlugin::read(const std::string &path)
{
    CChangeset *changeset = new CChangeset();
    std::cout << "Reading " << path << std::endl;
    return NULL;
}

extern "C" void registerPlugin(CRawDataReaderPluginManager &manager) {
    manager.addChangesetReaderPlugin(new OneRevisionPerFileChangesetReaderPlugin());
}

} /* namespace soda */
