#include <gtest/gtest.h>
#include <string>

#include "search.cpp"

// Поиск в одном ноде
TEST(ContainsInTree, CompareElements)
{
    Dictionary<int, int> a;
    // std::string key = "health";
    // std::string key2 = "meat";
    // std::string key3 = "salad";
    a.put(1, 10);
    a.put(3, 20);
    a.put(5, 30);
    a.put(6, 30);
    a.put(7, 30);

    // a.put(4, 30);
    // a.put(4, 67);

    std::cout << "Size: " << a.size() << std::endl;

    a.show();

    ASSERT_TRUE(a.contains(1));
    ASSERT_TRUE(a.contains(3));
    ASSERT_TRUE(a.contains(5));
    ASSERT_TRUE(a.contains(6));
    ASSERT_TRUE(a.contains(7));
};

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}