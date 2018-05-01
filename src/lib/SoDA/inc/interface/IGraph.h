#include <vector>
#include <string>
#include "data/CIDManager.h"
#include "data/CBitMatrix.h"

#ifndef IGRAPH_H
#define IGRAPH_H

using namespace std;

namespace soda
{
    class IGraph
    {
        public:
            virtual IndexType nodeCount() = 0;
            virtual IndexType edgeCount() = 0;
            virtual void clear() = 0;
            virtual IndexType addNode(const String& n) = 0;
            virtual void addEdge(const IndexType i, IndexType j) = 0;
            virtual vector<IndexType>& getEdges(const IndexType& i) = 0;
            virtual vector<IndexType>& getDFS(const IndexType& i) = 0;
            virtual vector<IndexType>& getBFS(const IndexType& i) = 0;
    };
}

#endif
