#include <string>
#include "gtest/gtest.h"
#include "data/CTree.h"

using namespace soda;

TEST(Tree, BuildTreeByLinkingNodes) {
    CTree tree = CTree();
    auto n0 = tree.addNode("A");
    auto n1 = tree.addNode("B");
    auto n2 = tree.addNode("C");
    auto n3 = tree.addNode("D");
    auto n4 = tree.addNode("E");
    auto n5 = tree.addNode("F");
    auto n6 = tree.addNode("G");
    auto n7 = tree.addNode("H");
    auto n8 = tree.addNode("I");
    auto n9 = tree.addNode("J");

    ASSERT_TRUE(tree.edgeCount() == 0);
    ASSERT_TRUE(tree.nodeCount() == 10);

    ASSERT_EQ(tree.getNodeValue(n0), "A");
    ASSERT_EQ(tree.getNodeValue(n1), "B");
    ASSERT_EQ(tree.getNodeValue(n2), "C");
    ASSERT_EQ(tree.getNodeValue(n3), "D");
    ASSERT_EQ(tree.getNodeValue(n4), "E");
    ASSERT_EQ(tree.getNodeValue(n5), "F");
    ASSERT_EQ(tree.getNodeValue(n6), "G");
    ASSERT_EQ(tree.getNodeValue(n7), "H");
    ASSERT_EQ(tree.getNodeValue(n8), "I");
    ASSERT_EQ(tree.getNodeValue(n9), "J");

    tree.addEdge(n0, n1);
    tree.addEdge(n0, n2);
    tree.addEdge(n0, n3);
    tree.addEdge(n1, n4);
    tree.addEdge(n1, n5);
    tree.addEdge(n2, n6);
    tree.addEdge(n3, n7);
    tree.addEdge(n7, n8);
    tree.addEdge(n7, n9);

    ASSERT_TRUE(tree.edgeCount() == 18);
}

TEST(Tree, BuildTreeByLinkingSubTrees) {
    CTree tree = CTree();
    auto n0 = tree.addNode("A");
    auto n1 = tree.addNode("B");
    auto n2 = tree.addNode("C");
    auto n3 = tree.addNode("D");
    auto n4 = tree.addNode("E");
    auto n5 = tree.addNode("F");
    auto n6 = tree.addNode("G");
    auto n7 = tree.addNode("H");
    auto n8 = tree.addNode("I");
    auto n9 = tree.addNode("J");

    tree.addEdge(n7, n8);
    tree.addEdge(n7, n9);

    tree.addEdge(n1, n4);
    tree.addEdge(n1, n5);

    tree.addEdge(n2, n6);

    tree.addEdge(n3, n7);

    tree.addEdge(n0, n1);
    tree.addEdge(n0, n2);
    tree.addEdge(n0, n3);

    ASSERT_TRUE(tree.edgeCount() == 18);
    ASSERT_TRUE(tree.nodeCount() == 10);
}

TEST(Tree, BuildTreeByLeaves) {
    CTree tree = CTree();
    auto n0 = tree.addNode("A"); //Root
    auto n1 = tree.addChild(n0->m_id, "B");
    auto n2 = tree.addChild(n0->m_id, "C");
    auto n3 = tree.addChild(n0->m_id, "D");
    auto n4 = tree.addChild(n1->m_id, "E");
    auto n5 = tree.addChild(n1->m_id, "F");
    auto n6 = tree.addChild(n2->m_id, "G");
    auto n7 = tree.addChild(n3->m_id, "H");
    auto n8 = tree.addChild(n7->m_id, "I");
    auto n9 = tree.addChild(n7->m_id, "J");

    ASSERT_TRUE(tree.nodeCount() == 10);
    ASSERT_TRUE(tree.edgeCount() == 18);
}

TEST(Tree, FormingCircleBySubTrees) {
    CTree tree = CTree();
    auto n0 = tree.addNode("A");
    auto n1 = tree.addNode("B");
    auto n2 = tree.addNode("C");
    auto n3 = tree.addNode("D");
    auto n4 = tree.addNode("E");
    auto n5 = tree.addNode("F");
    auto n6 = tree.addNode("G");
    auto n7 = tree.addNode("H");
    auto n8 = tree.addNode("I");
    auto n9 = tree.addNode("J");

    tree.addEdge(n7, n8);
    tree.addEdge(n7, n9);

    tree.addEdge(n1, n4);
    tree.addEdge(n1, n5);

    tree.addEdge(n2, n6);

    tree.addEdge(n3, n7);
    tree.addEdge(n0, n2);
    tree.addEdge(n0, n3);
    tree.addEdge(n0, n1);

    EXPECT_THROW(tree.addEdge(n7, n1), logic_error); //additional enclosing edge
}

