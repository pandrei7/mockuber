// Copyright 2019 Preda Andrei

#include "./uber_map.h"

#include <queue>
#include <string>
#include <vector>

int UberMap::Id(const std::string &name) const
{
    if (!ids_.HasKey(name)) {
        return -1;
    }
    return ids_.Get(name);
}

void UberMap::AddIntersection(const std::string &name)
{
    if (!ids_.HasKey(name)) {
        ids_.Put(name, ids_.Size());
        graph_.AddNode(UberNode(name));
        changed_ = true;
    }
}

void UberMap::AddStreet(const std::string &a, const std::string &b)
{
    auto id_a = ids_.Get(a);
    auto id_b = ids_.Get(b);

    if (graph_.AddEdge(id_a, id_b)) {
        changed_ = true;
    }
}

void UberMap::RemoveStreet(const std::string &a, const std::string &b)
{
    auto id_a = ids_.Get(a);
    auto id_b = ids_.Get(b);

    if (graph_.RemoveEdge(id_a, id_b)) {
        changed_ = true;
    }
}

void UberMap::ReverseStreet(const std::string &a, const std::string &b)
{
    auto id_a = ids_.Get(a);
    auto id_b = ids_.Get(b);

    if (graph_.ReverseEdge(id_a, id_b)) {
        changed_ = true;
    }
}

void UberMap::AddDriver(const std::string &name, int driver_id)
{
    auto id = ids_.Get(name);
    graph_.Data(id).AddDriver(driver_id);
}

void UberMap::RemoveDriver(const std::string &name, int driver_id)
{
    auto id = ids_.Get(name);
    graph_.Data(id).RemoveDriver(driver_id);
}

// The distances are computed using a simple Bfs algorithm.
std::vector<int> UberMap::AllDistances(const std::string &name) const
{
    auto id = ids_.Get(name);

    std::vector<int> dist(graph_.Size(), -1);
    dist[id] = 0;

    std::queue<int> q;
    q.push(id);

    while (!q.empty()) {
        id = q.front();
        q.pop();

        for (const auto &next : graph_.Edges(id)) {
            if (dist[next] == -1) {
                dist[next] = dist[id] + 1;
                q.push(next);
            }
        }
    }
    return dist;
}

int UberMap::Distance(const std::string &source, const std::string &dest) const
{
    auto id_dest = ids_.Get(dest);
    return AllDistances(source)[id_dest];
}

std::string UberMap::Destination(const std::string &source,
                                 const std::string &dest) const
{
    auto id_dest = ids_.Get(dest);
    auto dist = AllDistances(source);

    // If the desired destination is reachable, return its name.
    if (dist[id_dest] != -1) {
        return dest;
    }

    // If the desired destination is unreachable, try to reach
    // one of its neighbours, picking the closest one.
    auto best_id = -1;
    for (const auto &next : graph_.Edges(id_dest)) {
        if (dist[next] == -1) {
            continue;
        }
        if (best_id == -1 || dist[next] < dist[best_id]) {
            best_id = next;
        }
    }

    return (best_id == -1 ? "" : graph_.Data(best_id).Name());
}

// In order to find the closest drivers, this method uses the Bfs
// algorithm on the transpose of the graph.
std::vector<int> UberMap::ClosestDrivers(const std::string &name)
{
    // Only compute the transpose if needed.
    if (changed_) {
        trans_ = graph_.Transposed();
        changed_ = false;
    }

    std::vector<int> dist(trans_.Size(), -1);
    dist[ids_.Get(name)] = 0;

    std::queue<int> q;
    q.push(ids_.Get(name));

    auto driver_dist = -1;
    std::vector<int> drivers;

    while (!q.empty()) {
        auto id = q.front();
        q.pop();

        // Don't search the nodes past the closest level that has drivers.
        if (driver_dist > -1 && dist[id] > driver_dist) {
            continue;
        }

        // Add the drivers found at the current node to the returned list.
        if (!graph_.Data(id).Drivers().empty()) {
            driver_dist = dist[id];  // Save the shortest distance to a driver.

            const auto &vec = graph_.Data(id).Drivers();
            drivers.insert(drivers.end(), vec.begin(), vec.end());
            continue;
        }

        for (const auto &next : trans_.Edges(id)) {
            if (dist[next] == -1) {
                dist[next] = dist[id] + 1;
                q.push(next);
            }
        }
    }
    return drivers;
}
