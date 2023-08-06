#include <iostream>

#include "gtest/gtest.h"
#include "nlohmann/json.hpp"

bool test_json()
{
    using namespace nlohmann::literals;
    using json = nlohmann::json;
    json ex1 = json::parse(R"({"pi":3.14159, "happy": true})");

    std::cout << ex1.dump(4) << std::endl;    
    return true;
}

TEST(TestJsonDemo, test_json) {
    bool res = test_json();
    EXPECT_EQ(res, true);
}