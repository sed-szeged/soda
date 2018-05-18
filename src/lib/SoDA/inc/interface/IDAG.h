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
#include "data/Node.h"

#ifndef IDAG_H
#define IDAG_H

using namespace std;

namespace soda
{
    class IDAG
    {
        public:
            
            /**
             * @brief Gets the count of mapping nodes
             * @return count
             */
            virtual IndexType nodeCount() = 0;

            /**
             * @brief Gets the count of edges
             * @return count
             */
            virtual IndexType edgeCount() = 0;

            /**
             * @brief Clears the containers
             */
            virtual void clear() = 0;

            /**
             * @brief Adds a new node
             * @param n The new value of the element 
             * @return the newly created Node
             */
            virtual Node* addNode(const String& n) = 0;

            /**
             * @brief Adds an exsisting element as a new node
             * @param elemenId The exsisting elementId
             * @return the newly created Node
             */
            virtual Node* addNode(const IndexType i) = 0;

            /**
             * @brief Adds an exsisting element as a new node to a parent
             * @param elemenId The new value of the element
             * @return the newly created Node
             */
            virtual Node* addChild(const IndexType parentNodeId, const String& n) = 0;

            /**
             * @brief Adds a new edge between exsisting nodes
             * @param parentNodeId The parent node 
             * @param childNodeId The child node
             */
            virtual void addEdge(const IndexType parentNodeId, const IndexType childNodeId) = 0;

            /**
             * @brief Adds a new edge between exsisting nodes
             * @param parentNode The parent node 
             * @param childNode The child node
             */
            virtual void addEdge(Node* parent, Node* child) = 0;

            /**
             * @brief Gets value of node
             * @param node The node 
             * @return the value
             */
            virtual bool isValid(IndexType root) = 0;

            /**
             * @brief Gets value of node
             * @param node The node 
             * @return the value
             */
            virtual String getNodeValue(Node* node) = 0;

            /**
             * @brief Gets value of node by nodeId
             * @param nodeId The node 
             * @return the value
             */
            virtual String getNodeValue(IndexType nodeId) = 0;

            /**
             * @brief Gets value of the element by elementId
             * @param elementId The node 
             * @return the value
             */
            virtual String getValue(IndexType elementId) = 0;

            /**
             * @brief Gets value of the element by nodeId
             * @param nodeId The node 
             * @return the vector of edges
             */
            virtual vector<IndexType>& getEdges(const IndexType& i) = 0;

            /**
             * @brief Gets DFS order starting with nodeId
             * @param i The node Id
             * @return the vector of order
             */
            virtual vector<IndexType>* getDFS(IndexType i) = 0;

            /**
             * @brief Gets BFS order starting with nodeId
             * @param i The node Id
             * @return the vector of order
             */
            virtual vector<IndexType>* getBFS(IndexType i) = 0;

            /**
             * @brief Converts a DAG to chains
             * @return the vector of orderlists
             */
            virtual vector<list<IndexType>*>* convertToNodeIdChains() = 0;

            /**
             * @brief Saves a chain as binary data to the out stream
             * @param out The stream
             */
            virtual void save(io::CBinaryIO* out) const = 0;

            /**
             * @brief Loads chain by binary data from in stream
             * @param in The source stream
             */
            virtual void load(io::CSoDAio* in) = 0;

            /**
             * @brief Saves the chain to file
             * @param in The filename
             */
            virtual void save(const char* filename) const = 0;

            /**
             * @brief Loads chain from file
             * @param filename The filename
             */
            virtual void load(const char* filename) = 0;

            /**
             * @brief Loads chain from JSON data file
             * @param filename The requested id
             */
            virtual void loadJson(const String& path) = 0;
    };
}

#endif
