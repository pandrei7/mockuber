// Copyright 2019 Preda Andrei

#ifndef UBER_APP_H_
#define UBER_APP_H_

#include <string>
#include <vector>

#include "./uber_database.h"
#include "./uber_driver.h"
#include "./uber_map.h"

class UberApp
{
 public:
    void AddIntersection(const std::string &name);
    void AddStreet(const std::string &a, const std::string &b);
    void RemoveStreet(const std::string &a, const std::string &b);
    void ReverseStreet(const std::string &a, const std::string &b);

    int Distance(const std::string &source, const std::string &dest) const;

    UberDriver Driver(const std::string &name) const;

    void GoOnline(const std::string &driver, const std::string &location);
    void GoOffline(const std::string &driver);

    std::string MakeTrip(const std::string &source,
                         const std::string &dest, double rating);

    std::vector<UberDriver> SortedDrivers(const std::string &mode) const;

    std::vector<std::string> SortedByDistance(const std::string &source,
                                const std::vector<std::string> &dest) const;

 private:
    UberDatabase database_;
    UberMap map_;
};

#endif  // UBER_APP_H_
