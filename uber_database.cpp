// Copyright 2019 Preda Andrei

#include "./uber_database.h"

#include <algorithm>
#include <list>
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

UberDriver UberDatabase::Driver(const std::string &name) const
{
    auto id = ids_.Get(name);
    return drivers_[id];
}

void UberDatabase::AddToSortings(int id)
{
    auto name = drivers_[id].Name();
    by_rating_.Add(std::make_pair(-drivers_[id].Rating(), name));
    by_dist_.Add(std::make_pair(-drivers_[id].Distance(), name));
    by_trips_.Add(std::make_pair(-drivers_[id].Trips(), name));
}

void UberDatabase::RemoveFromSortings(int id)
{
    auto name = drivers_[id].Name();
    by_rating_.Remove(std::make_pair(-drivers_[id].Rating(), name));
    by_dist_.Remove(std::make_pair(-drivers_[id].Distance(), name));
    by_trips_.Remove(std::make_pair(-drivers_[id].Trips(), name));
}

void UberDatabase::GoOnline(const std::string &name,
                            const std::string &location)
{
    auto id = Id(name);
    if (id == -1) {
        id = ids_.Size();
        ids_.Put(name, id);
        drivers_.push_back(UberDriver(name));
        AddToSortings(id);
    }

    drivers_[id].GoOnline(location);
}

void UberDatabase::GoOffline(const std::string &name)
{
    drivers_[ids_.Get(name)].GoOffline();
}

void UberDatabase::MakeTrip(const std::string &name, const std::string &dest,
                                                int distance, double rating)
{
    auto id = ids_.Get(name);

    RemoveFromSortings(id);
    drivers_[id].MakeTrip(dest, distance, rating);
    AddToSortings(id);
}

static bool BetterDriver(const UberDriver &a, const UberDriver &b)
{
    if (std::abs(a.Rating() - b.Rating()) < 0.0001) {
        return a.Name() < b.Name();
    }
    return a.Rating() > b.Rating();
}

std::string UberDatabase::BestDriver(const std::vector<int> &ids) const
{
    std::string best_name = "";
    int best_id = -1;

    for (const auto &id : ids) {
        if (!drivers_[id].Online()) {
            continue;
        }
        if (best_id == -1 || BetterDriver(drivers_[id], drivers_[best_id])) {
            best_name = drivers_[id].Name();
            best_id = id;
        }
    }
    return best_name;
}

std::vector<UberDriver> UberDatabase::SortedDrivers(const std::string &mode,
                                                    std::size_t count) const
{
    std::vector<std::pair<double, std::string>> sorted;
    if (mode == "top_rating") {
        sorted = by_rating_.Get(count);
    } else if (mode == "top_dist") {
        sorted = by_dist_.Get(count);
    } else {
        sorted = by_trips_.Get(count);
    }

    std::vector<UberDriver> drivers;
    for (const auto &p : sorted) {
        auto id = ids_.Get(p.second);
        drivers.push_back(drivers_[id]);
    }
    return drivers;
}
