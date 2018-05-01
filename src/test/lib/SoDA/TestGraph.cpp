#include <string>
#include "gtest/gtest.h"
#include "data/CGraph.h"

using namespace soda;
 
TEST(Graph, BasicOperations) {    
    CGraph g = CGraph();
    IndexType a = g.addNode("a");
    IndexType b = g.addNode("b");
    IndexType c = g.addNode("c");

    ASSERT_TRUE(g.edgeCount() == 0);
    ASSERT_TRUE(g.nodeCount() == 3);

    g.addEdge(a, b);
    g.addEdge(a, c);

    ASSERT_TRUE(g.edgeCount() == 4);
}

TEST(Graph, DFS)
{
    CGraph graph = CGraph();

    IndexType a = graph.addNode("a");
    IndexType b = graph.addNode("b");
    IndexType c = graph.addNode("c");
    IndexType d = graph.addNode("d");
    IndexType e = graph.addNode("e");
    IndexType f = graph.addNode("f");
    IndexType g = graph.addNode("g");
    graph.addEdge(a, b);
    graph.addEdge(a, c);
    graph.addEdge(a, d);
    graph.addEdge(c, e);
    graph.addEdge(c, f);
    graph.addEdge(d, g);
   
    vector<IndexType> dfs0 = graph.getDFS(0);
    ASSERT_TRUE(dfs0.size() == 7);
    
    vector<IndexType> dfs1 = graph.getDFS(1);
    ASSERT_TRUE(dfs1.size() == 7);
    
    vector<IndexType> dfs2 = graph.getDFS(2);
    ASSERT_TRUE(dfs2.size() == 7);
    
    vector<IndexType> dfs3 = graph.getDFS(3);
    ASSERT_TRUE(dfs3.size() == 7);
}

TEST(Graph, BFS)
{
    CGraph graph = CGraph();

    IndexType a = graph.addNode("a");
    IndexType b = graph.addNode("b");
    IndexType c = graph.addNode("c");
    IndexType d = graph.addNode("d");
    IndexType e = graph.addNode("e");
    IndexType f = graph.addNode("f");
    IndexType g = graph.addNode("g");
    graph.addEdge(a, b);
    graph.addEdge(a, c);
    graph.addEdge(a, d);
    graph.addEdge(c, e);
    graph.addEdge(c, f);
    graph.addEdge(d, g);
   
    vector<IndexType> bfs0 = graph.getBFS(0);
    ASSERT_TRUE(bfs0.size() == 7);
    
    vector<IndexType> bfs1 = graph.getBFS(1);
    ASSERT_TRUE(bfs1.size() == 7);
    
    vector<IndexType> bfs2 = graph.getBFS(2);
    ASSERT_TRUE(bfs2.size() == 7);
    
    vector<IndexType> bfs3 = graph.getBFS(3);
    ASSERT_TRUE(bfs3.size() == 7);
}

TEST(Graph, SaveLoad)
{
    CGraph graph = CGraph();

    IndexType a = graph.addNode("a");
    IndexType b = graph.addNode("b");
    IndexType c = graph.addNode("c");
    IndexType d = graph.addNode("d");
    IndexType e = graph.addNode("e");
    IndexType f = graph.addNode("f");
    IndexType g = graph.addNode("g");
    graph.addEdge(a, b);
    graph.addEdge(a, c);
    graph.addEdge(a, d);
    graph.addEdge(c, e);
    graph.addEdge(c, f);
    graph.addEdge(d, g);
    
    graph.save("graphFile");
    
    CGraph loadedGraph = CGraph();
    loadedGraph.load("graphFile");

    ASSERT_EQ(loadedGraph.nodeCount(), 7);
    ASSERT_EQ(loadedGraph.edgeCount(), 12);
}