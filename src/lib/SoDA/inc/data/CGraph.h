#include <vector>
#include <string>
#include "data/CIDManager.h"
#include "data/CBitMatrix.h"

#ifndef CGRAPH_H
#define CGRAPH_H

using namespace std;

namespace soda
{
    class CGraph{

        private:
            IIDManager* m_codeElements;
            CBitMatrix* m_edges;
        public:
            CGraph(IndexType initialNodeCount);
            ~CGraph();
            
            IndexType nodeCount();
            IndexType edgeCount();
            
            void clear();

            void addNode(const String& n);
            void addEdge(const IndexType i, IndexType j);
            void addEdge(const String& n1, const String& n2);
            // 
            
            
    };
}

#endif
