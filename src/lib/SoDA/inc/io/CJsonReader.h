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

#ifndef CJSONREADER_H
#define CJSONREADER_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "data/SoDALibDefs.h"

using namespace boost::property_tree;

namespace soda { namespace io {

/**
 * @brief The CJsonReader class implements methods for opening and reading json type files.
 */
class CJsonReader
{
public:

    /**
     * @brief Creates a new CJsonReader object.
     * @param path File path.
     */
    CJsonReader(ptree tr);
    CJsonReader(String path);
    ~CJsonReader();

    /**
     * @brief Loads a json file and parses it.
     * @param path File path.
     */
    void load(String path);

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

    /**
     * @brief Returns the value of the specified property from property tree as String.
     * @param property Specified path in property tree.
     * @return Value of the specified property.
     */
    String getStringFromProperty(String property);

    /**
     * @brief Returns the value of the specified property from property tree as bool.
     * @param property Specified path in property tree.
     * @return Value of the specified property.
     */
    bool getBoolFromProperty(String property);

    /**
     * @brief Returns the value of the specified property from property tree as int.
     * @param property Specified path in property tree.
     * @return Value of the specified property.
     */
    int getIntFromProperty(String property);

    /**
     * @brief Returns the values of the specified property from property tree as IntVector.
     * @param property Specified path in property tree.
     * @return Value of the specified property.
     */
    IntVector getIntVectorFromProperty(String property);

    /**
     * @brief Returns the value of the specified property from property tree as StringVector.
     * @param property Specified path in property tree.
     * @return Value of the specified property.
     */
    StringVector getStringVectorFromProperty(String property);

    /**
     * @brief Returns property trees from the specified property as CJsonReader vector.
     * @param property Specified path in property tree.
     * @return Property trees from the specified property.
     */
    std::vector<CJsonReader> getPropertyVectorFromProperty(String property);

private:

    /**
     * @brief Property tree which stores json file data.
     */
    ptree m_data;
};

} /* namespace io */

} /* namespace soda */

#endif /* CJSONREADER_H */
