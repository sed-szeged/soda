#include <string>
#include "gtest/gtest.h"
#include "data/CChain.h"

using namespace soda;

TEST(Chain, BasicOperations) {
    CChain s = CChain();
    vector<string> sVector = vector<string>();
    sVector.push_back("_0");
    sVector.push_back("_1");

    ASSERT_EQ(s.count(), 0);

    s.add("_0");
    s.add("_1");
    s.add("_2");

    ASSERT_TRUE(s.contains("_0"));
    ASSERT_TRUE(s.contains("_1"));
    ASSERT_TRUE(s.contains("_2"));

    ASSERT_TRUE(s.getId("_0") == 0);
    ASSERT_TRUE(s.getId("_1") == 1);
    ASSERT_TRUE(s.getId("_2") == 2);
    ASSERT_EQ(s.count(), 3);

    s.remove("_1");

    ASSERT_EQ(s.count(), 2);
    ASSERT_FALSE(s.contains("_1"));
    EXPECT_THROW(s.getId("_1"), std::out_of_range);
    ASSERT_TRUE(s.getId("_2") == 2);
    ASSERT_EQ(s.getValue(0), "_0");
    ASSERT_EQ(s.getValue(2), "_2");

    s.clear();

    ASSERT_EQ(s.count(), 0);

    s.add(sVector);

    ASSERT_EQ(s.count(), 2);
}

TEST(Chain, Iterators)
{
    CChain s = CChain();
    s.add("_0");
    s.add("_1");
    s.add("_2");

    ASSERT_EQ(*(s.first()), "_0");
    ASSERT_EQ(*--s.end(), "_2");
}

TEST(Chain, SaveLoad)
{
    CChain s = CChain();
    s.add("_0");
    s.add("_1");
    s.add("_2");
    s.save("chainFile");

    CChain a = CChain();
    a.load("chainFile");

    ASSERT_EQ(a.count(), 3);
    ASSERT_EQ(*(a.first()), "_0");
    ASSERT_EQ(a.getValue(0), "_0");
    ASSERT_EQ(a.getValue(1), "_1");
    ASSERT_EQ(a.getValue(2), "_2");
}

TEST(Chain, JsonLoad)
{
    CChain s = CChain();
    s.loadJson("sample/cchain.json");

    ASSERT_EQ(s.count(), 4);
    ASSERT_EQ(*(s.first()), "Element1");
    ASSERT_EQ(*(s.end()-1), "Debug");
}
