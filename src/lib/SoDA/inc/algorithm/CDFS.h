#include "data/SoDALibDefs.h"
#include "data/CBitMatrix.h"

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
            void recursiveHelper(IndexType current, bool *visited);
            bool hasCycleHelper(IndexType current, bool visited[], bool *recorded);

        public:
            CDFS(vector<vector<IndexType>> &m_edges);
            ~CDFS();

            vector<IndexType> *getDFS(IndexType root);
            bool hasCycle();
    };
}

#endif
