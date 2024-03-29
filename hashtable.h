// Copyright 2019 Preda Andrei

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <algorithm>
#include <cstddef>
#include <list>
#include <utility>
#include <vector>

#include "./hashtable_utils.h"

// This class represents a generic, resizable hashtable.
// It uniquely associates a key of type TKey with a value of type TValue.
// The class accepts a custom "hash operator" of type THash.
// Equality between two keys is determined using an operator of type TEqual.
// Collisions are solved using the separate chaining technique.
// The hashtable is resized when a fixed "load factor" is reached.
template <typename TKey,
          typename TValue,
          typename THash = Hash<TKey>,
          typename TEqual = Equal<TKey>>
class Hashtable
{
 public:
    using Pair = std::pair<TKey, TValue>;

    Hashtable(std::size_t capacity = 3,
              const THash &hash = Hash<TKey>(),
              const TEqual &equal = Equal<TKey>())
        : hash_(hash),
          equal_(equal),
          buckets_(std::vector<std::list<Pair>>(capacity)),
          size_(0) {}

    // This method returns the number of keys held in the hashtable.
    std::size_t Size() const
    {
        return size_;
    }

    bool HasKey(const TKey &key) const
    {
        auto index = hash_(key) % buckets_.size();

        for (const auto &p : buckets_[index]) {
            if (equal_(p.first, key)) {
                return true;
            }
        }
        return false;
    }

    // This method updates an existing key-value pair with a new value,
    // or inserts a new key-value pair if the key cannot be found.
    void Put(const TKey &key, const TValue &value)
    {
        auto index = hash_(key) % buckets_.size();

        for (auto &p : buckets_[index]) {
            if (equal_(p.first, key)) {
                p.second = value;
                return;
            }
        }

        buckets_[index].push_back({key, value});
        size_ += 1;

        if (size_ > kLoadFactor * buckets_.size()) {
            Resize();
        }
    }

    // This method return the value associated with a given key.
    // If the key cannot be found, a default-constructed value is returned.
    TValue Get(const TKey &key) const
    {
        auto index = hash_(key) % buckets_.size();

        for (const auto &p : buckets_[index]) {
            if (equal_(p.first, key)) {
                return p.second;
            }
        }
        return TValue();
    }

 private:
    static constexpr double kLoadFactor = 0.75;
    static constexpr double kResizeFactor = 2.337;

    // This method resizes the hashtable.
    // The number of buckets gets multiplied by a constant.
    void Resize()
    {
        std::size_t new_capacity = buckets_.size() * kResizeFactor;
        std::vector<std::list<Pair>> new_buckets(new_capacity);

        for (const auto &bucket : buckets_) {
            for (const auto &p : bucket) {
                auto index = hash_(p.first) % new_capacity;
                new_buckets[index].push_back(p);
            }
        }
        buckets_ = new_buckets;
    }

    THash hash_;
    TEqual equal_;
    std::vector<std::list<Pair>> buckets_;
    std::size_t size_;
};

#endif  // HASHTABLE_H_
