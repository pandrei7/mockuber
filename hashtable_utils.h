// Copyright 2019 Preda Andrei

#ifndef HASHTABLE_UTILS_H_
#define HASHTABLE_UTILS_H_

#include <algorithm>
#include <cstddef>
#include <string>

// This class represents a generic "equal-to" operator.
// When called, it returns the result of the "==" operator.
template <typename T>
class Equal
{
 public:
    bool operator()(const T &a, const T &b) const
    {
        return a == b;
    }
};

// This class represents a generic "hash-operator".
// When called, it should return the result of a hash function for a given type.
template <typename T>
class Hash;

// This is a specialization of Hash for std::string.
// It uses the SDBM algorithm to compute hash values.
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

#endif  // HASHTABLE_UTILS_H_
