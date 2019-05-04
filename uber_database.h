// Copyright 2019 Preda Andrei

#ifndef UBER_DATABASE_H_
#define UBER_DATABASE_H_

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "./hashtable.h"
#include "./sorted_list.h"
#include "./uber_driver.h"

class UberDatabase
{
 public:
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
    void RemoveFromSortings(int id);
    void AddToSortings(int id);

    std::vector<UberDriver> drivers_;
    Hashtable<std::string, int> ids_;

    SortedList<std::pair<double, std::string>> by_rating_;
    SortedList<std::pair<double, std::string>> by_dist_;
    SortedList<std::pair<double, std::string>> by_trips_;
};

#endif  // UBER_DATABASE_H_
