// Copyright 2019 Preda Andrei

#ifndef UBER_APP_H_
#define UBER_APP_H_

#include <string>
#include <vector>

#include "./uber_database.h"
#include "./uber_driver.h"
#include "./uber_map.h"

// This class represents the "Uber app" as a whole.
// It provides an interface used to solve all the tasks.
class UberApp
{
 public:
    // These methods change the layout of the map.
    void AddIntersection(const std::string &name);
    void AddStreet(const std::string &a, const std::string &b);
    void RemoveStreet(const std::string &a, const std::string &b);
    void ReverseStreet(const std::string &a, const std::string &b);

    // This method returns the distance between two intersections,
    // or -1 if there is not path between them.
    int Distance(const std::string &source, const std::string &dest) const;

    // This method returns data about a driver, given his name.
    UberDriver Driver(const std::string &name) const;

    // These methods change the status of the drivers.
    void GoOnline(const std::string &driver, const std::string &location);
    void GoOffline(const std::string &driver);

    // This method tries to make a trip from source to dest.
    // It returns an appropriate message related to the trip status.
    std::string MakeTrip(const std::string &source,
                         const std::string &dest, double rating);

    // This method returns a list of drivers, sorted by certain criteria.
    // mode can be one of the following: "top_rating", "top_dist", "top_rides".
    std::vector<UberDriver> SortedDrivers(const std::string &mode) const;

    // This method sorts a list of intersections in ascending order of distance
    // from source. Only intersections that can be reached in at most
    // max_dist steps are kept. The returned list does not contain duplicates.
    std::vector<std::string> SortedByDistance(const std::string &source,
                    const std::vector<std::string> &dest, int max_dist) const;

 private:
    UberDatabase database_;  // The database of drivers.
    UberMap map_;           // The map of the city.
};

#endif  // UBER_APP_H_
