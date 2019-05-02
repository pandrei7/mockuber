// Copyright 2019 Preda Andrei

#include "./uber_node.h"

#include <algorithm>

void UberNode::AddDriver(int id)
{
    auto it = std::find(drivers_.begin(), drivers_.end(), id);

    if (it == drivers_.end()) {
        drivers_.push_back(id);
    }
}

void UberNode::RemoveDriver(int id)
{
    auto &vec = drivers_;
    vec.erase(std::remove(vec.begin(), vec.end(), id), vec.end());
}
