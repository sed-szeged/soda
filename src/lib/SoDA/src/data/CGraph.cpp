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

#include "data/CGraph.h"
#include "algorithm/CDFS.h"
#include "algorithm/CBFS.h"

using namespace std;
using namespace soda::io;

namespace soda
{
    CGraph::CGraph():
        m_codeElements(new CIDManager()),
        m_edges(new vector<vector<IndexType>>())
    {}

    CGraph::~CGraph()
    {
        delete m_codeElements;
        m_codeElements = NULL;
        delete m_edges;
        m_edges = NULL;
    }

    IndexType CGraph::nodeCount()
    {
        return m_codeElements->size();
    }

    IndexType CGraph::edgeCount()
    {
        IndexType count = 0;

        for(vector<vector<IndexType>>::iterator it = m_edges->begin(); it != m_edges->end(); it++) {
            {
                count+= it->size();
            }
        }

        return count;
    }

    void CGraph::clear()
    {
        m_codeElements->clear();
        m_edges->clear();
    }

    IndexType CGraph::addNode(const String& n)
    {
        //Check exsisting element
        if(m_codeElements->containsValue(n))
        {
            return m_codeElements->getID(n);
        }

        m_codeElements->add(n);
        IndexType i = m_codeElements->getID(n);

        m_edges->push_back(vector<IndexType>());

        return i;
    }

    void CGraph::addEdge(const IndexType i, const IndexType j)
    {
        IndexType ma = max(i,j);

        //Index check
        if(ma >= m_codeElements->size())
        {
            throw range_error("Invalid element index.");
        }

        auto edges = m_edges->at(min(i,j));

        //Checking exsistance
        if(std::find(edges.begin(), edges.end(), ma) == edges.end())
        {
            m_edges->at(i).push_back(j);
            m_edges->at(j).push_back(i);
        }
    }

    vector<IndexType>& CGraph::getEdges(const IndexType& i)
    {
        return m_edges->at(i);
    }

    vector<IndexType> *CGraph::getDFS(const IndexType& i)
    {
        return CDFS(*m_edges).getDFS(i);
    }

    vector<IndexType> *CGraph::getBFS(const IndexType& i)
    {
        return CBFS(*m_edges).getBFS(i);
    }

    void CGraph::save(io::CBinaryIO *out) const
    {
        //IDManager
        m_codeElements->save(out, io::CSoDAio::IDMANAGER);

        //Graph
        out->writeUInt4(io::CSoDAio::GRAPH);

        IndexType length = sizeof(IndexType);

        for(vector<vector<IndexType>>::const_iterator rit = m_edges->begin() ; rit != m_edges->end(); ++rit )
        {
            length += sizeof(IndexType);

            for(vector<IndexType>::const_iterator it = rit->begin() ; it != rit->end(); ++it )
            {
                length += sizeof(IndexType);
            }
        }
        out->writeULongLong8(length);

        //Count of nodes
        IndexType size = m_edges->size();
        out->writeULongLong8(size);

        for(vector<vector<IndexType>>::const_iterator rit = m_edges->begin() ; rit != m_edges->end(); ++rit )
        {
            IndexType edgeCount = rit->size();
            out->writeULongLong8(edgeCount);

            for(vector<IndexType>::const_iterator it = rit->begin() ; it != rit->end(); ++it )
            {
                out->writeULongLong8(*it);
            }
        }
    }

    void CGraph::load(io::CSoDAio *in)
    {
        m_codeElements->clear();
        m_edges->clear();

        while(in->nextChunkID()) {
            auto chunkId = in->getChunkID();

            if(chunkId == io::CSoDAio::IDMANAGER) {
                m_codeElements->load(in);
            }

            if(chunkId == io::CSoDAio::GRAPH){
                IndexType nodeCount = in->readULongLong8();

                for (IndexType e = 0; e < nodeCount; ++e) {
                    IndexType edgeCount = in->readULongLong8();

                    vector<IndexType> edges;

                    for (IndexType i = 0; i < edgeCount; ++i) {
                        IndexType edge = in->readULongLong8();
                        edges.push_back(edge);
                    }

                    m_edges->push_back(edges);
                }
            }
        }
    }

    void CGraph::save(const char * filename) const
    {
        io::CSoDAio *out = new io::CSoDAio(filename, io::CBinaryIO::omWrite);
        save(out);
        delete out;
    }

    void CGraph::load(const char * filename)
    {
        io::CSoDAio *in = new io::CSoDAio(filename, io::CBinaryIO::omRead);
        load(in);
        delete in;
    }

    void CGraph::loadJson(const String& path)
    {
        CJsonReader *reader  = new CJsonReader(path);

        //Read IDManager elements
        vector<CJsonReader> properties = reader->getPropertyVectorFromProperty("graph");
        for(IndexType i = 0; i <  properties.size(); i++)
        {
            String nodeName = properties[i].getStringFromProperty("node");
            vector<String> edges = properties[i].getStringVectorFromProperty("edges");

            IndexType base = this->addNode(nodeName);

            for(StringVector::iterator i = edges.begin(); i< edges.end(); i++)
            {
                IndexType to = this->addNode(*i);

                this->addEdge(base, to);
            }
        }

        delete reader;
        reader = 0;
    }
}
