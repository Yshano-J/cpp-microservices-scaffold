
#include <gtest/gtest.h>
#include <iostream>
#include <unordered_map>


class LocalTest : public testing::Test
{
public:
    // 在所有测试用例开始/结束 前/后
    static void SetUpTestCase()
    {
        std::cout << "----执行所有测试用例之前----" << std::endl;
    }
    static void TearDownTestCase()
    {
        std::cout << "----执行所有测试用例之后----" << std::endl;
    }

    // 保证每个测试用例有独立的测试环境，重写虚函数
    virtual void SetUp()
    {
        std::cout << "----执行测试用例之前----" << std::endl;
        map.insert(std::make_pair(1, 1));
        map.insert(std::make_pair(2, 2));
        map.insert(std::make_pair(3, 3));
    }
    virtual void TearDown()
    {
        std::cout << "----执行测试用例之后----" << std::endl;
        map.clear();
    }

public:
    std::unordered_map<int, int> map;
};
// 以下为测试用例
TEST_F(LocalTest, insert)
{
    std::cout << "----执行测试用例1----" << std::endl;
    ASSERT_EQ(map.size(), 3);
    map.erase(1);
}
TEST_F(LocalTest, erase)
{
    std::cout << "----执行测试用例2----" << std::endl;

    map.erase(1); // 会对后面的造成影响
}
TEST_F(LocalTest, find)
{
    std::cout << "----执行测试用例3----" << std::endl;

    ASSERT_NE(map.find(1), map.end());
    ASSERT_EQ(map.find(2)->second, 2);
    ASSERT_EQ(map.size(), 1);
}
// 每个测试用例是独立的，进行每个测试用例的时候，都会先去初始化（SetUp）,该测试用例结束后会去清理。
// 每个测试用例均是如此
int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv); // 初始化
    return RUN_ALL_TESTS();
}