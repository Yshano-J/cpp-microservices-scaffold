#include <gtest/gtest.h>
#include <iostream>
#include <unordered_map>

// 以下为测试用例
TEST(unordered_map_TEST, insert)
{
    std::unordered_map<int, int> map;
    map.insert(std::make_pair(1, 1));
    map.insert(std::make_pair(2, 2));
    map.insert(std::make_pair(3, 3));
    ASSERT_EQ(map.size(), 2);
}

TEST(unordered_map_TEST, find)
{
    std::unordered_map<int, int> map;
    map.insert(std::make_pair(1, 1));
    map.insert(std::make_pair(2, 2));
    map.insert(std::make_pair(3, 3));
    ASSERT_NE(map.find(1), map.end());
    ASSERT_EQ(map.find(2)->second, 2);
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv); // 初始化
    return RUN_ALL_TESTS();
}