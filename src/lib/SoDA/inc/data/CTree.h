#include <vector>
#include <string>
#include "data/CIDManager.h"
#include "interface/ITree.h"
#include "io/CJsonReader.h"
#include "data/CGraph.h"

#ifndef CTREE_H
#define CTREE_H

using namespace std;

namespace soda
{
    class CTree : CGraph
    {
        private:
            IIDManager* m_codeElements;
            vector<vector<IndexType>>* m_edges;
            IndexType* m_root;

        public:
            CTree();
            ~CTree();

            virtual void addEdge(const IndexType i, const IndexType j) override;
            virtual io::CSoDAio::ChunkID getChunkId() override;
            virtual vector<IndexType>& getDFS();
            virtual vector<IndexType>& getBFS();
    };
}

#endif
