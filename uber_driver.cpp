// Copyright 2019 Preda Andrei

#include "./uber_driver.h"

void UberDriver::MakeTrip(const std::string &dest, int distance, double rating)
{
    location_ = dest;
    distance_ += distance;

    rating_ = (rating_ * trips_ + rating) / (trips_ + 1);
    trips_ += 1;
}

void UberDriver::GoOnline(const std::string &location)
{
    online_ = true;
    location_ = location;
}

void UberDriver::GoOffline()
{
    online_ = false;
}
