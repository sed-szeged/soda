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
        IndexType size = m_edges->getNumOfCols();

        if(max(i,j) >= size)
        {
            IndexType newSize = max(i, j);
            m_edges->resize(newSize, newSize);
        }
     
        m_edges->set(min(i,j), max(i,j), true);    
    }

    void CGraph::addEdge(const String& n1, const String& n2)
    {
        IndexType i = m_codeElements->getID(n1);
        IndexType j = m_codeElements->getID(n2);

        this->addEdge(min(i,j), max(i,j));
    }

    IBitList& CGraph::getEdges(const String& n)
    {
        IndexType row = m_codeElements->getID(n);

        return m_edges->getRow(row);
    }

    IBitList& CGraph::getEdges(const IndexType& i)
    {
        return m_edges->getRow(i);
    }

    void CGraph::removeEdge(const IndexType i, IndexType j)
    {
        m_edges->set(min(i,j), max(i,j), false);
    }

    void CGraph::removeEdge(const String& n1, const String& n2)
    {
        IndexType i = m_codeElements->getID(n1);
        IndexType j = m_codeElements->getID(n2);

        this->removeEdge(i, j);
    }
}