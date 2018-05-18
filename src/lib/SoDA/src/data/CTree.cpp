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

#include "data/CTree.h"
#include "algorithm/CDFS.h"
#include "algorithm/CBFS.h"

using namespace std;
using namespace soda::io;

namespace soda
{
    CTree::CTree(): CDAG()
    {

    }

    CTree::~CTree()
    {
        m_codeElements->clear();
        m_structure->clear();

        for(IndexType i = 0; i < m_nodes->size(); i++)
        {
            delete m_nodes->at(i);
            m_nodes->at(i) = NULL;
        }

        m_nodes->clear();
    }

    Node* CTree::addChild(const IndexType parentNodeId, const String& n)
    {
        Node* newNode = addNode(n);
        m_structure->at(parentNodeId).push_back(newNode->m_id);

        return newNode;
    }

    bool CTree::isValid(IndexType root)
    {
        return !CBFS(*m_structure).inDegreeGTOne(root);
    }

    CDAG* CTree::toDAG()
    {
        return (CDAG*)this;
    }
}
