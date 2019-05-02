// Copyright 2019 Preda Andrei

#include <string>

#include "./catch.hpp"

#include "./uber_node.h"

TEST_CASE("UberNode is constructed correctly")
{
    UberNode node("Piata Amzei");
    REQUIRE(node.Name() == "Piata Amzei");
    REQUIRE(node.Drivers().empty());
}

TEST_CASE("Drivers are added correctly")
{
    UberNode node("Dristor");

    node.AddDriver(1);
    REQUIRE(node.Drivers().size() == 1);
    REQUIRE(node.Drivers()[0] == 1);

    SECTION("drivers are added only once") {
        node.AddDriver(1);
        node.AddDriver(1);
        REQUIRE(node.Drivers().size() == 1);
        REQUIRE(node.Drivers()[0] == 1);
    }
}

TEST_CASE("Drivers are removed correctly")
{
    UberNode node("Damaroaia");

    node.AddDriver(1);
    node.AddDriver(2);
    node.AddDriver(3);

    SECTION("removing existing drivers") {
        node.RemoveDriver(2);
        REQUIRE(node.Drivers().size() == 2);

        node.RemoveDriver(3);
        REQUIRE(node.Drivers().size() == 1);
        REQUIRE(node.Drivers()[0] == 1);
    }

    SECTION("removing non-existing drivers") {
        node.RemoveDriver(5);
        REQUIRE(node.Drivers().size() == 3);

        node.RemoveDriver(1);
        REQUIRE(node.Drivers().size() == 2);
        node.RemoveDriver(1);
        REQUIRE(node.Drivers().size() == 2);
    }
}
