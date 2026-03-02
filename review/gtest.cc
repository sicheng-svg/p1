#include <gtest/gtest.h>

int add(int a, int b) {
    return a + b;
}

TEST(testCase, test1){
    EXPECT_EQ(add(1, 2), 3);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}