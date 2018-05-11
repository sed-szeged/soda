#include "data/CTree.h"
#include "algorithm/CDFS.h"
#include "algorithm/CBFS.h"

using namespace std;
using namespace soda::io;

namespace soda
{
    CTree::CTree(): CDAG()
    {

    }

    CTree::~CTree()
    {

    }

    Node* CTree::addChild(const IndexType parentId, const String& n)
    {
        Node* newNode = addNode(n);
        m_structure->at(parentId).push_back(newNode->m_id);

        return newNode;
    }

    // vector<vector<IndexType>*> *convertToChains()
    // {
    //     auto chains = new vector<vector<IndexType>*>();
    //     //TODO
    //     return chains;
    // }

    bool CTree::isValid()
    {
        return CBFS(*m_structure).isValid(0);
    }

    void CTree::addEdge(Node* parent, Node* child)
    {
        CDAG::addEdge(parent->m_id, child->m_id);
    }
}
