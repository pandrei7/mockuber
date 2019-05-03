// Copyright 2019 Preda Andrei

#ifndef UBER_DATABASE_H_
#define UBER_DATABASE_H_

#include <string>
#include <vector>

#include "./hashtable.h"
#include "./uber_driver.h"

class UberDatabase
{
 public:
    using CmpFunc = bool(*)(const UberDriver &a, const UberDriver &b);

    int Id(const std::string &name) const;

    void GoOnline(const std::string &name, const std::string &location);

    void GoOffline(const std::string &name);

    void MakeTrip(const std::string &name, const std::string &dest,
                                        int distance, double rating);

    std::vector<UberDriver> SortedDrivers(const CmpFunc &cmp) const;

 private:
    std::vector<UberDriver> drivers_;
    Hashtable<std::string, int> ids_;
};

#endif  // UBER_DATABASE_H_
