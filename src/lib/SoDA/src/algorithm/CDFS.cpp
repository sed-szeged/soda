#include <bits/stdc++.h>
#include "algorithm/CDFS.h"

namespace soda 
{
    CDFS::CDFS(vector<vector<IndexType>>& edges) :
        m_edges(&edges),
        m_dfsOrder(new vector<IndexType>()) { };

    CDFS::~CDFS(){ }

    void CDFS::recursiveHelper(IndexType current, bool visited[])
    {
        visited[current] = true;

        m_dfsOrder->push_back(current);

        for (std::vector<IndexType>::iterator i = m_edges->at(current).begin(); i != m_edges->at(current).end(); ++i)
        {
            if (!visited[*i])
            {
                recursiveHelper(*i, visited);
            }
        }
    }

    vector<IndexType> *CDFS::getDFS(IndexType root)
    {
        IndexType edgeCount = m_edges->size();
        bool *visited = new bool[edgeCount];

        for (IndexType i = 0; i < edgeCount; i++)
        {
            visited[i] = false;
        }

        recursiveHelper(root, visited);

        delete visited;

        return m_dfsOrder;
    }

    bool CDFS::hasCycleHelper(IndexType v, bool visited[], bool *recorded)
    {
        if(visited[v] == false)
        {
            visited[v] = true;
            recorded[v] = true;

            for(vector<IndexType>::iterator i = m_edges->at(v).begin(); i != m_edges->at(v).end(); ++i)
            {
                if (!visited[*i] && hasCycleHelper(*i, visited, recorded))
                {
                    return true;
                }
                else if (recorded[*i])
                {
                    return true;
                }
            }
        }

        recorded[v] = false;

        return false;
    }

    bool CDFS::hasCycle()
    {
        IndexType size = m_edges->size();

        bool *visited = new bool[size];
        bool *recorded = new bool[size];

        for(IndexType i = 0; i < size; i++)
        {
            visited[i] = false;
            recorded[i] = false;
        }

        for(IndexType i = 0; i < size; i++)
        {
            if (hasCycleHelper(i, visited, recorded))
            {
                return true;
            }
        }

        delete visited;
        delete recorded;

        return false;
    }
}
