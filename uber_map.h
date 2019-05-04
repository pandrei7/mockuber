// Copyright 2019 Preda Andrei

#ifndef UBER_MAP_H_
#define UBER_MAP_H_

#include <string>
#include <vector>

#include "./graph.h"
#include "./hashtable.h"
#include "./uber_node.h"

// This class represents the city map used by the Uber app.
// The intersections and streets are represented as a directed graph.
// The class also has a copy of the transpose of this graph, which is
// used for quickly finding available drivers. The transpose is only
// constructed when needed. Because of this, the map also remembers
// if the normal graph has changed since the last transpose construction.
class UberMap
{
 public:
    UberMap() : changed_(false) {}

    // This method returns the id of an intersection, given its name.
    int Id(const std::string &name) const;

    void AddIntersection(const std::string &name);

    void AddStreet(const std::string &a, const std::string &b);

    void RemoveStreet(const std::string &a, const std::string &b);

    void ReverseStreet(const std::string &a, const std::string &b);

    void AddDriver(const std::string &name, int driver_id);

    void RemoveDriver(const std::string &name, int driver_id);

    // This method returns a list of the distances from a given
    // intersection, to all the others.
    std::vector<int> AllDistances(const std::string &name) const;

    // This methods returns the distance between two intersections.
    // If there is not path available, it returns -1.
    int Distance(const std::string &source, const std::string &dest) const;

    // This method returns a reachable destination, given data about a desired
    // trip. If no destination is reachable, it returns an emtpy string.
    std::string Destination(const std::string &source,
                            const std::string &dest) const;

    // This method returns a list of ids of the drivers who are closest
    // to a given intersection (the drivers can reach the intersection).
    std::vector<int> ClosestDrivers(const std::string &name);

 private:
    Graph<UberNode> graph_;
    Graph<UberNode> trans_;

    Hashtable<std::string, int> ids_;
    bool changed_;
};

#endif  // UBER_MAP_H_
