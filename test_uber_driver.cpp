// Copyright 2019 Preda Andrei

#include "./catch.hpp"

#include "./uber_driver.h"

TEST_CASE("UberDriver works correctly")
{
    UberDriver d("Enrique");
    REQUIRE(d.Name() == "Enrique");
    REQUIRE(d.Rating() == 0);
    REQUIRE(d.Location() == "");
    REQUIRE(d.Trips() == 0);
    REQUIRE(d.Distance() == 0);
    REQUIRE(!d.Online());

    d.GoOnline("Cuca Macaii");
    REQUIRE(d.Online());
    REQUIRE(d.Trips() == 0);
    REQUIRE(d.Location() == "Cuca Macaii");

    d.GoOnline("Pana la baie");
    REQUIRE(d.Online());
    REQUIRE(d.Location() == "Pana la baie");

    d.GoOffline();
    REQUIRE(!d.Online());
    REQUIRE(d.Location() == "Pana la baie");

    d.GoOnline("La rascruce de vanturi");

    SECTION("check trips") {
        d.MakeTrip("Acasa la Enrique", 10, 2.25);
        REQUIRE(d.Location() == "Acasa la Enrique");
        REQUIRE(d.Distance() == 10);
        REQUIRE(d.Trips() == 1);
        REQUIRE(d.Rating() == 2.25);

        d.MakeTrip("Pe camp", 20, 4.25);
        REQUIRE(d.Location() == "Pe camp");
        REQUIRE(d.Distance() == 30);
        REQUIRE(d.Trips() == 2);
        REQUIRE(d.Rating() == 3.25);
    }
}
