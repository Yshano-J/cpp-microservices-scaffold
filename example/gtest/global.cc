/*
//全局事件:针对整个测试程序,提供全局事件机制,能够在测试之前配置测试环境数据,测试完毕后清
理数据
//先定义环境类,通过继承testing::Environment的派⽣类来完成
//重写的虚函数接⼝SetUp会在测试之前被调⽤; TearDown会在测试完毕后调⽤.
*/

#include <gtest/gtest.h>
#include <iostream>
#include <unordered_map>

std::unordered_map<int, int> map;

class GlobalTestEnvironment : public testing::Environment
{
public:
    virtual void SetUp() override
    {
        std::cout << "----执行测试用例之前----" << std::endl;
    }
    virtual void TearDown() override
    {
        std::cout << "----执行测试用例之后----" << std::endl;
    }
};
// 以下为测试用例
TEST(GlobalTestEnvironment, insert)
{
    map.insert(std::make_pair(1, 1));
    map.insert(std::make_pair(2, 2));
    map.insert(std::make_pair(3, 3));
    ASSERT_EQ(map.size(), 2);
}
TEST(GlobalTestEnvironment, erase)
{
    map.erase(1); // 会对后面的造成影响
}
TEST(GlobalTestEnvironment, find)
{
    ASSERT_NE(map.find(1), map.end());
    ASSERT_EQ(map.find(2)->second, 2);
}
int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv); // 初始化
    testing::AddGlobalTestEnvironment(new GlobalTestEnvironment());
    return RUN_ALL_TESTS();
}