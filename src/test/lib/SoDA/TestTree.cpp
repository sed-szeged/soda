/*
 * Copyright (C): 2013-2014 Department of Software Engineering, University of Szeged
 *
 * Author: David Imre Adam <adam.david.imre@gmail.com>
 *
 * This file is part of SoDA.
 *
 *  SoDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  SoDA is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with SoDA.  If not, see <http://www.gnu.org/licenses/>.
 */

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

    ASSERT_TRUE(tree.edgeCount() == 9);
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

    ASSERT_TRUE(tree.edgeCount() == 9);
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
    ASSERT_TRUE(tree.edgeCount() == 9);
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
    ASSERT_TRUE(dfs1->size() == 3);

    vector<IndexType>* dfs2 = tree.getDFS(n3->m_id);
    ASSERT_TRUE(dfs2->size() == 4);

    vector<IndexType>* dfs3 = tree.getDFS(n9->m_id);
    ASSERT_TRUE(dfs3->size() == 1);
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
    ASSERT_TRUE(dfs1->size() == 3);

    vector<IndexType>* dfs2 = tree.getBFS(n2->m_id);
    ASSERT_TRUE(dfs2->size() == 2);

    vector<IndexType>* dfs3 = tree.getBFS(n9->m_id);
    ASSERT_TRUE(dfs3->size() == 1);
}

TEST(Tree, BinarySaveLoad)
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
    ASSERT_EQ(loadedCTree.edgeCount(), 6);
}

TEST(Tree, JsonLoad)
{
    CTree tree = CTree();
    tree.loadJson("sample/ctree.json");

    ASSERT_EQ(tree.nodeCount(), 10);  // 0,1,2,3,4,5,6,7,8,9
    ASSERT_EQ(tree.edgeCount(), 9); // 0>1,0>2,0>3,1>4,1>5,2>6,3>7,7>8,7>9

    auto aEdges = tree.getEdges(0);
    ASSERT_EQ(aEdges.size(), 3);
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 1) != aEdges.end());
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 2) != aEdges.end());
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 3) != aEdges.end());
    ASSERT_TRUE(std::find(aEdges.begin(), aEdges.end(), 10) == aEdges.end()); //control case

    auto bEdges = tree.getEdges(1);
    ASSERT_EQ(bEdges.size(), 2);
    ASSERT_TRUE(std::find(bEdges.begin(), bEdges.end(), 4) != bEdges.end());
    ASSERT_TRUE(std::find(bEdges.begin(), bEdges.end(), 5) != bEdges.end());

    auto cEdges = tree.getEdges(2);
    ASSERT_EQ(cEdges.size(), 1);
    ASSERT_TRUE(std::find(cEdges.begin(), cEdges.end(), 6) != cEdges.end());
    
    auto dEdges = tree.getEdges(3);
    ASSERT_EQ(dEdges.size(), 1);
    ASSERT_TRUE(std::find(dEdges.begin(), dEdges.end(), 7) != dEdges.end());
    
    auto eEdges = tree.getEdges(4);
    ASSERT_EQ(eEdges.size(), 0);
    
    auto fEdges = tree.getEdges(5);
    ASSERT_EQ(fEdges.size(), 0);
    
    auto gEdges = tree.getEdges(6);
    ASSERT_EQ(gEdges.size(), 0);
    
    auto hEdges = tree.getEdges(7);
    ASSERT_EQ(hEdges.size(), 2);
    ASSERT_TRUE(std::find(hEdges.begin(), hEdges.end(), 8) != hEdges.end());
    ASSERT_TRUE(std::find(hEdges.begin(), hEdges.end(), 9) != hEdges.end());

    auto iEdges = tree.getEdges(8);
    ASSERT_EQ(iEdges.size(), 0);

    auto jEdges = tree.getEdges(9);
    ASSERT_EQ(jEdges.size(), 0);

}

