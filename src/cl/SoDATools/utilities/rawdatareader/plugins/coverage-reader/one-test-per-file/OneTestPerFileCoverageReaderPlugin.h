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

#ifndef ONETESTPERFILECOVERAGEREADERPLUGIN_H
#define ONETESTPERFILECOVERAGEREADERPLUGIN_H

#include "../ICoverageReaderPlugin.h"

namespace soda {

class OneTestPerFileCoverageReaderPlugin : public ICoverageReaderPlugin
{
public:

    OneTestPerFileCoverageReaderPlugin();
    ~OneTestPerFileCoverageReaderPlugin();

    std::string getName();
    std::string getDescription();

    CCoverageMatrix* read(const std::string &path);
};

}

#endif /* ONETESTPERFILECOVERAGEREADERPLUGIN_H */
