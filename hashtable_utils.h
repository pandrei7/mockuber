// Copyright 2019 Preda Andrei

#ifndef HASHTABLE_UTILS_H_
#define HASHTABLE_UTILS_H_

#include <algorithm>
#include <cstddef>
#include <string>

template <typename T>
class Equal
{
 public:
    bool operator()(const T &a, const T &b) const
    {
        return a == b;
    }
};

template <typename T>
class Hash;

template <>
class Hash<std::string>
{
 public:
    std::size_t operator()(const std::string &str) const
    {
        std::size_t hash = 0;
        for (const auto &ch : str) {
            hash = ch + (hash << 6) + (hash << 16) - hash;
        }
        return hash;
    }
};

template <>
class Hash<int>
{
 public:
     std::size_t operator()(int num) const
     {
         return std::abs(num);
     }
};

#endif  // HASHTABLE_UTILS_H_
