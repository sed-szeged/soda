#include <vector>
#include <string>
#include "data/CTree.h"

#ifndef CDAG_H
#define CDAG_H

using namespace std;

namespace soda
{
    class CDAG : public CTree
    {
        public:
            CDAG();
            ~CDAG();

            const io::CSoDAio::ChunkID m_chunkId = io::CSoDAio::ChunkID::DAG;

            virtual Node* addChild(const IndexType parentId, const String& n) override;
            virtual bool isValid() override;
            virtual void addEdge(Node* parent, Node* child);
    };
}

#endif
