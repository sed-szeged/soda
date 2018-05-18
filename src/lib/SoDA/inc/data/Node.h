/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Author: David Imre Adam <adam.david.imre@gmail.com>
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

#include <vector>
#include <string>
#include "data/SoDALibDefs.h"

#ifndef NODE_H
#define NODE_H

using namespace soda;

namespace soda
{
    /**
     * @brief Node item helper class
     */
    class Node
    {
        public:

        /**
         * @brief Id of node
         */
        IndexType m_id;

        /**
         * @brief Id of the element value
         */
        IndexType m_elementId;

        /**
         * @brief Constructor with default values
         */
        Node(IndexType id, IndexType elementId)
        {
            m_id = id;
            m_elementId = elementId;
        }

        /**
         * @brief Destructor
         */
        ~Node()
        {}
    };
}

#endif