TEST(Tree, convertToNodeIdChains)
{
    CTree tree = CTree();
    auto n0 = tree.addNode("A"); //Root
    auto n1 = tree.addChild(n0->m_id, "B");
    auto n2 = tree.addChild(n0->m_id, "C");
    auto n3 = tree.addChild(n0->m_id, "D");
        
    auto n4 = tree.addChild(n1->m_id, "C");
    auto n5 = tree.addChild(n1->m_id, "E");
    auto n6 = tree.addChild(n1->m_id, "F");

    auto n7 = tree.addChild(n4->m_id, "F");
    auto n8 = tree.addChild(n7->m_id, "I");

    auto n9 = tree.addChild(n6->m_id, "C");
    auto n10 = tree.addChild(n9->m_id, "G");
    auto n11 = tree.addChild(n10->m_id, "I");

    auto n12 = tree.addChild(n2->m_id, "G");
    auto n13 = tree.addChild(n12->m_id, "I");

    auto n14 = tree.addChild(n3->m_id, "H");
    auto n15 = tree.addChild(n14->m_id, "I");
    auto n16 = tree.addChild(n14->m_id, "J");


    vector<list<IndexType>*>* nodeIdChains = tree.convertToNodeIdChains();

    ASSERT_EQ(nodeIdChains->size(), 6);

    String chain0;
    for(list<IndexType>::iterator c = nodeIdChains->at(0)->begin(); c != nodeIdChains->at(0)->end(); c++)
    {
        chain0.append(tree.getNodeValue(*c));
    }
    ASSERT_EQ(chain0, "ABE");

    String chain1;
    for(list<IndexType>::iterator c = nodeIdChains->at(1)->begin(); c != nodeIdChains->at(1)->end(); c++)
    {
        chain1.append(tree.getNodeValue(*c));
    }
    ASSERT_EQ(chain1, "ACGI");

    String chain2;
    for(list<IndexType>::iterator c = nodeIdChains->at(2)->begin(); c != nodeIdChains->at(2)->end(); c++)
    {
        chain2.append(tree.getNodeValue(*c));
    }
    ASSERT_EQ(chain2, "ADHI");

    String chain3;
    for(list<IndexType>::iterator c = nodeIdChains->at(3)->begin(); c != nodeIdChains->at(3)->end(); c++)
    {
        chain3.append(tree.getNodeValue(*c));
    }
    ASSERT_EQ(chain3, "ADHJ");

    String chain4;
    for(list<IndexType>::iterator c = nodeIdChains->at(4)->begin(); c != nodeIdChains->at(4)->end(); c++)
    {
        chain4.append(tree.getNodeValue(*c));
    }
    ASSERT_EQ(chain4, "ABCFI");

    String chain5;
    for(list<IndexType>::iterator c = nodeIdChains->at(5)->begin(); c != nodeIdChains->at(5)->end(); c++)
    {
        chain5.append(tree.getNodeValue(*c));
    }
    ASSERT_EQ(chain5, "ABFCGI");
}

TEST(Tree, toDAG)
{
    CTree tree = CTree();
    auto n0 = tree.addNode("A"); //Root
    auto n1 = tree.addChild(n0->m_id, "B");
    auto n2 = tree.addChild(n0->m_id, "C");
    auto n3 = tree.addChild(n0->m_id, "D");
        
    auto n4 = tree.addChild(n1->m_id, "C");
    auto n5 = tree.addChild(n1->m_id, "E");
    auto n6 = tree.addChild(n1->m_id, "F");

    auto n7 = tree.addChild(n4->m_id, "G");
    auto n8 = tree.addChild(n7->m_id, "I");

    auto n9 = tree.addChild(n6->m_id, "C");
    auto n10 = tree.addChild(n9->m_id, "G");
    auto n11 = tree.addChild(n10->m_id, "I");

    auto n12 = tree.addChild(n2->m_id, "G");
    auto n13 = tree.addChild(n12->m_id, "I");

    auto n14 = tree.addChild(n3->m_id, "H");
    auto n15 = tree.addChild(n14->m_id, "I");
    auto n16 = tree.addChild(n14->m_id, "J");

    CDAG* dag = tree.toDAG();

    ASSERT_EQ(dag->nodeCount(), 17);
    ASSERT_EQ(dag->edgeCount(), 16);

    auto contractedDAG = dag->getContractedDAG();

    ASSERT_EQ(contractedDAG->nodeCount(), 10);
    ASSERT_EQ(contractedDAG->edgeCount(), 12);
}
