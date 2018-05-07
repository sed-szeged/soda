#include "data/CTree.h"
#include "algorithm/CDFS.h"
#include "algorithm/CBFS.h"

using namespace std;
using namespace soda::io;

namespace soda
{
    CTree::CTree():
        m_codeElements(new CIDManager()),
        m_edges(new vector<vector<IndexType>>()),
        m_mapping(new vector<IndexType>())
    {}

    CTree::~CTree()
    {
        delete m_codeElements;
        m_codeElements = NULL;
        delete m_edges;
        m_edges = NULL;
        delete m_mapping;
        m_mapping = NULL;
    }

    void CTree::clear()
    {
        m_codeElements->clear();
        m_edges->clear();
        m_mapping->clear();
    }

    IndexType CTree::nodeCount()
    {
        return m_mapping->size();
    }

    IndexType CTree::edgeCount()
    {
        IndexType count = 0;

        for(vector<vector<IndexType>>::iterator it = m_edges->begin(); it != m_edges->end(); it++) {
            {
                count+= it->size();
            }
        }

        return count;
    }

    vector<IndexType>& CTree::getEdges(const IndexType& i)
    {
        return m_edges->at(i);
    }

    IndexType CTree::addNode(const IndexType i)
    {
        m_mapping->push_back(i);
        m_edges->push_back(vector<IndexType>());

        return m_mapping->size()-1;
    }

    IndexType CTree::addNode(const String& n)
    {
        m_codeElements->add(n);

        IndexType i = m_codeElements->getID(n);
        m_mapping->push_back(i);
        m_edges->push_back(vector<IndexType>());

        return m_mapping->size()-1;
    }

    IndexType CTree::addChild(const IndexType parentId, const String& n)
    {
        IndexType newNodeId = addNode(n);
        m_edges->at(parentId).push_back(newNodeId);
        m_edges->at(newNodeId).push_back(parentId);

        return newNodeId;
    }

    String CTree::getNodeValue(const IndexType nodeIndex)
    {
        IndexType codeElementId = m_mapping->at(nodeIndex);

        return m_codeElements->getValue(codeElementId);
    }

    bool hasCommonNode(soda::IntVector* treeA, soda::IntVector* treeB)
    {
        for(soda::IntVector::iterator pi = treeA->begin(); pi != treeA->end(); pi++)
        {
            for(soda::IntVector::iterator ci = treeB->begin(); ci != treeB->end(); ci++)
            {
                if(*ci == *pi)
                {
                    return true;
                }
            }
        }

        return false;
    }

    void CTree::addEdge(const IndexType parentId, const IndexType childId)
    {
        bool standaloneChild = m_edges->at(childId).size() == 0;
        bool independentTrees = false;

        if(!standaloneChild)
        {
            auto parentTreeNodes = CBFS(*m_edges).getBFS(parentId);
            auto childTreeNodes = CBFS(*m_edges).getBFS(childId);

            independentTrees = !hasCommonNode(parentTreeNodes, childTreeNodes);

            delete parentTreeNodes;
            delete childTreeNodes;
        }

        if(standaloneChild || independentTrees)
        {
            auto edges = m_edges->at(parentId);

            if(std::find(edges.begin(), edges.end(), childId) == edges.end())
            {
                m_edges->at(parentId).push_back(childId);
                m_edges->at(childId).push_back(parentId);
            }
        }
        else
        {
            throw logic_error("These points can not be linked because they form a circle.");
        }
    }

    vector<IndexType>* CTree::getDFS(IndexType i)
    {
        return CDFS(*m_edges).getDFS(i);
    }

    vector<IndexType>* CTree::getBFS(IndexType i)
    {
        return CBFS(*m_edges).getBFS(i);
    }

    void CTree::save(io::CBinaryIO *out) const
    {
        //IDManager
        m_codeElements->save(out, io::CSoDAio::IDMANAGER);

        //Tree
        out->writeUInt4(io::CSoDAio::TREE);

        IndexType length = sizeof(IndexType);

        for(vector<vector<IndexType>>::const_iterator rit = m_edges->begin() ; rit != m_edges->end(); ++rit )
        {
            length += sizeof(IndexType);

            for(vector<IndexType>::const_iterator it = rit->begin() ; it != rit->end(); ++it )
            {
                length += sizeof(IndexType);
            }
        }
        out->writeULongLong8(length);

        //Count of nodes
        IndexType size = m_edges->size();
        out->writeULongLong8(size);

        for(vector<vector<IndexType>>::const_iterator rit = m_edges->begin() ; rit != m_edges->end(); ++rit )
        {
            IndexType edgeCount = rit->size();
            out->writeULongLong8(edgeCount);

            for(vector<IndexType>::const_iterator it = rit->begin() ; it != rit->end(); ++it )
            {
                out->writeULongLong8(*it);
            }
        }
    }

    void CTree::load(io::CSoDAio *in)
    {
        m_codeElements->clear();
        m_edges->clear();

        while(in->nextChunkID()) {
            auto chunkId = in->getChunkID();

            if(chunkId == io::CSoDAio::IDMANAGER) {
                m_codeElements->load(in);
            }

            if(chunkId == io::CSoDAio::TREE){
                IndexType nodeCount = in->readULongLong8();

                vector<vector<IndexType>> nodes;

                for (IndexType e = 0; e < nodeCount; ++e) {
                    IndexType edgeCount = in->readULongLong8();

                    vector<IndexType> edges;

                    for (IndexType i = 0; i < edgeCount; ++i) {
                        IndexType edge = in->readULongLong8();
                        edges.push_back(edge);
                    }

                    m_edges->push_back(edges);
                }
            }
        }
    }

    void CTree::save(const char * filename) const
    {
        io::CSoDAio *out = new io::CSoDAio(filename, io::CBinaryIO::omWrite);
        save(out);
        delete out;
    }

    void CTree::load(const char * filename)
    {
        io::CSoDAio *in = new io::CSoDAio(filename, io::CBinaryIO::omRead);
        load(in);
        delete in;
    }

    void CTree::loadJson(const String& path)
    {
        CJsonReader *reader  = new CJsonReader(path);

        //Read IDManager elements
        vector<CJsonReader> properties = reader->getPropertyVectorFromProperty("tree");
        for(IndexType i = 0; i <  properties.size(); i++)
        {
            String nodeName = properties[i].getStringFromProperty("node");
            vector<String> edges = properties[i].getStringVectorFromProperty("edges");
            
            IndexType base = this->addNode(nodeName);

            for(StringVector::iterator i = edges.begin(); i< edges.end(); i++)
            {
                IndexType to = this->addNode(*i);
                
                this->addEdge(base, to);
            }
        }

        delete reader;
        reader = 0;
    }

}
