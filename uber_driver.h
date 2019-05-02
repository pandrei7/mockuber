// Copyright 2019 Preda Andrei

#ifndef UBER_DRIVER_H_
#define UBER_DRIVER_H_

#include <string>

class UberDriver
{
 public:
     explicit UberDriver(const std::string &name)
                 : name_(name), rating_(0), location_(-1),
                   distance_(0), trips_(0), online_(false) {}

     void MakeTrip(int dest, int distance, double rating);

     void GoOnline(int location) { online_ = true; location_ = location; }
     void GoOffline() { online_ = false; }

     const std::string& Name() const { return name_; }

     double Rating() const { return rating_; }

     int Location() const { return location_; }

     int Distance() const { return distance_; }

     int Trips() const { return trips_; }

     bool Online() const { return online_; }

 private:
    std::string name_;
    double rating_;
    int location_;
    int distance_;
    int trips_;
    bool online_;
};

#endif  // UBER_DRIVER_H_
