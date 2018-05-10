#include <string>
#include "gtest/gtest.h"
#include "data/CDAG.h"

using namespace soda;

TEST(DAG, BuildDAGByLinkingNodes) {
    CDAG dag = CDAG();
    auto n0 = dag.addNode("A");
    auto n1 = dag.addNode("B");
    auto n2 = dag.addNode("C");
    auto n3 = dag.addNode("D");
    auto n4 = dag.addNode("E");
    auto n5 = dag.addNode("F");
    auto n6 = dag.addNode("G");
    auto n7 = dag.addNode("H");
    auto n8 = dag.addNode("I");
    auto n9 = dag.addNode("J");

    ASSERT_TRUE(dag.edgeCount() == 0);
    ASSERT_TRUE(dag.nodeCount() == 10);

    ASSERT_EQ(dag.getNodeValue(n0), "A");
    ASSERT_EQ(dag.getNodeValue(n1), "B");
    ASSERT_EQ(dag.getNodeValue(n2), "C");
    ASSERT_EQ(dag.getNodeValue(n3), "D");
    ASSERT_EQ(dag.getNodeValue(n4), "E");
    ASSERT_EQ(dag.getNodeValue(n5), "F");
    ASSERT_EQ(dag.getNodeValue(n6), "G");
    ASSERT_EQ(dag.getNodeValue(n7), "H");
    ASSERT_EQ(dag.getNodeValue(n8), "I");
    ASSERT_EQ(dag.getNodeValue(n9), "J");

    dag.addEdge(n0, n1);
    dag.addEdge(n0, n2);
    dag.addEdge(n0, n3);
    dag.addEdge(n1, n4);
    dag.addEdge(n1, n5);
    dag.addEdge(n2, n6);
    dag.addEdge(n3, n7);
    dag.addEdge(n7, n8);
    dag.addEdge(n7, n9);

    ASSERT_TRUE(dag.edgeCount() == 9);
}

TEST(DAG, BuildDAGByLinkingSubDAGs)
{
    CDAG dag = CDAG();
    auto n0 = dag.addNode("A");
    auto n1 = dag.addNode("B");
    auto n2 = dag.addNode("C");
    auto n3 = dag.addNode("D");
    auto n4 = dag.addNode("E");
    auto n5 = dag.addNode("F");
    auto n6 = dag.addNode("G");
    auto n7 = dag.addNode("H");
    auto n8 = dag.addNode("I");
    auto n9 = dag.addNode("J");

    dag.addEdge(n7, n8);
    dag.addEdge(n7, n9);

    dag.addEdge(n1, n4);
    dag.addEdge(n1, n5);

    dag.addEdge(n2, n6);

    dag.addEdge(n3, n7);

    dag.addEdge(n0, n1);
    dag.addEdge(n0, n2);
    dag.addEdge(n0, n3);

    ASSERT_TRUE(dag.edgeCount() == 9);
    ASSERT_TRUE(dag.nodeCount() == 10);
}

TEST(DAG, BuildDAGByLeaves)
{
    CDAG dag = CDAG();
    auto n0 = dag.addNode("A"); //Root
    auto n1 = dag.addChild(n0->m_id, "B");
    auto n2 = dag.addChild(n0->m_id, "C");
    auto n3 = dag.addChild(n0->m_id, "D");
    auto n4 = dag.addChild(n1->m_id, "E");
    auto n5 = dag.addChild(n1->m_id, "F");
    auto n6 = dag.addChild(n2->m_id, "G");
    auto n7 = dag.addChild(n3->m_id, "H");
    auto n8 = dag.addChild(n7->m_id, "I");
    auto n9 = dag.addChild(n7->m_id, "J");

    ASSERT_TRUE(dag.nodeCount() == 10);
    ASSERT_TRUE(dag.edgeCount() == 9);
}

TEST(DAG, FormingCircleBySubDAGs)
{
    CDAG dag = CDAG();
    auto n0 = dag.addNode("A");
    auto n1 = dag.addNode("B");
    auto n2 = dag.addNode("C");
    auto n3 = dag.addNode("D");
    auto n4 = dag.addNode("E");
    
    dag.addEdge(n0, n1);
    dag.addEdge(n1, n2);
    dag.addEdge(n2, n3);
    dag.addEdge(n3, n4);
    
    EXPECT_THROW(dag.addEdge(n4, n0), logic_error); //additional enclosing edge
    EXPECT_THROW(dag.addEdge(n2, n0), logic_error); //additional enclosing edge
}

TEST(DAG, DFS)
{
    CDAG dag = CDAG();
    auto n0 = dag.addNode("A"); //Root
    auto n1 = dag.addChild(n0->m_id, "B");
    auto n2 = dag.addChild(n0->m_id, "C");
    auto n3 = dag.addChild(n0->m_id, "D");
    auto n4 = dag.addChild(n1->m_id, "E");
    auto n5 = dag.addChild(n1->m_id, "F");
    auto n6 = dag.addChild(n2->m_id, "G");
    auto n7 = dag.addChild(n3->m_id, "H");
    auto n8 = dag.addChild(n7->m_id, "I");
    auto n9 = dag.addChild(n7->m_id, "J");

    dag.addEdge(n1,n2);
    dag.addEdge(n1,n6);
    dag.addEdge(n5,n2);
    dag.addEdge(n5,n6);

    vector<IndexType>* dfs0 = dag.getDFS(n0->m_id);
    ASSERT_TRUE(dfs0->size() == 10);

    vector<IndexType>* dfs1 = dag.getDFS(n1->m_id);
    ASSERT_TRUE(dfs1->size() == 5);

    vector<IndexType>* dfs2 = dag.getDFS(n6->m_id);
    ASSERT_TRUE(dfs2->size() == 1);

    vector<IndexType>* dfs3 = dag.getDFS(n9->m_id);
    ASSERT_TRUE(dfs3->size() == 1);
}

