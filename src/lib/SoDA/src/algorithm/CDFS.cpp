#include "algorithm/CDFS.h"

namespace soda {

    CDFS::CDFS(vector<vector<IndexType>>& edges) :
        m_edges(&edges),
        m_dfsOrder(new vector<IndexType>()) { };

    CDFS::~CDFS(){ }

    void CDFS::DFS(int v, bool visited[])
    {
        visited[v] = true;

        m_dfsOrder->push_back(v);

        for (std::vector<IndexType>::iterator i = m_edges->at(v).begin(); i != m_edges->at(v).end(); ++i)
            if (!visited[*i])
                DFS(*i, visited);
    }

    vector<IndexType>& CDFS::getDFS(const IndexType& root)
    {
        IndexType edgeCount = m_edges->size();
        bool *visited = new bool[edgeCount];

        for (int i = 0; i < edgeCount; i++)
        {
            visited[i] = false;
        }

        DFS(root, visited);

        delete visited;

        return *m_dfsOrder;
    }
}
