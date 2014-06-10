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
#include <sstream>
#include "util/CAddressResolver.h"


namespace soda {

std::string CAddressResolver::resolve(const std::string &binaryFullPath, const int address)
    {
        std::ostringstream command;
        command << "addr2line -C -f -p -e " << binaryFullPath << " 0x" << std::hex << address;

        FILE *pf;
        pf = popen(command.str().c_str(), "r");
        if (!pf) {
            throw CException("soda::CAddressResolver::resolve", "Can not run addr2line");
        }
        String output = "";
        char *buf = new char[3072];
        if (fgets(buf, 3072, pf) != NULL) {
            output = String(buf);
        }

        delete buf;
        pclose(pf);

        return output;
    }

} /* namespace soda */
