// Copyright 2019 Preda Andrei

#include "./uber_database.h"

#include <string>
#include <utility>
#include <vector>

#include "./algorithms.h"

int UberDatabase::Id(const std::string &name) const
{
    if (!ids_.HasKey(name)) {
        return -1;
    }
    return ids_.Get(name);
}

int UberDatabase::Location(const std::string &name) const
{
    return drivers_[ids_.Get(name)].Location();
}

void UberDatabase::GoOnline(const std::string &name, int location)
{
    auto id = Id(name);
    if (id == -1) {
        id = ids_.Size();
        ids_.Put(name, id);
        drivers_.push_back(UberDriver(name));
    }

    drivers_[id].GoOnline(location);
}

void UberDatabase::GoOffline(const std::string &name)
{
    drivers_[ids_.Get(name)].GoOffline();
}

void UberDatabase::MakeTrip(const std::string &name, int dest,
                            int distance, double rating)
{
    drivers_[ids_.Get(name)].MakeTrip(dest, distance, rating);
}

std::vector<UberDriver> UberDatabase::SortedDrivers(const CmpFunc &cmp) const
{
    std::vector<UberDriver> drivers(drivers_);
    if (cmp) {
        Quicksort(drivers, 0, drivers.size(), cmp);
    }

    return drivers;
}
