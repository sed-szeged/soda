#include <vector>
#include <string>
#include "data/SoDALibDefs.h"

#ifndef NODE_H
#define NODE_H

using namespace soda;

namespace soda
{
    class Node
    {
        public:
        IndexType m_id;
        IndexType m_elementId;

        Node(IndexType id, IndexType elementId)
        {
            m_id = id;
            m_elementId = elementId;
        }

        ~Node()
        {}
    };
}

#endif
