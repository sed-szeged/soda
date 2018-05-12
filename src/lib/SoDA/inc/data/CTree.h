#include <vector>
#include <string>
#include "data/CDAG.h"

#ifndef CTREE_H
#define CTREE_H

using namespace std;

namespace soda
{
    class CTree : public CDAG
    {
        public:
            CTree();
            ~CTree();

            const io::CSoDAio::ChunkID m_chunkId = io::CSoDAio::ChunkID::TREE;

            virtual Node* addChild(const IndexType parentNodeId, const String& n) override;
            virtual bool isValid(IndexType root) override;
            virtual void addEdge(Node* parent, Node* child);
            virtual CDAG* toDAG();
    };
}

#endif
