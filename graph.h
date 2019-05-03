// Copyright 2019 Preda Andrei

#ifndef GRAPH_H_
#define GRAPH_H_

#include <algorithm>
#include <cstddef>
#include <vector>

template <typename T>
class Graph
{
 public:
    struct Node
    {
        T data;
        std::vector<int> edges;

        explicit Node(const T &data) : data(data) {}
    };

    void AddNode(const T &data)
    {
        nodes_.push_back(Node(data));
    }

    bool AddEdge(int a, int b)
    {
        auto &edges = nodes_[a].edges;
        auto it = find(edges.begin(), edges.end(), b);

        if (it == edges.end()) {
            edges.push_back(b);
            return true;
        }
        return false;
    }

    bool RemoveEdge(int a, int b)
    {
        auto &edges = nodes_[a].edges;
        auto it = find(edges.begin(), edges.end(), b);

        if (it != edges.end()) {
            edges.erase(it);
            return true;
        }
        return false;
    }

    bool ReverseEdge(int a, int b)
    {
        auto &edges_a = nodes_[a].edges;
        auto &edges_b = nodes_[b].edges;

        auto has_ab = find(edges_a.begin(), edges_a.end(), b) != edges_a.end();
        auto has_ba = find(edges_b.begin(), edges_b.end(), a) != edges_b.end();

        if (a == b) {
            return (has_ab ? RemoveEdge(a, b) : AddEdge(a, b));
        }

        if (has_ab == has_ba) {
            return false;
        }

        if (has_ab) {
            RemoveEdge(a, b);
            AddEdge(b, a);
        } else {
            RemoveEdge(b, a);
            AddEdge(a, b);
        }
        return true;
    }

    Graph Transposed() const
    {
        Graph trans;
        for (const auto &node : nodes_) {
            trans.AddNode(node.data);
        }

        for (std::size_t i = 0; i < nodes_.size(); i += 1) {
            for (const auto &other : nodes_[i].edges) {
                trans.AddEdge(other, i);
            }
        }
        return trans;
    }

    std::size_t Size() const
    {
        return nodes_.size();
    }

    T& Data(int index)
    {
        return nodes_[index].data;
    }

    const T& Data(int index) const
    {
        return nodes_[index].data;
    }

    const std::vector<int>& Edges(int index) const
    {
        return nodes_[index].edges;
    }

 private:
    std::vector<Node> nodes_;
};

#endif  // GRAPH_H_
