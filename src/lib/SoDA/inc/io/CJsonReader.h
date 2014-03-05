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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "data/SoDALibDefs.h"

using namespace boost::property_tree;

namespace soda { namespace io {

/**
 * @brief The CJsonReader class contains methods for opening and reading json type files.
 */
class CJsonReader
{
public:

    /**
     * @brief Creates a new CJsonReader object.
     * @param path File path.
     */
    CJsonReader(String path);
    ~CJsonReader();

    /**
     * @brief Opens a new json file and parses it.
     * @param path File path.
     */
    void open(String path);

    /**
     * @brief Clears the property tree.
     */
    void clear();

    /**
     * @brief Returns true if the specified property exists in the property tree.
     * @param property Specified property.
     * @return True if the specified property exists in the property tree.
     */
    bool existsProperty(String property);

    String getStringFromProperty(String property);
    bool getBoolFromProperty(String property);
    int getIntFromProperty(String property, int def = 0);
    IntVector getIntVectorFromProperty(String property);
    StringVector getStringVectorFromProperty(String property);
private:
    /**
     * @brief Property tree stores json file data.
     */
    ptree m_data;
};

} /* namespace io */

} /* namespace soda */
