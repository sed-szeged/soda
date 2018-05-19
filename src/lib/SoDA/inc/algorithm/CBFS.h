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
#include "data/Node.h"

#ifndef CBFS_H
#define CBFS_H

using namespace std;

namespace soda
{
    /**
     * @brief The CBFS class stores the Breadth First Search related algorithms
     */
    class CBFS
    {
        private:
            /**
             * @brief Vector of vectors of edges
             */
            vector<vector<IndexType>>* m_edges;

            /**
             * @brief An ordering of edges
             */
            vector<IndexType>* m_bfsOrder;
            
            /**
             * @brief Creates an ordering of m_edges by BFS algorithm
             * @param v The current root element
             * @param root The helper container for the visited edges
             */
            void BFS(int v, bool visited[]);

            /**
             * @brief Creates the reversed order of chainElements from root with ending with endNode
             * @param root The startpoint
             * @param root The ending Node
             * @param root The starting point
             * @return list of reversed order
             */
            list<IndexType>* scrollBackSingleChain(vector<Node*>* chainElements, Node* endNode, IndexType root);

        public:
            /**
             * @brief Constructor of CBFS
             * @param m_edges The edges.
             */
            CBFS(vector<vector<IndexType>>& m_edges);
            
            /**
             * @brief Default destructor
             */
            ~CBFS();

            /**
             * @brief Creates an ordering of m_edges by BFS algorithm
             * @param root The startpoint
             * @return The BFS order
             */
            vector<IndexType>* getBFS(IndexType root);

            /**
             * @brief Creates a list of paths on the m_edges those contains all of the vertices
             * @param rootElementId The starting elementId of a vertex 
             * @return Vector of paths
             */
            vector<list<IndexType>*>* getPaths(IndexType rootElementId);

            /**
             * @brief Checks any nodes have more thane one in degree
             * @param root The startpoint 
             * @return A boolean answer value 
             */
            bool inDegreeGTOne(IndexType root);
    };
}

#endif
