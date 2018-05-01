#include <vector>
#include <string>
#include "data/CIDManager.h"
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
            vector<vector<IndexType>>* m_edges;
            
        public:
        
            CGraph(IndexType initialNodeCount);
            ~CGraph();
            
            virtual IndexType nodeCount();
            virtual IndexType edgeCount();
            virtual void clear();
            virtual IndexType addNode(const String& n);
            virtual void addEdge(const IndexType i, IndexType j);
            virtual vector<IndexType>& getEdges(const IndexType& i);
            virtual vector<IndexType>& getDFS(const IndexType& i);
            virtual vector<IndexType>& getBFS(const IndexType& i);
    };
}

#endif