TEST(DAG, BFS)
{
    CDAG dag = CDAG();
    auto n0 = dag.addNode("A"); //Root
    auto n1 = dag.addChild(n0->m_id, "A");
    auto n2 = dag.addChild(n0->m_id, "A");
    auto n3 = dag.addChild(n0->m_id, "A");
    auto n4 = dag.addChild(n1->m_id, "A");
    auto n5 = dag.addChild(n1->m_id, "A");
    auto n6 = dag.addChild(n2->m_id, "A");
    auto n7 = dag.addChild(n3->m_id, "A");
    auto n8 = dag.addChild(n7->m_id, "A");
    auto n9 = dag.addChild(n7->m_id, "A");

    vector<IndexType>* dfs0 = dag.getBFS(n0->m_id);
    ASSERT_TRUE(dfs0->size() == 10);

    vector<IndexType>* dfs1 = dag.getBFS(n1->m_id);
    ASSERT_TRUE(dfs1->size() == 3);

    vector<IndexType>* dfs2 = dag.getBFS(n6->m_id);
    ASSERT_TRUE(dfs2->size() == 1);

    vector<IndexType>* dfs3 = dag.getBFS(n9->m_id);
    ASSERT_TRUE(dfs3->size() == 1);
}

TEST(DAG, BinarySaveLoad)
{
    CDAG dag = CDAG();

    auto a = dag.addNode("a");
    auto b = dag.addNode("b");
    auto c = dag.addNode("c");
    auto d = dag.addNode("d");
    auto e = dag.addNode("e");
    auto f = dag.addNode("f");
    auto g = dag.addNode("g");
    dag.addEdge(a, b);
    dag.addEdge(a, c);
    dag.addEdge(a, d);
    dag.addEdge(c, e);
    dag.addEdge(c, f);
    dag.addEdge(d, g);

    dag.save("treeFile");

    CDAG loadedCDAG = CDAG();
    loadedCDAG.load("treeFile");

    ASSERT_EQ(loadedCDAG.nodeCount(), 7);
    ASSERT_EQ(loadedCDAG.edgeCount(), 6);
}

TEST(DAG, JsonLoad)
{
    CDAG dag = CDAG();
    dag.loadJson("sample/cdag.json");

    ASSERT_EQ(dag.nodeCount(), 10); // 0,1,2,3,4,5,6,7,8,9
    ASSERT_EQ(dag.edgeCount(), 12); // 0>1,0>2,0>3,1>2,1>4,1>5,2>6,3>7,5>6,6>8,7>8,7>9

    auto aEdges = dag.getEdges(0);
    ASSERT_EQ(aEdges.size(), 3);
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 1) != aEdges.end());
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 2) != aEdges.end());
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 3) != aEdges.end());
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 10) == aEdges.end()); //control case

    auto bEdges = dag.getEdges(1);
    ASSERT_EQ(bEdges.size(), 3);
    ASSERT_TRUE(std::find(bEdges.begin(), bEdges.end(), 2) != bEdges.end());
    ASSERT_TRUE(std::find(bEdges.begin(), bEdges.end(), 4) != bEdges.end());
    ASSERT_TRUE(std::find(bEdges.begin(), bEdges.end(), 5) != bEdges.end());

    auto cEdges = dag.getEdges(2);
    ASSERT_EQ(cEdges.size(), 1);
    ASSERT_TRUE(std::find(cEdges.begin(), cEdges.end(), 6) != cEdges.end());
    
    auto dEdges = dag.getEdges(3);
    ASSERT_EQ(dEdges.size(), 1);
    ASSERT_TRUE(std::find(dEdges.begin(), dEdges.end(), 7) != dEdges.end());
    
    auto eEdges = dag.getEdges(4);
    ASSERT_EQ(eEdges.size(), 0);
    
    auto fEdges = dag.getEdges(5);
    ASSERT_EQ(fEdges.size(), 1);
    ASSERT_TRUE(std::find(fEdges.begin(), fEdges.end(), 6) != fEdges.end());
    
    auto gEdges = dag.getEdges(6);
    ASSERT_EQ(gEdges.size(), 1);
    ASSERT_TRUE(std::find(gEdges.begin(), gEdges.end(), 8) != gEdges.end());
    
    auto hEdges = dag.getEdges(7);
    ASSERT_EQ(hEdges.size(), 2);
    ASSERT_TRUE(std::find(hEdges.begin(), hEdges.end(), 8) != hEdges.end());
    ASSERT_TRUE(std::find(hEdges.begin(), hEdges.end(), 9) != hEdges.end());

    auto iEdges = dag.getEdges(8);
    ASSERT_EQ(iEdges.size(), 0);

    auto jEdges = dag.getEdges(9);
    ASSERT_EQ(jEdges.size(), 0);
}
