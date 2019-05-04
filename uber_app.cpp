// Copyright 2019 Preda Andrei

#include "./uber_app.h"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "./algorithms.h"

void UberApp::AddIntersection(const std::string &name)
{
    map_.AddIntersection(name);
}

void UberApp::AddStreet(const std::string &a, const std::string &b)
{
    map_.AddStreet(a, b);
}

void UberApp::RemoveStreet(const std::string &a, const std::string &b)
{
    map_.RemoveStreet(a, b);
}

void UberApp::ReverseStreet(const std::string &a, const std::string &b)
{
    map_.ReverseStreet(a, b);
}

int UberApp::Distance(const std::string &source, const std::string &dest) const
{
    return map_.Distance(source, dest);
}

UberDriver UberApp::Driver(const std::string &name) const
{
    return database_.Driver(name);
}

void UberApp::GoOnline(const std::string &driver, const std::string &location)
{
    auto id_driver = database_.Id(driver);

    if (id_driver != -1) {
        auto prev_location = database_.Driver(driver).Location();
        map_.RemoveDriver(prev_location, id_driver);
    }

    database_.GoOnline(driver, location);
    map_.AddDriver(location, database_.Id(driver));
}

void UberApp::GoOffline(const std::string &driver)
{
    auto location = database_.Driver(driver).Location();
    auto id_driver = database_.Id(driver);

    database_.GoOffline(driver);
    map_.RemoveDriver(location, id_driver);
}

std::string UberApp::MakeTrip(const std::string &source,
                              const std::string &dest, double rating)
{
    auto ids_drivers = map_.ClosestDrivers(source);
    if (ids_drivers.empty()) {
        return "Soferi indisponibili";
    }

    // Choose the best driver out of the closest ones.
    auto real_driver = database_.BestDriver(ids_drivers);
    auto real_driver_id = database_.Id(real_driver);
    auto real_start = database_.Driver(real_driver).Location();

    auto real_dest = map_.Destination(source, dest);
    if (real_dest.empty()) {
        return "Destinatie inaccesibila";
    }

    auto dist = map_.Distance(real_start, source) +
                map_.Distance(source, real_dest);

    map_.RemoveDriver(real_start, real_driver_id);
    map_.AddDriver(real_dest, real_driver_id);
    database_.MakeTrip(real_driver, real_dest, dist, rating);

    return "";  // The trip was made successfully.
}

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

std::vector<UberDriver> UberApp::SortedDrivers(const std::string &mode) const
{
    UberDatabase::CmpFunc cmp = nullptr;
    if (mode == "top_rating") {
        cmp = CmpRating;
    } else if (mode == "top_dist") {
        cmp = CmpDistance;
    } else if (mode == "top_rides") {
        cmp = CmpTrips;
    }

    return database_.SortedDrivers(cmp);
}

static bool CmpIntersections(const std::pair<std::string, int> &a,
                             const std::pair<std::string, int> &b)
{
    if (a.second == b.second) {
        return a.first < b.first;
    }
    return a.second < b.second;
}

std::vector<std::string> UberApp::SortedByDistance(const std::string &source,
                    const std::vector<std::string> &dest, int max_dist) const
{
    auto dist = map_.AllDistances(source);
    std::vector<std::pair<std::string, int>> pairs;

    for (const auto &name : dest) {
        auto id = map_.Id(name);
        // Only intersection that can be reached in a certain
        // number of steps are considered.
        if (dist[id] != -1 && dist[id] <= max_dist) {
            pairs.push_back({name, dist[id]});
            dist[id] = -1;  // Prevent insertion of duplicates.
        }
    }

    // Sort the intersections in ascending order by distance.
    Quicksort(pairs, 0, pairs.size(), &CmpIntersections);

    // Only save the intersection names.
    std::vector<std::string> res(pairs.size());
    for (std::size_t i = 0; i < res.size(); i += 1) {
        res[i] = pairs[i].first;
    }
    return res;
}
