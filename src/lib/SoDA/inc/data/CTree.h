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
#include "data/CDAG.h"

#ifndef CTREE_H
#define CTREE_H

using namespace std;

namespace soda
{
    /**
     * @brief Tree datastructure class
     */
    class CTree : public CDAG
    {
        public:

            /**
             * @brief Constructor
             */
            CTree();

            /**
             * @brief Destructor
             */
            ~CTree();

            /**
             * @brief ChunkID for export / import functionality
             */
            const io::CSoDAio::ChunkID m_chunkId = io::CSoDAio::ChunkID::TREE;

            /**
             * @brief Adds an exsisting element as a new node to a parent
             * @param elemenId The new value of the element
             * @return the newly created Node
             */
            virtual Node* addChild(const IndexType parentNodeId, const String& n) override;

            /**
             * @brief Checks the DAG contains circle
             * @param root The startin point of the check 
             * @return the answer
             */
            virtual bool isValid(IndexType root) override;

            /**
             * @brief Converts a Tree to DAG
             */
            virtual CDAG* toDAG();
    };
}

#endif
