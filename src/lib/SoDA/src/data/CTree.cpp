#include "data/CTree.h"
#include "algorithm/CDFS.h"
#include "algorithm/CBFS.h"

using namespace std;
using namespace soda::io;

namespace soda
{
    CTree::CTree():
        m_codeElements(new CIDManager()),
        m_edges(new vector<vector<IndexType>>())
    {}

    CTree::~CTree()
    {
        delete m_codeElements;
        m_codeElements = NULL;
        delete m_edges;
        m_edges = NULL;
    }

    io::CSoDAio::ChunkID CTree::getChunkId()
    {
        return io::CSoDAio::TREE;
    }

    void CTree::addEdge(const IndexType i, const IndexType j)
    {
        auto edges = m_edges->at(i);

        if(std::find(edges.begin(), edges.end(), j) == edges.end())
        {
            m_edges->at(i).push_back(j);
        }
    }

    vector<IndexType>& CTree::getDFS()
    {
        return CGraph::getDFS(*m_root);
    }

    vector<IndexType>& CTree::getBFS()
    {
        return CGraph::getBFS(*m_root);
    }
}
