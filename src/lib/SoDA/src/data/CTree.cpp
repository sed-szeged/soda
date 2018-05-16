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
        m_codeElements->clear();
        m_structure->clear();

        for(IndexType i = 0; i < m_nodes->size(); i++)
        {
            delete m_nodes->at(i);
            m_nodes->at(i) = NULL;
        }

        m_nodes->clear();
    }

    Node* CTree::addChild(const IndexType parentNodeId, const String& n)
    {
        Node* newNode = addNode(n);
        m_structure->at(parentNodeId).push_back(newNode->m_id);

        return newNode;
    }

    bool CTree::isValid(IndexType root)
    {
        return !CBFS(*m_structure).isDegreeGTOne(root);
    }

    CDAG* CTree::toDAG()
    {
        return (CDAG*)this;
    }
}
