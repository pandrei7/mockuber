// Copyright 2019 Preda Andrei

#include "./catch.hpp"

#include "./uber_app.h"

TEST_CASE("Distances work correctly")
{
    UberApp a;
    a.AddIntersection("A");
    a.AddIntersection("B");
    a.AddIntersection("C");

    a.AddStreet("A", "B");
    a.AddStreet("B", "C");
    a.AddStreet("C", "B");

    SECTION("simple distances") {
        REQUIRE(a.Distance("A", "B") == 1);
        REQUIRE(a.Distance("A", "C") == 2);
        REQUIRE(a.Distance("C", "A") == -1);
        REQUIRE(a.Distance("C", "B") == 1);
        REQUIRE(a.Distance("B", "B") == 0);
    }
    SECTION("sorting by distance") {
        auto sorted = a.SortedByDistance("B", {"B", "C", "B", "A", "C"}, 3);
        REQUIRE(sorted.size() == 2);
        REQUIRE(sorted[0] == "B");
        REQUIRE(sorted[1] == "C");
    }
}

TEST_CASE("Street operations work correctly")
{
    UberApp a;
    a.AddIntersection("A");
    a.AddIntersection("B");
    a.AddIntersection("C");

    a.AddStreet("A", "B");
    REQUIRE(a.Distance("A", "B") == 1);
    REQUIRE(a.Distance("B", "A") == -1);

    a.AddStreet("B", "C");
    REQUIRE(a.Distance("A", "C") == 2);

    a.ReverseStreet("A", "B");
    REQUIRE(a.Distance("A", "C") == -1);
    REQUIRE(a.Distance("B", "A") == 1);

    a.ReverseStreet("A", "B");
    a.RemoveStreet("B", "C");
    REQUIRE(a.Distance("A", "C") == -1);

    a.ReverseStreet("B", "C");
    REQUIRE(a.Distance("A", "C") == -1);
}

TEST_CASE("Drivers work correctly")
{
    UberApp a;
    a.AddIntersection("A");
    a.AddIntersection("B");
    a.AddIntersection("C");

    a.AddStreet("A", "B");
    a.AddStreet("C", "A");

    a.GoOnline("Enrique", "A");
    REQUIRE(a.Driver("Enrique").Name() == "Enrique");
    REQUIRE(a.Driver("Enrique").Location() == "A");
    REQUIRE(a.Driver("Enrique").Online());

    a.GoOffline("Enrique");
    REQUIRE(!a.Driver("Enrique").Online());
    REQUIRE(a.Driver("Enrique").Location() == "A");

    a.GoOnline("Enrique", "A");

    a.MakeTrip("A", "B", 3);
    REQUIRE(a.Driver("Enrique").Location() == "B");
    REQUIRE(a.Driver("Enrique").Trips() == 1);
    REQUIRE(a.Driver("Enrique").Rating() == 3);

    a.MakeTrip("B", "A", 5);
    REQUIRE(a.Driver("Enrique").Location() == "B");
    REQUIRE(a.Driver("Enrique").Trips() == 2);
    REQUIRE(a.Driver("Enrique").Rating() == 4);

    a.MakeTrip("B", "C", 5);
    REQUIRE(a.Driver("Enrique").Location() == "B");
    REQUIRE(a.Driver("Enrique").Trips() == 2);
    REQUIRE(a.Driver("Enrique").Rating() == 4);
}

TEST_CASE("Sortings work correctly")
{
    UberApp a;
    a.AddIntersection("A");
    a.AddIntersection("B");
    a.AddIntersection("C");
    a.AddIntersection("D");

    a.AddStreet("A", "B");
    a.AddStreet("B", "D");
    a.AddStreet("B", "C");

    a.GoOnline("Manole", "C");
    a.GoOnline("Enrique", "A");

    REQUIRE(a.MakeTrip("B", "D", 3) == "");
    REQUIRE(a.MakeTrip("C", "C", 3) == "");
    REQUIRE(a.MakeTrip("C", "C", 5) == "");
    REQUIRE(a.MakeTrip("A", "B", 1) == "Soferi indisponibili");

    SECTION("sorting by rating") {
        auto sorted = a.SortedDrivers("top_rating");
        REQUIRE(sorted.size() == 2);
        REQUIRE(sorted[0].Name() == "Manole");
        REQUIRE(sorted[1].Name() == "Enrique");
    }
    SECTION("sorting by distance") {
        auto sorted = a.SortedDrivers("top_dist");
        REQUIRE(sorted.size() == 2);
        REQUIRE(sorted[0].Name() == "Enrique");
        REQUIRE(sorted[1].Name() == "Manole");
    }
    SECTION("sorting by trips") {
        auto sorted = a.SortedDrivers("top_rides");
        REQUIRE(sorted.size() == 2);
        REQUIRE(sorted[0].Name() == "Manole");
        REQUIRE(sorted[1].Name() == "Enrique");
    }
    SECTION("sorting by distance") {
        auto sorted = a.SortedByDistance("B", {"A", "B", "C", "B", "D"}, 4);
        REQUIRE(sorted.size() == 3);
        REQUIRE(sorted[0] == "B");
        REQUIRE(sorted[1] == "C");
        REQUIRE(sorted[2] == "D");
    }
}
