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

#include "data/SoDALibDefs.h"
#include "data/CBitMatrix.h"

#ifndef CDFS_H
#define CDFS_H

using namespace std;

namespace soda
{
    /**
     * @brief The CDFS class stores the Depth First Search related algorithms
     */
    class CDFS
    {
        private:
            /**
             * @brief Vector of vectors of edges
             */
            vector<vector<IndexType>>* m_edges;
             
            /**
            * @brief An ordering of edges
            */
            vector<IndexType>* m_dfsOrder;
            
            /**
             * @brief Recursive helper. Executes visiting an a subgraph started with current item
             * @param v The current root element
             * @param visited The helper container for the visited edges
             */
            void recursiveHelper(IndexType current, bool *visited);

            /**
             * @brief Recursive helper. Executes a search for a circle on graph started by current, helped by visited nodes and recoderer routes.
             * @param current The current root element
             * @param visited A helper container for the visited nodes
             * @param recorded A helper container for the building-up routes
             */
            bool hasCycleHelper(IndexType current, bool visited[], bool *recorded);

        public:

            /**
             * @brief Constructor of CDFS
             * @param m_edges The edges.
             */
            CDFS(vector<vector<IndexType>> &m_edges);
            ~CDFS();

            /**
             * @brief Gets the DFS ordering of elements.
             * @param root The starting node.
             */
            vector<IndexType> *getDFS(IndexType root);

            /**
             * @brief Gets a logical decision, does graph have circle.
             */
            bool hasCycle();
    };
}

#endif
