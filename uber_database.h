// Copyright 2019 Preda Andrei

#ifndef UBER_DATABASE_H_
#define UBER_DATABASE_H_

#include <string>
#include <vector>

#include "./hashtable.h"
#include "./uber_driver.h"

// This class represents the database of Uber drivers.
class UberDatabase
{
 public:
    using CmpFunc = bool(*)(const UberDriver &a, const UberDriver &b);

    // This method returns a drivers id, given his name.
    // It returns -1 if there is no driver with the given name.
    int Id(const std::string &name) const;

    // This method return data about a driver, given his name.
    UberDriver Driver(const std::string &name) const;

    void GoOnline(const std::string &name, const std::string &location);

    void GoOffline(const std::string &name);

    void MakeTrip(const std::string &name, const std::string &dest,
                                        int distance, double rating);

    // This method returns the name of the best driver, given a list of ids.
    std::string BestDriver(const std::vector<int> &ids) const;

    // This method return a list of the drivers, sorted by certain criteria.
    // If the function pointer is nullptr, the drivers are not ordered.
    std::vector<UberDriver> SortedDrivers(const CmpFunc &cmp) const;

 private:
    std::vector<UberDriver> drivers_;
    Hashtable<std::string, int> ids_;
};

#endif  // UBER_DATABASE_H_
