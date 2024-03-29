// Copyright 2019 Preda Andrei

#ifndef UBER_DRIVER_H_
#define UBER_DRIVER_H_

#include <string>

// This class represents an Uber driver.
class UberDriver
{
 public:
     explicit UberDriver(const std::string &name)
                 : name_(name), location_(""), rating_(0),
                   distance_(0), trips_(0), online_(false) {}

     void MakeTrip(const std::string &dest, int distance, double rating);

     void GoOnline(const std::string &location);

     void GoOffline();

     const std::string& Name() const { return name_; }

     const std::string& Location() const { return location_; }

     double Rating() const { return rating_; }

     int Distance() const { return distance_; }

     int Trips() const { return trips_; }

     bool Online() const { return online_; }

 private:
    std::string name_;
    std::string location_;
    double rating_;  // The average rating (total_rating / number of trips).
    int distance_;
    int trips_;
    bool online_;
};

#endif  // UBER_DRIVER_H_
