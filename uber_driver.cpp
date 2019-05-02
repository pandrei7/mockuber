// Copyright 2019 Preda Andrei

#include "./uber_driver.h"

void UberDriver::MakeTrip(int dest, int distance, double rating)
{
    location_ = dest;
    distance_ += distance;

    rating_ = (rating_ * trips_ + rating) / (trips_ + 1);
    trips_ += 1;
}
