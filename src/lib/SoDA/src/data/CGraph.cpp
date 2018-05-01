#include "data/CGraph.h"
#include "algorithm/CDFS.h"
#include "algorithm/CBFS.h"

using namespace std;

namespace soda
{
    CGraph::CGraph(IndexType initialNodeCount):
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
}