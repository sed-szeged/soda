#include "algorithm/CBFS.h"

namespace soda 
{
    CBFS::CBFS(vector<vector<IndexType>>& edges) : 
        m_edges(&edges),
        m_bfsOrder(new vector<IndexType>()) { };

    CBFS::~CBFS(){ }

    vector<IndexType>& CBFS::getBFS(const IndexType& root)
    {
        IndexType edgeCount = m_edges->size();
        list<IndexType> queue;
        bool *visited = new bool[edgeCount];

        for(int i = 0; i < edgeCount; i++)
            visited[i] = false;
    
        visited[root] = true;
        queue.push_back(root);
            
        while(!queue.empty())
        {
            IndexType s = queue.front();
            m_bfsOrder->push_back(s);
            queue.pop_front();
    
            for (std::vector<IndexType>::iterator i = m_edges->at(s).begin(); i != m_edges->at(s).end(); ++i)
            {
                if (!visited[*i])
                {
                    visited[*i] = true;
                    queue.push_back(*i);
                }
            }
        }

        delete visited;
        
        return *m_bfsOrder;
    }
}