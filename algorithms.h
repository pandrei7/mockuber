// Copyright 2019 Preda Andrei

#ifndef ALGORITHMS_H_
#define ALGORITHMS_H_

#include <utility>
#include <vector>

template <typename T>
using CmpFunc = bool (*)(const T &a, const T &b);

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
