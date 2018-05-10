#include "data/CDAG.h"
#include "algorithm/CDFS.h"
#include "algorithm/CBFS.h"

using namespace std;
using namespace soda::io;

namespace soda
{
    CDAG::CDAG(): CTree()
    {

    }

    CDAG::~CDAG()
    {

    }

    Node* CDAG::addChild(const IndexType parentId, const String& n)
    {
        Node* newNode = addNode(n);
        m_structure->at(parentId).push_back(newNode->m_id);

        return newNode;
    }

    bool CDAG::isValid()
    {
        return !CDFS(*m_structure).hasCycle();
    }

    void CDAG::addEdge(Node* parent, Node* child)
    {
        CTree::addEdge(parent->m_id, child->m_id);
    }
}
