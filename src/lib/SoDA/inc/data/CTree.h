#include <vector>
#include <string>
#include "data/CIDManager.h"
#include "io/CJsonReader.h"
#include "interface/ITree.h"

#ifndef CTREE_H
#define CTREE_H

using namespace std;

namespace soda
{
    class CTree : public ITree
    {
        private:
            IIDManager* m_codeElements;         // Id <-> string : mapping
            vector<vector<IndexType>>* m_edges; // index=nodeId <-> [nodeId] : enumeration
            vector<IndexType>* m_mapping;       // index=nodeId <-> codeElement.ID : mapping
            IndexType* m_root;

        public:
            CTree();
            ~CTree();

            virtual IndexType nodeCount();
            virtual IndexType edgeCount();
            virtual void clear();
            virtual IndexType addNode(const String& n);
            virtual IndexType addNode(const IndexType i);
            virtual IndexType addChild(const IndexType parentId, const String& n);
            virtual void addEdge(const IndexType parentId, const IndexType childId);
            virtual String getNodeValue(const IndexType nodeIndex);
            virtual vector<IndexType>& getEdges(const IndexType& i);
            virtual vector<IndexType> *getDFS(IndexType i);
            virtual vector<IndexType> *getBFS(IndexType i);
            virtual void save(io::CBinaryIO *out) const;
            virtual void load(io::CSoDAio *in);
            virtual void save(const char * filename) const;
            virtual void load(const char * filename);
            virtual void loadJson(const String& path);
    };
}

#endif
