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

TEST(Graph, JsonLoad)
{
    CGraph g = CGraph();
    g.loadJson("sample/cgraph.json");

    //alias = index
    //a = 0
    //b = 1
    //c = 2
    //d = 3
    //e = 4
    //f = 5
    //g = 6

    ASSERT_EQ(g.nodeCount(), 7);  // a,b,c,d,e,f,g
    ASSERT_EQ(g.edgeCount(), 14); // ab,ac,ad,ce,cf,dg,ga (7*2)

    auto aEdges = g.getEdges(0);
    ASSERT_EQ(aEdges.size(), 4); // 1(b), 2(c), 3(d), 4(g)
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 1) != aEdges.end());
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 2) != aEdges.end());
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 3) != aEdges.end());
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 6) != aEdges.end());
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 10) == aEdges.end()); //control case

    auto bEdges = g.getEdges(1);
    ASSERT_EQ(bEdges.size(), 1); //0(a)
    ASSERT_TRUE(std::find(bEdges.begin(), bEdges.end(), 0) != bEdges.end());

    auto cEdges = g.getEdges(2);
    ASSERT_EQ(cEdges.size(), 3); //0(a), 4(e), 5(f)
    ASSERT_TRUE(std::find(cEdges.begin(), cEdges.end(), 0) != cEdges.end());
    ASSERT_TRUE(std::find(cEdges.begin(), cEdges.end(), 4) != cEdges.end());
    ASSERT_TRUE(std::find(cEdges.begin(), cEdges.end(), 5) != cEdges.end());

    auto dEdges = g.getEdges(3);
    ASSERT_EQ(dEdges.size(), 2); //0(a), 6(g)
    ASSERT_TRUE(std::find(dEdges.begin(), dEdges.end(), 0) != dEdges.end());
    ASSERT_TRUE(std::find(dEdges.begin(), dEdges.end(), 6) != dEdges.end());

    auto eEdges = g.getEdges(4);
    ASSERT_EQ(eEdges.size(), 1); //2(c)
    ASSERT_TRUE(std::find(eEdges.begin(), eEdges.end(), 2) != eEdges.end());

    auto fEdges = g.getEdges(5);
    ASSERT_EQ(fEdges.size(), 1); //2(c)
    ASSERT_TRUE(std::find(fEdges.begin(), fEdges.end(), 2) != fEdges.end());

    auto gEdges = g.getEdges(6);
    ASSERT_EQ(gEdges.size(), 2); //3(d), 0(a)
    ASSERT_TRUE(std::find(gEdges.begin(), gEdges.end(), 3) != gEdges.end());
    ASSERT_TRUE(std::find(gEdges.begin(), gEdges.end(), 0) != gEdges.end());
}
