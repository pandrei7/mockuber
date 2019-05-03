// Copyright 2019 Preda Andrei

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "./catch.hpp"

#include "./uber_map.h"

TEST_CASE("Ids work correctly")
{
    UberMap m;
    m.AddIntersection("A");
    m.AddIntersection("B");

    SECTION("existing nodes") {
        REQUIRE(m.Id("A") == 0);
        REQUIRE(m.Id("B") == 1);
    }

    SECTION("non-existing nodes") {
        REQUIRE(m.Id("Z") == -1);
    }
}

TEST_CASE("Distance works correctly")
{
    UberMap m;
    m.AddIntersection("A");
    m.AddIntersection("B");
    m.AddIntersection("C");

    m.AddStreet("A", "B");
    m.AddStreet("B", "C");

    REQUIRE(m.Distance("A", "B") == 1);
    REQUIRE(m.Distance("A", "C") == 2);
    REQUIRE(m.Distance("B", "A") == -1);
    REQUIRE(m.Distance("B", "C") == 1);
    REQUIRE(m.Distance("C", "A") == -1);
    REQUIRE(m.Distance("C", "B") == -1);

    REQUIRE(m.Distance("A", "A") == 0);

    SECTION("self loops work correctly") {
        m.AddStreet("A", "A");
        REQUIRE(m.Distance("A", "A") == 0);
    }
}

TEST_CASE("Street operations work ok")
{
    std::vector<std::string> names = {"A", "B", "C", "D"};

    std::vector<std::pair<std::string, std::string>> edges = {
        {"A", "B"}, {"A", "C"}, {"B", "D"}
    };

    UberMap m;
    for (const auto &name : names) {
        m.AddIntersection(name);
    }
    for (const auto &edge : edges) {
        m.AddStreet(edge.first, edge.second);
    }

    SECTION("removing streets") {
        SECTION("existing streets") {
            REQUIRE(m.Distance("A", "D") == 2);
            m.RemoveStreet("B", "D");
            REQUIRE(m.Distance("A", "D") == -1);
        }

        SECTION("non-existing streets") {
            REQUIRE(m.Distance("B", "C") == -1);
            m.RemoveStreet("B", "C");
            REQUIRE(m.Distance("B", "C") == -1);
        }
    }

    SECTION("reversing streets") {
        REQUIRE(m.Distance("A", "D") == 2);
        m.ReverseStreet("A", "D");
        REQUIRE(m.Distance("A", "D") == 2);

        m.ReverseStreet("D", "B");
        REQUIRE(m.Distance("A", "D") == -1);

        m.ReverseStreet("B", "D");
        REQUIRE(m.Distance("A", "D") == 2);
    }
}

TEST_CASE("Drivers work correctly")
{
    UberMap m;
    m.AddIntersection("A");

    m.AddDriver("A", 1);
    m.AddDriver("A", 2);

    auto drivers = m.ClosestDrivers("A");
    REQUIRE(drivers.size() == 2);
    REQUIRE(std::find(drivers.begin(), drivers.end(), 1) != drivers.end());
    REQUIRE(std::find(drivers.begin(), drivers.end(), 2) != drivers.end());

    SECTION("removing drivers") {
        SECTION("existing drivers") {
            m.RemoveDriver("A", 1);
            REQUIRE(m.ClosestDrivers("A").size() == 1);
        }
        SECTION("non-existing drivers") {
            m.RemoveDriver("A", 300);
            REQUIRE(m.ClosestDrivers("A").size() == 2);
        }
    }
}

TEST_CASE("Destination finding works correctly")
{
    UberMap m;
    m.AddIntersection("A");
    m.AddIntersection("B");
    m.AddIntersection("C");
    m.AddIntersection("D");

    m.AddStreet("A", "B");
    m.AddStreet("C", "B");
    m.AddStreet("D", "C");

    SECTION("source and destination are the same") {
        REQUIRE(m.Destination("A", "A") == "A");
    }
    SECTION("destination can be reached directly") {
        REQUIRE(m.Destination("A", "B") == "B");
    }
    SECTION("real destination is a neighbour of the wanted one") {
        REQUIRE(m.Destination("A", "C") == "B");
        REQUIRE(m.Destination("B", "A") == "B");
        REQUIRE(m.Destination("B", "C") == "B");
        REQUIRE(m.Destination("C", "D") == "C");
    }
    SECTION("cannot find a destination") {
        REQUIRE(m.Destination("A", "D") == "");
        REQUIRE(m.Destination("B", "D") == "");
    }
}

TEST_CASE("Closest drivers are found correctly")
{
    UberMap m;
    m.AddIntersection("A");
    m.AddIntersection("B");
    m.AddIntersection("C");
    m.AddIntersection("D");

    m.AddStreet("A", "B");
    m.AddStreet("B", "C");
    m.AddStreet("C", "D");

    m.AddDriver("B", 1);
    m.AddDriver("C", 2);

    SECTION("driver is at the destination") {
        REQUIRE(m.ClosestDrivers("B") == std::vector<int>{1});
        REQUIRE(m.ClosestDrivers("C") == std::vector<int>{2});
    }
    SECTION("only the closest drivers are returned") {
        REQUIRE(m.ClosestDrivers("D") == std::vector<int>{2});
    }
    SECTION("no available drivers") {
        REQUIRE(m.ClosestDrivers("A") == std::vector<int>());
    }
}
