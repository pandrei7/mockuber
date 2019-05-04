// Copyright 2019 Preda Andrei

#ifndef UBER_MAP_H_
#define UBER_MAP_H_

#include <string>
#include <vector>

#include "./graph.h"
#include "./hashtable.h"
#include "./uber_node.h"

class UberMap
{
 public:
    UberMap() : changed_(false) {}

    int Id(const std::string &name) const;

    void AddIntersection(const std::string &name);

    void AddStreet(const std::string &a, const std::string &b);

    void RemoveStreet(const std::string &a, const std::string &b);

    void ReverseStreet(const std::string &a, const std::string &b);

    void AddDriver(const std::string &name, int driver_id);

    void RemoveDriver(const std::string &name, int driver_id);

    void SaveDistances(bool status);

    std::vector<int> AllDistances(const std::string &name);

    int Distance(const std::string &source, const std::string &dest);

    std::string Destination(const std::string &source, const std::string &dest);

    std::vector<int> ClosestDrivers(const std::string &name);

 private:
    Graph<UberNode> graph_;
    Graph<UberNode> trans_;

    Hashtable<std::string, int> ids_;
    std::vector<std::vector<int>> dists_;

    bool changed_;
    bool save_dists_;
};

#endif  // UBER_MAP_H_
