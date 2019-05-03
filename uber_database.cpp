// Copyright 2019 Preda Andrei

#include "./uber_database.h"

#include <string>
#include <utility>
#include <vector>

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

static int Partition(std::vector<UberDriver> &vec,
                     int left, int right, const UberDatabase::CmpFunc &cmp)
{
    auto mid = left + (right - left) / 2;
    std::swap(vec[left], vec[mid]);

    auto i = left + 1;
    auto j = right - 1;

    while (i <= j) {
        while (i <= j && cmp(vec[i], vec[left])) {
            i += 1;
        }
        while (i <= j && cmp(vec[left], vec[j])) {
            j -= 1;
        }
        if (i <= j) {
            std::swap(vec[i], vec[j]);
            i += 1;
            j -= 1;
        }
    }

    std::swap(vec[left], vec[i - 1]);
    return i - 1;
}

static void Quicksort(std::vector<UberDriver> &vec,
                      int left, int right, const UberDatabase::CmpFunc &cmp)
{
    if (left < right) {
        auto pos = Partition(vec, left, right, cmp);
        Quicksort(vec, left, pos, cmp);
        Quicksort(vec, pos + 1, right, cmp);
    }
}

std::vector<UberDriver> UberDatabase::SortedDrivers(const CmpFunc &cmp) const
{
    std::vector<UberDriver> drivers(drivers_);
    if (cmp) {
        Quicksort(drivers, 0, drivers.size(), cmp);
    }

    return drivers;
}
