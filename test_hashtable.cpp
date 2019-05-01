// Copyright 2019 Preda Andrei

#include <string>
#include <vector>

#include "catch.hpp"

#include "./hashtable.h"
#include "./hashtable_utils.h"

TEST_CASE("Hashtables work normally")
{
    Hashtable<int, int> hash(10);
    REQUIRE(hash.Size() == 0);

    SECTION("insertion works") {
        hash.Put(1, 3);

        REQUIRE(hash.Size() == 1);
        REQUIRE(hash.Get(1) == 3);
        REQUIRE(hash.Get(3) == 0);

        SECTION("duplicates are not allowed") {
            hash.Put(1, 5);
            REQUIRE(hash.Size() == 1);
            REQUIRE(hash.Get(1) == 5);
        }
    }
}

TEST_CASE("Hashtables resize properly")
{
    Hashtable<int, int> hash(2);
    for (auto i = 0; i < 1000; i += 1) {
        hash.Put(i, 2 * i);
    }

    REQUIRE(hash.Size() == 1000);
    for (auto i = 0; i < 1000; i += 1) {
        REQUIRE(hash.Get(i) == 2 * i);
    }
    REQUIRE(hash.Get(-1) == 0);
}

TEST_CASE("Default hash functions aren't too bad")
{
    SECTION("int hash is ok") {
        std::vector<int> count(10, 0);
        Hash<int> hash;

        for (auto i = 0; i < 50; i += 1) {
            count[hash(i) % count.size()] += 1;
        }
        for (const auto &num : count) {
            REQUIRE(num <= 50 / count.size() + 1);
        }
    }

    SECTION("string hash is ok") {
        std::vector<std::string> words = {
            "Ana", "are", "mere", "Maria", "ciuperci",
            "era", "mancare", "shupeta", "brclp", "pingpong",
            "strada", "alee", "bulevard", "Marcel", "Bogdan",
            "Anca", "Cristi", "Bulion", "Azalea"
        };

        std::vector<int> count(6, 0);
        Hash<std::string> hash;

        for (const auto &word : words) {
            count[hash(word) % count.size()] += 1;
        }
        for (const auto &num : count) {
            REQUIRE(num <= words.size() / count.size() + 1);
        }
    }
}
