// Copyright 2019 Preda Andrei

#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

#include <utility>
#include <vector>

// This alias describes a pointer to a generic comparison function.
template <typename T>
using CmpFunc = bool (*)(const T &a, const T &b);

// This is a helper function used by Quicksort.
// It partitions the [left, right) range of a vector into two halves:
// the left part contains elements smaller than a certain pivot,
// the right part contains greater elements.
// The function returns the position of the pivot after the partitioning.
template <typename T>
int Partition(std::vector<T> &vec, int left, int right, const CmpFunc<T> &cmp)
{
    auto mid = left + (right - 1 - left) / 2;
    std::swap(vec[left], vec[mid]);

    auto i = left + 1;
    auto j = right - 1;

    while (i <= j) {
        while (i <= j && cmp(vec[i], vec[left])) {
            i += 1;
        }
        while (i <= j && cmp(vec[left], vec[j])) {
            j -= 1;
        }
        if (i <= j) {
            std::swap(vec[i], vec[j]);
            i += 1;
            j -= 1;
        }
    }

    std::swap(vec[left], vec[i - 1]);
    return i - 1;
}

// This function sorts the elemens in the range [left, right) in ascending
// order, based on certain criteria. The comparison function cmp needs to
// return true if the first parameter passed to it should be placed first.
template <typename T>
void Quicksort(std::vector<T> &vec, int left, int right, const CmpFunc<T> &cmp)
{
    if (left + 1 < right) {
        auto pos = Partition(vec, left, right, cmp);
        Quicksort(vec, left, pos, cmp);
        Quicksort(vec, pos + 1, right, cmp);
    }
}

#endif  // ALGORITHMS_H_
