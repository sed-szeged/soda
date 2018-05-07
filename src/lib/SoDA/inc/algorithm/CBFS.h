#include "data/SoDALibDefs.h"

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

        public:
            vector<IndexType>* getBFS(const IndexType& root);

            CBFS(vector<vector<IndexType>>& m_edges);
            ~CBFS();
    };
}

#endif
