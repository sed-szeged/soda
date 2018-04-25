#include <vector>
#include <string>
#include <algorithm> 
#include <iostream>
#include "data/CIDManager.h"
#include "data/CBitMatrix.h"
#include "data/CGraph.h"
#include "data/CBitMatrix.h"
#include "interface/IBitList.h"
#include "interface/IIterators.h"

using namespace std;

namespace soda
{
    CGraph::CGraph(IndexType initialNodeCount):
        m_codeElements(new CIDManager()),
        m_edges(new CBitMatrix(initialNodeCount, initialNodeCount))
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

        for(IBitMatrixIterator& it = m_edges->begin(); it != m_edges->end(); it++) {
            if(*it)
            {
                count++;
            }
        }

        return count;
    }

    void CGraph::clear()
    {
        m_codeElements->clear();
        m_edges->clear();
    }            
    
    void CGraph::addNode(const String& n)
    {
        m_codeElements->add(n);
    }

    void CGraph::addEdge(const IndexType i, const IndexType j)
    {
        if(m_edges->getNumOfCols() > i && m_edges->getNumOfRows() > j)
        {
            m_edges->set(i, j, true);
        }
        else
        {
            IndexType newSize = max(i, j);
            m_edges->resize(newSize, newSize);
        }
    }

    void CGraph::addEdge(const String& n1, const String& n2)
    {
        IndexType i = m_codeElements->getID(n1);
        IndexType j = m_codeElements->getID(n2);

        this->addEdge(i, j);
    }
}