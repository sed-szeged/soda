#include "data/CDAG.h"
#include "data/Node.h"
#include "algorithm/CDFS.h"
#include "algorithm/CBFS.h"

using namespace std;
using namespace soda::io;

namespace soda
{
    CDAG::CDAG():
        m_codeElements(new CIDManager()),
        m_structure(new vector<vector<IndexType>>()),
        m_nodes(new vector<Node*>())
    {}

    CDAG::~CDAG()
    {
        delete m_codeElements;
        m_codeElements = NULL;
        delete m_structure;
        m_structure = NULL;

        for(IndexType i = 0; i < m_nodes->size(); i++)
        {
            delete m_nodes->at(i);
            m_nodes->at(i) = NULL;
        }

        delete m_nodes;
        m_nodes = NULL;
    }

    void CDAG::clear()
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

    IndexType CDAG::nodeCount()
    {
        return m_nodes->size();
    }

    IndexType CDAG::edgeCount()
    {
        IndexType count = 0;

        for(vector<vector<IndexType>>::iterator it = m_structure->begin(); it != m_structure->end(); it++) 
        {
            count+= it->size();
        }

        return count;
    }

    vector<IndexType>& CDAG::getEdges(const IndexType& i)
    {
        return m_structure->at(i);
    }

    void CDAG::addEdge(Node* parent, Node* child)
    {
        addEdge(parent->m_id, child->m_id);
    }

    Node* CDAG::addNode(const IndexType i)
    {
        IndexType newIndex =  m_nodes->size() == 0 ? 0 : m_nodes->back()->m_id+1;
        Node* n = new Node(newIndex, i);

        m_nodes->push_back(n);
        m_structure->push_back(vector<IndexType>());

        return n;
    }

    Node* CDAG::addNode(const String& n)
    {
        m_codeElements->add(n);

        IndexType i = m_codeElements->getID(n);

        return addNode(i);
    }

    Node* CDAG::addChild(const IndexType parentId, const String& n)
    {
        Node* newNode = addNode(n);
        m_structure->at(parentId).push_back(newNode->m_id);

        return newNode;
    }

    String CDAG::getNodeValue(Node* node)
    {
        return m_codeElements->getValue(node->m_elementId);
    }

    String CDAG::getValue(IndexType elementId)
    {
        return m_codeElements->getValue(elementId);
    }

    bool CDAG::isValid()
    {
        return !CDFS(*m_structure).hasCycle();
    }

    void CDAG::addEdge(const IndexType parentId, const IndexType childId)
    {
        bool exsistingEdge = std::find(m_structure->at(childId).begin(), m_structure->at(childId).end(), parentId) != m_structure->at(childId).end();
        if(exsistingEdge)
        {
            return;
        }

        m_structure->at(parentId).push_back(childId);

        bool isValid = this->isValid();

        if (!isValid)
        {
            m_structure->at(parentId).pop_back();
            throw logic_error("This edge cannot be applied. It creates a circle.");
        }
    }

    vector<IndexType>* CDAG::getDFS(IndexType i)
    {
        return CDFS(*m_structure).getDFS(i);
    }

    vector<IndexType>* CDAG::getBFS(IndexType i)
    {
        return CBFS(*m_structure).getBFS(i);
    }

    vector<list<IndexType>*> *CDAG::convertToChains()
    {
        return CBFS(*m_structure).getPaths(0);
    }

