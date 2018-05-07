#include <string>
#include "gtest/gtest.h"
#include "data/CTree.h"

using namespace soda;

TEST(Tree, BuildTreeByLinkingNodes) {
    CTree tree = CTree();
    IndexType n0 = tree.addNode("A");
    IndexType n1 = tree.addNode("B");
    IndexType n2 = tree.addNode("C");
    IndexType n3 = tree.addNode("D");
    IndexType n4 = tree.addNode("A");
    IndexType n5 = tree.addNode("B");
    IndexType n6 = tree.addNode("C");
    IndexType n7 = tree.addNode("D");
    IndexType n8 = tree.addNode("A");
    IndexType n9 = tree.addNode("B");

    ASSERT_TRUE(tree.edgeCount() == 0);
    ASSERT_TRUE(tree.nodeCount() == 10);

    ASSERT_EQ(tree.getNodeValue(n0), "A");
    ASSERT_EQ(tree.getNodeValue(n1), "B");
    ASSERT_EQ(tree.getNodeValue(n2), "C");
    ASSERT_EQ(tree.getNodeValue(n3), "D");
    ASSERT_EQ(tree.getNodeValue(n4), "A");
    ASSERT_EQ(tree.getNodeValue(n5), "B");
    ASSERT_EQ(tree.getNodeValue(n6), "C");
    ASSERT_EQ(tree.getNodeValue(n7), "D");
    ASSERT_EQ(tree.getNodeValue(n8), "A");
    ASSERT_EQ(tree.getNodeValue(n9), "B");

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
    IndexType n0 = tree.addNode("A");
    IndexType n1 = tree.addNode("B");
    IndexType n2 = tree.addNode("C");
    IndexType n3 = tree.addNode("D");
    IndexType n4 = tree.addNode("E");
    IndexType n5 = tree.addNode("F");
    IndexType n6 = tree.addNode("G");
    IndexType n7 = tree.addNode("H");
    IndexType n8 = tree.addNode("I");
    IndexType n9 = tree.addNode("J");

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
    IndexType n0 = tree.addNode("A"); //Root
    IndexType n1 = tree.addChild(n0, "B");
    IndexType n2 = tree.addChild(n0, "C");
    IndexType n3 = tree.addChild(n0, "D");
    IndexType n4 = tree.addChild(n1, "E");
    IndexType n5 = tree.addChild(n1, "F");
    IndexType n6 = tree.addChild(n2, "G");
    IndexType n7 = tree.addChild(n3, "H");
    IndexType n8 = tree.addChild(n7, "I");
    IndexType n9 = tree.addChild(n7, "J");

    ASSERT_TRUE(tree.nodeCount() == 10);
    ASSERT_TRUE(tree.edgeCount() == 18);
}

TEST(Tree, FormingCircleBySubTrees) {
    CTree tree = CTree();
    IndexType n0 = tree.addNode("A");
    IndexType n1 = tree.addNode("B");
    IndexType n2 = tree.addNode("C");
    IndexType n3 = tree.addNode("D");
    IndexType n4 = tree.addNode("E");
    IndexType n5 = tree.addNode("F");
    IndexType n6 = tree.addNode("G");
    IndexType n7 = tree.addNode("H");
    IndexType n8 = tree.addNode("I");
    IndexType n9 = tree.addNode("J");

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
    IndexType n0 = tree.addNode("A"); //Root
    IndexType n1 = tree.addChild(n0, "B");
    IndexType n2 = tree.addChild(n0, "C");
    IndexType n3 = tree.addChild(n0, "D");
    IndexType n4 = tree.addChild(n1, "E");
    IndexType n5 = tree.addChild(n1, "F");
    IndexType n6 = tree.addChild(n2, "G");
    IndexType n7 = tree.addChild(n3, "H");
    IndexType n8 = tree.addChild(n7, "I");
    IndexType n9 = tree.addChild(n7, "J");

    vector<IndexType>* dfs0 = tree.getDFS(n0);
    ASSERT_TRUE(dfs0->size() == 10);

    vector<IndexType>* dfs1 = tree.getDFS(n1);
    ASSERT_TRUE(dfs1->size() == 10);

    vector<IndexType>* dfs2 = tree.getDFS(n6);
    ASSERT_TRUE(dfs2->size() == 10);

    vector<IndexType>* dfs3 = tree.getDFS(n9);
    ASSERT_TRUE(dfs3->size() == 10);
}

TEST(Tree, BFS)
{
    CTree tree = CTree();
    IndexType n0 = tree.addNode("A"); //Root
    IndexType n1 = tree.addChild(n0, "A");
    IndexType n2 = tree.addChild(n0, "A");
    IndexType n3 = tree.addChild(n0, "A");
    IndexType n4 = tree.addChild(n1, "A");
    IndexType n5 = tree.addChild(n1, "A");
    IndexType n6 = tree.addChild(n2, "A");
    IndexType n7 = tree.addChild(n3, "A");
    IndexType n8 = tree.addChild(n7, "A");
    IndexType n9 = tree.addChild(n7, "A");

    vector<IndexType>* dfs0 = tree.getBFS(n0);
    ASSERT_TRUE(dfs0->size() == 10);

    vector<IndexType>* dfs1 = tree.getBFS(n1);
    ASSERT_TRUE(dfs1->size() == 10);

    vector<IndexType>* dfs2 = tree.getBFS(n6);
    ASSERT_TRUE(dfs2->size() == 10);

    vector<IndexType>* dfs3 = tree.getBFS(n9);
    ASSERT_TRUE(dfs3->size() == 10);
}

TEST(Tree, SaveLoad)
{

}

TEST(Tree, JsonLoad)
{

}
