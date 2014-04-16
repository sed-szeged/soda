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

#include <fstream>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "io/CJsonReader.h"

namespace soda { namespace io {

CJsonReader::CJsonReader(ptree tr) :
    m_data(tr)
{}

CJsonReader::CJsonReader(String path)
{
    load(path);
}

CJsonReader::~CJsonReader()
{}

void CJsonReader::load(String path)
{
    m_data.clear();
    try {
        read_json(path, m_data);
    } catch (json_parser_error &e) {
        throw;
    }
}

void CJsonReader::clear()
{
    m_data.clear();
}

bool CJsonReader::existsProperty(String property)
{
    return (m_data.count(property) > 0);
}

String CJsonReader::getStringFromProperty(String property)
{
    return  m_data.get<String>(property, "");
}

bool CJsonReader::getBoolFromProperty(String property)
{
    return m_data.get<bool>(property, false);
}

int CJsonReader::getIntFromProperty(String property)
{
    return m_data.get<int>(property, 0);
}

IntVector CJsonReader::getIntVectorFromProperty(String property)
{
    IntVector tmp;
    BOOST_FOREACH(ptree::value_type &v, m_data.get_child(property))
        tmp.push_back(boost::lexical_cast<int>(v.second.data()));

    return tmp;
}

StringVector CJsonReader::getStringVectorFromProperty(String property)
{
    StringVector tmp;
    BOOST_FOREACH(ptree::value_type &v, m_data.get_child(property))
        tmp.push_back(v.second.data());

    return tmp;
}

std::vector<CJsonReader> CJsonReader::getPropertyVectorFromProperty(String property)
{
    std::vector<CJsonReader> tmp;
    BOOST_FOREACH(ptree::value_type &v, m_data.get_child(property))
        tmp.push_back(CJsonReader(v.second));

    return tmp;
}

} /* namespace io */

} /* namespace soda */