TEST(Tree, DFS)
{
    CTree tree = CTree();
    auto n0 = tree.addNode("A"); //Root
    auto n1 = tree.addChild(n0->m_id, "B");
    auto n2 = tree.addChild(n0->m_id, "C");
    auto n3 = tree.addChild(n0->m_id, "D");
    auto n4 = tree.addChild(n1->m_id, "E");
    auto n5 = tree.addChild(n1->m_id, "F");
    auto n6 = tree.addChild(n2->m_id, "G");
    auto n7 = tree.addChild(n3->m_id, "H");
    auto n8 = tree.addChild(n7->m_id, "I");
    auto n9 = tree.addChild(n7->m_id, "J");

    vector<IndexType>* dfs0 = tree.getDFS(n0->m_id);
    ASSERT_TRUE(dfs0->size() == 10);

    vector<IndexType>* dfs1 = tree.getDFS(n1->m_id);
    ASSERT_TRUE(dfs1->size() == 10);

    vector<IndexType>* dfs2 = tree.getDFS(n6->m_id);
    ASSERT_TRUE(dfs2->size() == 10);

    vector<IndexType>* dfs3 = tree.getDFS(n9->m_id);
    ASSERT_TRUE(dfs3->size() == 10);
}

TEST(Tree, BFS)
{
    CTree tree = CTree();
    auto n0 = tree.addNode("A"); //Root
    auto n1 = tree.addChild(n0->m_id, "A");
    auto n2 = tree.addChild(n0->m_id, "A");
    auto n3 = tree.addChild(n0->m_id, "A");
    auto n4 = tree.addChild(n1->m_id, "A");
    auto n5 = tree.addChild(n1->m_id, "A");
    auto n6 = tree.addChild(n2->m_id, "A");
    auto n7 = tree.addChild(n3->m_id, "A");
    auto n8 = tree.addChild(n7->m_id, "A");
    auto n9 = tree.addChild(n7->m_id, "A");

    vector<IndexType>* dfs0 = tree.getBFS(n0->m_id);
    ASSERT_TRUE(dfs0->size() == 10);

    vector<IndexType>* dfs1 = tree.getBFS(n1->m_id);
    ASSERT_TRUE(dfs1->size() == 10);

    vector<IndexType>* dfs2 = tree.getBFS(n6->m_id);
    ASSERT_TRUE(dfs2->size() == 10);

    vector<IndexType>* dfs3 = tree.getBFS(n9->m_id);
    ASSERT_TRUE(dfs3->size() == 10);
}

TEST(Tree, SaveLoad)
{
    CTree tree = CTree();

    auto a = tree.addNode("a");
    auto b = tree.addNode("b");
    auto c = tree.addNode("c");
    auto d = tree.addNode("d");
    auto e = tree.addNode("e");
    auto f = tree.addNode("f");
    auto g = tree.addNode("g");
    tree.addEdge(a, b);
    tree.addEdge(a, c);
    tree.addEdge(a, d);
    tree.addEdge(c, e);
    tree.addEdge(c, f);
    tree.addEdge(d, g);

    tree.save("treeFile");

    CTree loadedCTree = CTree();
    loadedCTree.load("treeFile");

    ASSERT_EQ(loadedCTree.nodeCount(), 7);
    ASSERT_EQ(loadedCTree.edgeCount(), 12);
}

TEST(Tree, JsonLoad)
{
    CTree tree = CTree();
    tree.loadJson("sample/ctree.json");

    //alias = index
    //a = 0
    //b = 1
    //c = 2
    //d = 3
    //e = 4
    //f = 5
    //g = 6

    ASSERT_EQ(tree.nodeCount(), 7);  // a,b,c,d,e,f,g
    ASSERT_EQ(tree.edgeCount(), 12); // ab,ac,ad,ce,cf,dg (6*2)

    auto aEdges = tree.getEdges(0);
    ASSERT_EQ(aEdges.size(), 3); // 1(b), 2(c), 3(d)
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 1) != aEdges.end());
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 2) != aEdges.end());
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 3) != aEdges.end());
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 10) == aEdges.end()); //control case

    auto bEdges = tree.getEdges(1);
    ASSERT_EQ(bEdges.size(), 1); //0(a)
    ASSERT_TRUE(std::find(bEdges.begin(), bEdges.end(), 0) != bEdges.end());

    auto cEdges = tree.getEdges(2);
    ASSERT_EQ(cEdges.size(), 3); //0(a), 4(e), 5(f)
    ASSERT_TRUE(std::find(cEdges.begin(), cEdges.end(), 0) != cEdges.end());
    ASSERT_TRUE(std::find(cEdges.begin(), cEdges.end(), 4) != cEdges.end());
    ASSERT_TRUE(std::find(cEdges.begin(), cEdges.end(), 5) != cEdges.end());

    auto dEdges = tree.getEdges(3);
    ASSERT_EQ(dEdges.size(), 2); //0(a), 6(g)
    ASSERT_TRUE(std::find(dEdges.begin(), dEdges.end(), 0) != dEdges.end());
    ASSERT_TRUE(std::find(dEdges.begin(), dEdges.end(), 6) != dEdges.end());

    auto eEdges = tree.getEdges(4);
    ASSERT_EQ(eEdges.size(), 1); //2(c)
    ASSERT_TRUE(std::find(eEdges.begin(), eEdges.end(), 2) != eEdges.end());

    auto fEdges = tree.getEdges(5);
    ASSERT_EQ(fEdges.size(), 1); //2(c)
    ASSERT_TRUE(std::find(fEdges.begin(), fEdges.end(), 2) != fEdges.end());

    auto gEdges = tree.getEdges(6);
    ASSERT_EQ(gEdges.size(), 1); //3(d)
    ASSERT_TRUE(std::find(gEdges.begin(), gEdges.end(), 3) != gEdges.end());
}
