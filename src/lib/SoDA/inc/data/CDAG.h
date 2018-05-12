#include <vector>
#include <string>
#include "data/CIDManager.h"
#include "data/CGraph.h"
#include "io/CJsonReader.h"
#include "interface/IDAG.h"

#ifndef CDAG_H
#define CDAG_H

using namespace std;

namespace soda
{
    class CDAG : public IDAG
    {
        protected:
            IIDManager* m_codeElements;
            vector<vector<IndexType>>* m_structure;
            vector<Node*>* m_nodes;

        public:
            CDAG();
            ~CDAG();

            io::CSoDAio::ChunkID m_chunkId = io::CSoDAio::ChunkID::DAG;

            virtual IndexType nodeCount();
            virtual IndexType edgeCount();
            virtual void clear();
            virtual Node* addNode(const String& n);
            virtual Node* addNode(const IndexType i);
            virtual Node* addChild(const IndexType parentId, const String& n);
            virtual bool isValid();
            virtual void addEdge(const IndexType parentId, const IndexType childId);
            virtual void addEdge(Node* parent, Node* child);
            virtual String getNodeValue(Node* node);
            virtual String getValue(IndexType elementId);
            virtual vector<IndexType>& getEdges(const IndexType& i);
            virtual vector<IndexType> *getDFS(IndexType i);
            virtual vector<IndexType> *getBFS(IndexType i);
            virtual vector<list<IndexType>*> *convertToChains();
            virtual void save(io::CBinaryIO *out) const;
            virtual void load(io::CSoDAio *in);
            virtual void save(const char * filename) const;
            virtual void load(const char * filename);
            virtual void loadJson(const String& path);
            virtual CGraph* toGraph();
    };
}

#endif
