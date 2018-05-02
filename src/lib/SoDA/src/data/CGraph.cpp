#include "data/CGraph.h"
#include "algorithm/CDFS.h"
#include "algorithm/CBFS.h"

using namespace std;

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
        if(m_codeElements->containsValue(n))
            return m_codeElements->getID(n);

        m_codeElements->add(n);
        IndexType i = m_codeElements->getID(n);

        m_edges->push_back(vector<IndexType>());

        return i;
    }

    void CGraph::addEdge(const IndexType i, const IndexType j)
    {
        m_edges->at(i).push_back(j);
        m_edges->at(j).push_back(i);
    }

    vector<IndexType>& CGraph::getEdges(const IndexType& i)
    {
        return m_edges->at(i);
    }

    vector<IndexType>& CGraph::getDFS(const IndexType& i)
    {
        return CDFS(*m_edges).getDFS(i);
    }

    vector<IndexType>& CGraph::getBFS(const IndexType& i)
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

                vector<vector<IndexType>> nodes;

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
}
