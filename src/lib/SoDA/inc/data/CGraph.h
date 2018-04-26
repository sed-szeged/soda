#include <vector>
#include <string>
#include "data/CIDManager.h"
#include "data/CBitMatrix.h"
#include "interface/IGraph.h"

#ifndef CGRAPH_H
#define CGRAPH_H

using namespace std;

namespace soda
{
    class CGraph : public IGraph 
    {
        private:
            IIDManager* m_codeElements;
            CBitMatrix* m_edges;
        public:
            CGraph(IndexType initialNodeCount);
            ~CGraph();
            
            virtual IndexType nodeCount();
            virtual IndexType edgeCount();
            virtual void clear();
            virtual void addNode(const String& n);
            virtual void addEdge(const IndexType i, IndexType j);
            virtual void addEdge(const String& n1, const String& n2);
            virtual void removeEdge(const String& n1, const String& n2);
            virtual void removeEdge(const IndexType i, IndexType j);
            virtual IBitList& getEdges(const String& n);
            virtual IBitList& getEdges(const IndexType& i);

            //Iteration, iterators
            //DFS (depth-first search) 
            //BFS (breath-first search)
            
    };
}

#endif
