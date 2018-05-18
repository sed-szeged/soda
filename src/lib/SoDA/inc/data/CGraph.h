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
#include "data/CIDManager.h"
#include "interface/IGraph.h"
#include "io/CJsonReader.h"

#ifndef CGRAPH_H
#define CGRAPH_H

using namespace std;

namespace soda
{
    /**
     * @brief Graph datastructure class
     */
    class CGraph : public IGraph
    {
        private:
            
            /**
             * @brief SoDA container of elements
             */
            IIDManager* m_codeElements;

            /**
             * @brief The container of the edges
             */
            vector<vector<IndexType>>* m_edges;

        public:

            /**
             * @brief Constructor
             */
            CGraph();

            /**
             * @brief Destructor
             */
            ~CGraph();

            /**
            * @brief Gets the count of mapping nodes
             * @return count
             */
            virtual IndexType nodeCount();

            /**
             * @brief Gets the count of edges
             * @return count
             */
            virtual IndexType edgeCount();

            /**
             * @brief Clears the containers
             */
            virtual void clear();

            /**
             * @brief Adds a new node
             * @param n The new value of the element 
             * @return the newly created elemntId
             */
            virtual IndexType addNode(const String& n);

            /**
             * @brief Adds an new edge between two elements
             * @param i The exsisting elementId
             * @param j The exsisting elementId
             */
            virtual void addEdge(const IndexType i, IndexType j);

            /**
             * @brief Gets value of the element by nodeId
             * @param nodeId The node 
             * @return the vector of edges
             */
            virtual vector<IndexType>& getEdges(const IndexType& i);

            /**
             * @brief Gets DFS order starting with nodeId
             * @param i The node Id
             * @return the vector of order
             */
            virtual vector<IndexType>* getDFS(const IndexType& i);

            /**
             * @brief Gets BFS order starting with nodeId
             * @param i The node Id
             * @return the vector of order
             */
            virtual vector<IndexType>* getBFS(const IndexType& i);
            
            /**
             * @brief Saves a chain as binary data to the out stream
             * @param [IN] out The stream
             */
            virtual void save(io::CBinaryIO *out) const;

            /**
             * @brief Loads chain by binary data from in stream
             * @param [IN] in The source stream
             */
            virtual void load(io::CSoDAio *in);

            /**
             * @brief Saves the chain to file
             * @param [IN] in The filename
             */
            virtual void save(const char * filename) const;

            /**
             * @brief Loads chain from file
             * @param [IN] filename The filename
             */
            virtual void load(const char * filename);

            /**
             * @brief Loads chain from JSON data file
             * @param [IN] filename The requested id
             */
            virtual void loadJson(const String& path);
    };
}

#endif
