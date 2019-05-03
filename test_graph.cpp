// Copyright 2019 Preda Andrei

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "./catch.hpp"

#include "./graph.h"

TEST_CASE("Graph has correct size")
{
    Graph<std::string> graph;
    REQUIRE(graph.Size() == 0);

    graph.AddNode("Ala");
    REQUIRE(graph.Size() == 1);

    graph.AddNode("Bala");
    REQUIRE(graph.Size() == 2);

    graph.AddNode("Portocala");
    REQUIRE(graph.Size() == 3);
}

TEST_CASE("Data is held correctly inside the nodes")
{
    std::vector<std::string> names = {
        "First", "Second", "Third", "Fourth"
    };

    Graph<std::string> graph;
    for (const auto &name : names) {
        graph.AddNode(name);
    }

    REQUIRE(graph.Size() == names.size());
    for (std::size_t i = 0; i < graph.Size(); i += 1) {
        REQUIRE(graph.Data(i) == names[i]);
    }

    SECTION("data can be modified") {
        for (std::size_t i = 0; i < graph.Size(); i += 1) {
            graph.Data(i) = names[names.size() - i - 1];
        }
        for (std::size_t i = 0; i < graph.Size(); i += 1) {
            REQUIRE(graph.Data(i) == names[names.size() - i - 1]);
        }
    }
}

TEST_CASE("Edge-operations work correctly")
{
    Graph<std::string> graph;
    graph.AddNode("A");
    graph.AddNode("B");
    graph.AddNode("C");

    graph.AddEdge(0, 1);
    REQUIRE(graph.Edges(0).size() == 1);
    REQUIRE(graph.Edges(0)[0] == 1);

    graph.AddEdge(1, 2);
    REQUIRE(graph.Edges(1).size() == 1);
    REQUIRE(graph.Edges(1)[0] == 2);

    graph.AddEdge(2, 0);
    graph.AddEdge(2, 1);
    REQUIRE(graph.Edges(2).size() == 2);
    REQUIRE(graph.Edges(2)[0] == 0);
    REQUIRE(graph.Edges(2)[1] == 1);

    SECTION("multiple edges are not allowed") {
        graph.AddEdge(0, 1);
        graph.AddEdge(0, 1);
        graph.AddEdge(0, 1);
        REQUIRE(graph.Edges(0).size() == 1);
        REQUIRE(graph.Edges(0)[0] == 1);
    }

    SECTION("self loops are allowed") {
        graph.AddEdge(0, 0);
        graph.AddEdge(0, 0);
        REQUIRE(graph.Edges(0).size() == 2);
        REQUIRE(graph.Edges(0)[1] == 0);
    }

    SECTION("edges are removed correctly") {
        SECTION("remove existing edges") {
            graph.RemoveEdge(0, 1);

            REQUIRE(graph.Edges(0).size() == 0);
            REQUIRE(graph.Edges(1).size() == 1);
            REQUIRE(graph.Edges(2).size() == 2);
        }

        SECTION("remove non-existing edges") {
            graph.RemoveEdge(0, 2);

            REQUIRE(graph.Edges(0).size() == 1);
            REQUIRE(graph.Edges(0)[0] == 1);

            REQUIRE(graph.Edges(2).size() == 2);
            REQUIRE(graph.Edges(2)[0] == 0);
            REQUIRE(graph.Edges(2)[1] == 1);
        }
    }

    SECTION("edges are reversed correctly") {
        graph.ReverseEdge(0, 1);
        REQUIRE(graph.Edges(0).size() == 0);
        REQUIRE(graph.Edges(1).size() == 2);

        graph.ReverseEdge(1, 0);
        REQUIRE(graph.Edges(0).size() == 1);
        REQUIRE(graph.Edges(1).size() == 1);

        SECTION("nodes are not connected initally") {
            graph.RemoveEdge(0, 1);
            graph.ReverseEdge(0, 1);

            REQUIRE(graph.Edges(0).size() == 0);
            REQUIRE(graph.Edges(1).size() == 1);
        }
        SECTION("nodes are connected both ways") {
            graph.AddEdge(1, 0);
            graph.ReverseEdge(0, 1);

            REQUIRE(graph.Edges(0).size() == 1);
            REQUIRE(graph.Edges(1).size() == 2);
        }
    }
}

TEST_CASE("Transposed works correctly")
{
    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {1, 2}, {2, 0}, {2, 1}
    };

    Graph<std::string> graph;
    for (auto i = 0; i < 3; i += 1) {
        graph.AddNode("A");
    }
    for (const auto &p : edges) {
        graph.AddEdge(p.first, p.second);
    }

    auto trans = graph.Transposed();
    REQUIRE(trans.Size() == graph.Size());

    REQUIRE(trans.Edges(0).size() == 1);
    REQUIRE(trans.Edges(1).size() == 2);
    REQUIRE(trans.Edges(2).size() == 1);

    const auto &edges1 = trans.Edges(1);
    REQUIRE(find(edges1.begin(), edges1.end(), 0) != edges1.end());
    REQUIRE(find(edges1.begin(), edges1.end(), 2) != edges1.end());
    REQUIRE(find(edges1.begin(), edges1.end(), 5) == edges1.end());
}
