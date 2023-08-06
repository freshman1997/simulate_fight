#include "gtest/gtest.h"

int test_add(int a, int b)
{
    return a + b;
}


TEST(TestAddDemo, test_add) {
    int res = test_add(10, 100);
    EXPECT_EQ(res, 110);
}

