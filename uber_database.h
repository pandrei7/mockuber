// Copyright 2019 Preda Andrei

#ifndef UBER_DATABASE_H_
#define UBER_DATABASE_H_

#include <cstddef>
#include <string>
#include <vector>

#include "./hashtable.h"
#include "./uber_driver.h"

class UberDatabase
{
 public:
    UberDatabase();

    int Id(const std::string &name) const;

    UberDriver Driver(const std::string &name) const;

    void GoOnline(const std::string &name, const std::string &location);

    void GoOffline(const std::string &name);

    void MakeTrip(const std::string &name, const std::string &dest,
                                        int distance, double rating);

    std::string BestDriver(const std::vector<int> &ids) const;

    std::vector<UberDriver> SortedDrivers(const std::string &mode,
                                          std::size_t count) const;

 private:
    using CmpFunc = bool(*)(const UberDriver &a, const UberDriver &b);

    void UpdateSortings(int id);
    void Reorder(std::vector<int> &sorted, int id, CmpFunc cmp);

    std::vector<UberDriver> drivers_;
    Hashtable<std::string, int> ids_;

    Hashtable<std::string, std::vector<int>> sorted_ids_;
    Hashtable<std::string, CmpFunc> cmp_funcs_;
};

#endif  // UBER_DATABASE_H_
