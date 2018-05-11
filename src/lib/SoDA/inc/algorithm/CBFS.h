#include "data/SoDALibDefs.h"
#include "data/Node.h"

#ifndef CBFS_H
#define CBFS_H

using namespace std;

namespace soda
{
    class CBFS
    {
        private:
            vector<vector<IndexType>>* m_edges;
            vector<IndexType>* m_bfsOrder;
            void BFS(int v, bool visited[]);
            list<IndexType>* scrollBackSingleChain(vector<Node*>* chainElements, Node* endNode, IndexType root);

        public:
            CBFS(vector<vector<IndexType>>& m_edges);
            ~CBFS();

            vector<IndexType>* getBFS(IndexType root);
            bool isValid(IndexType root);
            vector<list<IndexType>*>* getPaths(IndexType root);
    };
}

#endif