    void CDAG::save(io::CBinaryIO *out) const
    {
        //IDManager
        m_codeElements->save(out, io::CSoDAio::IDMANAGER);

        //Tree
        out->writeUInt4(m_chunkId);

        IndexType length = sizeof(IndexType);

        for(vector<vector<IndexType>>::const_iterator rit = m_structure->begin() ; rit != m_structure->end(); ++rit )
        {
            length += sizeof(IndexType);

            for(vector<IndexType>::const_iterator it = rit->begin() ; it != rit->end(); ++it )
            {
                length += sizeof(IndexType);
            }
        }

        length += m_nodes->size() * sizeof(Node);

        out->writeULongLong8(length);

        //Count of nodes
        IndexType nodesCount = m_nodes->size();
        out->writeULongLong8(nodesCount);

        //Nodes
        for(vector<Node*>::const_iterator it = m_nodes->begin() ; it != m_nodes->end(); ++it )
        {
            out->writeULongLong8((*it)->m_id);
            out->writeULongLong8((*it)->m_elementId);
        }

        //Edges
        for(vector<vector<IndexType>>::const_iterator rit = m_structure->begin() ; rit != m_structure->end(); ++rit )
        {
            IndexType edgeCount = rit->size();
            out->writeULongLong8(edgeCount);

            for(vector<IndexType>::const_iterator it = rit->begin() ; it != rit->end(); ++it )
            {
                out->writeULongLong8(*it);
            }
        }
    }

    void CDAG::load(io::CSoDAio *in)
    {
        m_codeElements->clear();
        m_structure->clear();
        m_nodes->clear();

        while(in->nextChunkID()) {
            auto chunkId = in->getChunkID();

            if(chunkId == io::CSoDAio::IDMANAGER) 
            {
                m_codeElements->load(in);
            }

            if(m_chunkId == chunkId){
                IndexType nodeCount = in->readULongLong8();

                for(IndexType i = 0; i < nodeCount; ++i)
                {
                    IndexType mId = in->readULongLong8();
                    IndexType mElementId = in->readULongLong8();

                    m_nodes->push_back(new Node(mId, mElementId));
                }

                for (IndexType e = 0; e < nodeCount; ++e) 
                {
                    IndexType edgeCount = in->readULongLong8();

                    vector<IndexType> edges;
                    
                    for (IndexType i = 0; i < edgeCount; ++i) 
                    {
                        IndexType edge = in->readULongLong8();
                        
                        edges.push_back(edge);
                    }

                    m_structure->push_back(edges);
                }
            }
        }
    }

    void CDAG::save(const char * filename) const
    {
        io::CSoDAio *out = new io::CSoDAio(filename, io::CBinaryIO::omWrite);
        save(out);
        delete out;
    }

    void CDAG::load(const char * filename)
    {
        io::CSoDAio *in = new io::CSoDAio(filename, io::CBinaryIO::omRead);
        load(in);
        delete in;
    }

    void CDAG::loadJson(const String& path)
    {
        CJsonReader *reader  = new CJsonReader(path);

        //Read IDManager elements
        vector<CJsonReader> nodes = reader->getPropertyVectorFromProperty("nodes"); 
        vector<CJsonReader> edges = reader->getPropertyVectorFromProperty("edges"); 

        for(IndexType i = 0; i <  nodes.size(); i++)
        {
            IndexType nodeId = nodes[i].getIntFromProperty("id");
            String value = nodes[i].getStringFromProperty("value");

            this->addNode(value);
        }

        for(IndexType i = 0; i <  edges.size(); i++)
        {
            IndexType nodeId = edges[i].getIntFromProperty("nodeId");
            vector<IndexType> values = edges[i].getIntVectorFromProperty("values");

            for(vector<IndexType>::iterator v = values.begin(); v < values.end(); v++)
            {
                this->addEdge(nodeId, *v);
            }
        }

        delete reader;
        reader = 0;
    }

    CGraph* CDAG::toGraph()
    {
        CGraph* graph = new CGraph();

        for(IndexType c = 0; c < m_codeElements->size(); c++)
        {
            graph->addNode(m_codeElements->getValue(c));
        }

        for(IndexType i = 0; i < m_nodes->size();  i++)
        {
            IndexType nodeElement = m_nodes->at(i)->m_elementId;
            vector<IndexType>* neighbours = &(m_structure->at(i));

            for(IndexType j = 0; j < neighbours->size();  j++)
            {
                IndexType neighbourElement = m_nodes->at(neighbours->at(j))->m_elementId;
                graph->addEdge(nodeElement, neighbourElement);
            }
        }

        return graph;
    }
}
