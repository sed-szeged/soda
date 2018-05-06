#include <vector>
#include <string>
#include "data/CIDManager.h"
#include "interface/IGraph.h"
#include "io/CJsonReader.h"

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

            CGraph();
            ~CGraph();

            virtual IndexType nodeCount();
            virtual IndexType edgeCount();
            virtual void clear();
            virtual IndexType addNode(const String& n);
            virtual void addEdge(const IndexType i, IndexType j);
            virtual vector<IndexType>& getEdges(const IndexType& i);
            virtual vector<IndexType>& getDFS(const IndexType& i);
            virtual vector<IndexType>& getBFS(const IndexType& i);
            virtual void save(io::CBinaryIO *out) const;
            virtual void load(io::CSoDAio *in);
            virtual void save(const char * filename) const;
            virtual void load(const char * filename);
            virtual void loadJson(const String& path);
    };
}

#endif
