#include <string>
#include "gtest/gtest.h"
#include "data/CGraph.h"

using namespace soda;
 
TEST(Graph, BasicOperations) {    
    CGraph g = CGraph(10);
    g.addNode("a");
    g.addNode("b");
    g.addNode("c");

    ASSERT_TRUE(g.edgeCount() == 0);
    ASSERT_TRUE(g.nodeCount() == 3);

    g.addEdge("a", "b");
    g.addEdge("a", "c");

    ASSERT_TRUE(g.edgeCount() == 2);
}
