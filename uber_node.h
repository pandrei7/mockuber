// Copyright 2019 Preda Andrei

#ifndef UBER_NODE_H_
#define UBER_NODE_H_

#include <string>
#include <vector>

// This class represents an intersection for the Uber app.
// It has a name, as well as a list of the ids of the drivers who are
// found in the intersection, at a certain time.
class UberNode
{
 public:
    explicit UberNode(const std::string &name) : name_(name) {}

    void AddDriver(int id);

    void RemoveDriver(int id);

    const std::string& Name() const
    {
        return name_;
    }

    const std::vector<int>& Drivers() const
    {
        return drivers_;
    }

 private:
    std::string name_;
    std::vector<int> drivers_;
};

#endif  // UBER_NODE_H_
