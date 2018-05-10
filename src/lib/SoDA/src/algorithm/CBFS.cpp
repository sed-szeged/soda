#include "algorithm/CBFS.h"

namespace soda
{
    CBFS::CBFS(vector<vector<IndexType>>& edges) :
        m_edges(&edges),
        m_bfsOrder(new vector<IndexType>()) { };

    CBFS::~CBFS(){ }

    vector<IndexType> *CBFS::getBFS(IndexType root)
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

        return m_bfsOrder;
    }

    bool CBFS::isValid(IndexType root)
    {
        //Is it possible to touch all of the nodes one-time only
        IndexType nodeCount = m_edges->size();
        list<IndexType> queue;
        bool *visited = new bool[nodeCount];

        //Initialize bool[] with false values
        for(int i = 0; i < nodeCount; i++)
            visited[i] = false;

        //Starting with the Root node
        visited[root] = true;
        queue.push_back(root);

        while(!queue.empty())
        {
            IndexType s = queue.front();
            queue.pop_front();

            //Iterating edges to deepre-level nodes
            for (std::vector<IndexType>::iterator i = m_edges->at(s).begin(); i != m_edges->at(s).end(); ++i)
            {
                if (!visited[*i])
                {
                    //Feeding the queue with untouched nodes
                    visited[*i] = true;
                    queue.push_back(*i);
                }
                else
                {
                    //If the node is already visited meens that has two parents.
                    return false;
                }
            }
        }

        return true;
    }
}
