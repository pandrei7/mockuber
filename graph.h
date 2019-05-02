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

    void ToggleEdge(int a, int b)
    {
        auto &edges = nodes_[a].edges;
        auto it = find(edges.begin(), edges.end(), b);

        if (it == edges.end()) {
            edges.push_back(b);
        } else {
            edges.erase(it);
        }
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

    const std::vector<int>& Edges(int index) const
    {
        return nodes_[index].edges;
    }

 private:
    std::vector<Node> nodes_;
};

#endif  // GRAPH_H_
