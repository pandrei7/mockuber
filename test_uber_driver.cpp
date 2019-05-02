// Copyright 2019 Preda Andrei

#include "./catch.hpp"

#include "./uber_driver.h"

TEST_CASE("UberDriver works correctly")
{
    UberDriver d("Enrique");
    REQUIRE(d.Name() == "Enrique");
    REQUIRE(d.Rating() == 0);
    REQUIRE(d.Location() == -1);
    REQUIRE(d.Trips() == 0);
    REQUIRE(d.Distance() == 0);
    REQUIRE(!d.Online());

    d.GoOnline(1);
    REQUIRE(d.Online());
    REQUIRE(d.Trips() == 0);
    REQUIRE(d.Location() == 1);

    d.GoOnline(2);
    REQUIRE(d.Online());
    REQUIRE(d.Location() == 2);

    d.GoOffline();
    REQUIRE(!d.Online());
    REQUIRE(d.Location() == 2);

    d.GoOnline(3);

    SECTION("check trips") {
        d.MakeTrip(1, 10, 2.25);
        REQUIRE(d.Location() == 1);
        REQUIRE(d.Distance() == 10);
        REQUIRE(d.Trips() == 1);
        REQUIRE(d.Rating() == 2.25);

        d.MakeTrip(2, 20, 4.25);
        REQUIRE(d.Location() == 2);
        REQUIRE(d.Distance() == 30);
        REQUIRE(d.Trips() == 2);
        REQUIRE(d.Rating() == 3.25);
    }
}
