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

#include "algorithm/CBFS.h"

namespace soda
{
    CBFS::CBFS(vector<vector<IndexType>>& edges) :
        m_edges(&edges),
        m_bfsOrder(new vector<IndexType>()) { };

    CBFS::~CBFS(){ }

    vector<IndexType> *CBFS::getBFS(IndexType root)
    {
        IndexType edgeCount = m_edges->size();
        list<IndexType> queue;
        bool *visited = new bool[edgeCount];

        for(int i = 0; i < edgeCount; i++)
            visited[i] = false;

        visited[root] = true;
        queue.push_back(root);

        while(!queue.empty())
        {
            IndexType s = queue.front();
            m_bfsOrder->push_back(s);
            queue.pop_front();

            for (std::vector<IndexType>::iterator i = m_edges->at(s).begin(); i != m_edges->at(s).end(); ++i)
            {
                if (!visited[*i])
                {
                    visited[*i] = true;
                    queue.push_back(*i);
                }
            }
        }

        delete visited;

        return m_bfsOrder;
    }

    bool CBFS::inDegreeGTOne(IndexType root)
    {
        //Is it possible to touch all of the nodes one-time only
        IndexType nodeCount = m_edges->size();
        list<IndexType> queue;
        bool *visited = new bool[nodeCount];

        //Initialize bool[] with false values
        for(int i = 0; i < nodeCount; i++)
            visited[i] = false;

        //Starting with the Root node
        visited[root] = true;
        queue.push_back(root);

        while(!queue.empty())
        {
            IndexType s = queue.front();
            queue.pop_front();

            //Iterating edges to deepre-level nodes
            for (std::vector<IndexType>::iterator i = m_edges->at(s).begin(); i != m_edges->at(s).end(); ++i)
            {
                if (!visited[*i])
                {
                    //Feeding the queue with untouched nodes
                    visited[*i] = true;
                    queue.push_back(*i);
                }
                else
                {
                    return true;
                }
            }
        }

        return false;
    }

    list<IndexType>* CBFS::scrollBackSingleChain(vector<Node*>* chainElements, Node* endNode, IndexType root)
    {
        list<IndexType>* chain = new list<IndexType>();

        chain->push_front(endNode->m_elementId);
        Node* n = chainElements->at(endNode->m_id);

        while(n->m_elementId != root)
        {
            chain->push_front(n->m_elementId);
            n = chainElements->at(n->m_id);
        }

        chain->push_front(n->m_elementId);

        return chain;
    }

    vector<list<IndexType>*>* CBFS::getPaths(IndexType rootElementId)
    {
        auto chains = new vector<list<IndexType>*>();

        vector<Node*>* chainElements = new vector<Node*>();
        list<IndexType> currentLevel;
        IndexType index = 0;

        Node* rootNode = new Node(index,rootElementId);

        chainElements->push_back(rootNode);
        currentLevel.push_back(0);

        while(!currentLevel.empty())
        {
            IndexType parentIndex = currentLevel.front();
            currentLevel.pop_front();

            for (std::vector<IndexType>::iterator i = m_edges->at(chainElements->at(parentIndex)->m_elementId).begin(); i != m_edges->at(chainElements->at(parentIndex)->m_elementId).end(); ++i)
            {
                Node* c = new Node(parentIndex, *i);
                chainElements->push_back(c);
                if(m_edges->at(*i).size() == 0)
                {
                    chains->push_back(scrollBackSingleChain(chainElements, c, rootElementId));
                }

                currentLevel.push_back(++index);
            }
        }

        for(vector<Node*>::iterator i = chainElements->begin(); i < chainElements->end(); i++)
        {
            delete *i;
            *i = NULL;
        }
        
        delete chainElements;

        return chains;
    }
}
