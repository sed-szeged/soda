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

#ifndef IBUGSETREADERPLUGIN_H
#define IBUGSETREADERPLUGIN_H

#include "data/CBugset.h"

namespace soda {

    /**
    * @brief Interface of the changeset reader plugins.
    */
    class IBugsetReaderPlugin
    {
    public:

        /**
        * @brief Virtual destructor.
        */
        virtual ~IBugsetReaderPlugin() {}

        /**
        * @brief Returns the name of the plugin.
        * @return
        */
        virtual std::string getName() = 0;

        /**
        * @brief Returns the description of the plugin.
        * @return
        */
        virtual std::string getDescription() = 0;

        /**
        * @brief Reads a changeset from the specified path.
        * @param path The path to the changeset.
        * @return A CChangeset instance.
        */
        virtual CBugset* read(const std::string &path) = 0;
    };

} /* namespace soda */

#endif /* IBUGSETREADERPLUGIN_H */
