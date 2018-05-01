#include "data/SoDALibDefs.h"

#ifndef CDFS_H
#define CDFS_H

using namespace std;

namespace soda
{
    class CDFS
    {
        private:
            vector<vector<IndexType>>* m_edges;
            vector<IndexType>* m_dfsOrder;
            void DFS(int v, bool visited[]);

        public:
            vector<IndexType>& getDFS(const IndexType& root);
        
            CDFS(vector<vector<IndexType>>& m_edges);
            ~CDFS();
    };
}

#endif
