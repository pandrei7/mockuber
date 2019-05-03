// Copyright 2019 Preda Andrei

#include <cstdlib>
#include <string>
#include <vector>

#include "./catch.hpp"

#include "./uber_database.h"
#include "./uber_driver.h"

TEST_CASE("UberDatabase gives correct driver data")
{
    UberDatabase db;
    db.GoOnline("Mircea Badea", "Antena");
    db.GoOnline("Mihai Viteazul", "Alba Iulia");
    db.GoOnline("Chef Florin", "Paris");

    SECTION("ids of existing users") {
        REQUIRE(db.Id("Mircea Badea") == 0);
        REQUIRE(db.Id("Mihai Viteazul") == 1);
        REQUIRE(db.Id("Chef Florin") == 2);
    }
    SECTION("ids of non-existing users") {
        REQUIRE(db.Id("Chef Sorin") == -1);
    }
    SECTION("driver object") {
        auto driver = db.Driver("Chef Florin");
        REQUIRE(driver.Name() == "Chef Florin");
        REQUIRE(driver.Location() == "Paris");
        REQUIRE(driver.Rating() == 0);
        REQUIRE(driver.Distance() == 0);
        REQUIRE(driver.Trips() == 0);
        REQUIRE(driver.Online());
    }
}

static bool SmallerName(const UberDriver &a, const UberDriver &b)
{
    return a.Name() < b.Name();
}

static bool GreaterName(const UberDriver &a, const UberDriver &b)
{
    return a.Name() < b.Name();
}

static bool ComplexCmp(const UberDriver &a, const UberDriver &b)
{
    if (std::abs(a.Rating() - b.Rating()) < 0.01) {
        return a.Name() < b.Name();
    }
    return a.Rating() > b.Rating();
}

TEST_CASE("Sorting gives the right order")
{
    UberDatabase db;
    db.GoOnline("Mirciulica", "B1");
    db.GoOnline("Alexandra", "A2");
    db.GoOnline("Tezeu", "C3");
    db.GoOnline("Pilaf", "D4");
    db.GoOnline("Zaraza", "Circul de stat");
    db.GoOnline("Taximetrist craiovean", "Unde vrea el");

    std::vector<UberDatabase::CmpFunc> funcs = {
        SmallerName, GreaterName, ComplexCmp
    };

    for (const auto &cmp : funcs) {
        auto sorted = db.SortedDrivers(cmp);
        REQUIRE(sorted.size() == 6);

        for (std::size_t i = 1; i < sorted.size(); i += 1) {
            REQUIRE(cmp(sorted[i - 1], sorted[i]));
        }
    }
}

TEST_CASE("Sorting with nullptr does not change the order")
{
    UberDatabase db;
    db.GoOnline("B", "1");
    db.GoOnline("C", "2");
    db.GoOnline("A", "3");

    auto sorted = db.SortedDrivers(nullptr);
    REQUIRE(sorted.size() == 3);
    REQUIRE(sorted[0].Name() == "B");
    REQUIRE(sorted[1].Name() == "C");
    REQUIRE(sorted[2].Name() == "A");
}
