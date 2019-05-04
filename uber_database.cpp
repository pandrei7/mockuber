// Copyright 2019 Preda Andrei

#include "./uber_database.h"

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "./algorithms.h"

static bool CmpRating(const UberDriver &a, const UberDriver &b)
{
    if (std::abs(a.Rating() - b.Rating()) < 0.0001) {
        return a.Name() < b.Name();
    }
    return a.Rating() > b.Rating();
}

static bool CmpDistance(const UberDriver &a, const UberDriver &b)
{
    if (a.Distance() == b.Distance()) {
        return a.Name() < b.Name();
    }
    return a.Distance() > b.Distance();
}

static bool CmpTrips(const UberDriver &a, const UberDriver &b)
{
    if (a.Trips() == b.Trips()) {
        return a.Name() < b.Name();
    }
    return a.Trips() > b.Trips();
}

UberDatabase::UberDatabase()
{
    sorted_ids_.Put("top_rating", std::vector<int>());
    sorted_ids_.Put("top_dist", std::vector<int>());
    sorted_ids_.Put("top_rides", std::vector<int>());

    cmp_funcs_.Put("top_rating", CmpRating);
    cmp_funcs_.Put("top_dist", CmpDistance);
    cmp_funcs_.Put("top_rides", CmpTrips);
}

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

void UberDatabase::Reorder(std::vector<int> &sorted, int id, CmpFunc cmp)
{
    auto driver = drivers_[id];
    std::size_t pos = find(sorted.begin(), sorted.end(), id) - sorted.begin();

    while (pos > 0 && cmp(driver, drivers_[sorted[pos - 1]])) {
        std::swap(sorted[pos], sorted[pos - 1]);
        pos -= 1;
    }
    while (pos + 1 < sorted.size() && cmp(drivers_[sorted[pos + 1]], driver)) {
        std::swap(sorted[pos], sorted[pos + 1]);
        pos += 1;
    }
}

void UberDatabase::UpdateSortings(int id)
{
    Reorder(sorted_ids_.GetRef("top_rating"), id, cmp_funcs_.Get("top_rating"));
    Reorder(sorted_ids_.GetRef("top_dist"), id, cmp_funcs_.Get("top_dist"));
    Reorder(sorted_ids_.GetRef("top_rides"), id, cmp_funcs_.Get("top_rides"));
}

void UberDatabase::GoOnline(const std::string &name,
                            const std::string &location)
{
    auto id = Id(name);
    if (id == -1) {
        id = ids_.Size();
        ids_.Put(name, id);
        drivers_.push_back(UberDriver(name));

        sorted_ids_.GetRef("top_rating").push_back(id);
        sorted_ids_.GetRef("top_dist").push_back(id);
        sorted_ids_.GetRef("top_rides").push_back(id);
    }

    drivers_[id].GoOnline(location);
    UpdateSortings(id);
}

void UberDatabase::GoOffline(const std::string &name)
{
    drivers_[ids_.Get(name)].GoOffline();
}

void UberDatabase::MakeTrip(const std::string &name, const std::string &dest,
                                                int distance, double rating)
{
    drivers_[ids_.Get(name)].MakeTrip(dest, distance, rating);
    UpdateSortings(ids_.Get(name));
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
    const auto &sorted = sorted_ids_.Get(mode);
    std::vector<UberDriver> drivers;

    for (std::size_t i = 0; i < count && i < sorted.size(); i += 1) {
        drivers.push_back(drivers_[sorted[i]]);
    }
    return drivers;
}
