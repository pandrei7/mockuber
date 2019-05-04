// Copyright 2019 Preda Andrei

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

#include "./hashtable_utils.h"

template <typename TKey,
          typename TValue,
          typename THash = Hash<TKey>,
          typename TEqual = Equal<TKey>>
class Hashtable
{
 public:
    using Pair = std::pair<TKey, TValue>;

    Hashtable(std::size_t capacity = 1000,
              const THash &hash = Hash<TKey>(),
              const TEqual &equal = Equal<TKey>())
        : hash_(hash),
          equal_(equal),
          slots_(std::vector<Pair>(capacity)),
          occupied_(std::vector<bool>(capacity, false)),
          size_(0) {}

    std::size_t Size() const
    {
        return size_;
    }

    bool HasKey(const TKey &key) const
    {
        auto index = hash_(key) % slots_.size();

        while (occupied_[index]) {
            if (equal_(slots_[index].first, key)) {
                return true;
            }
            index = (index + 1) % slots_.size();
        }
        return false;
    }

    void Put(const TKey &key, const TValue &value)
    {
        auto index = hash_(key) % slots_.size();

        while (occupied_[index]) {
            if (equal_(slots_[index].first, key)) {
                slots_[index].second = value;
                return;
            }
            index = (index + 1) % slots_.size();
        }

        slots_[index] = {key, value};
        occupied_[index] = true;
        size_ += 1;
    }

    TValue Get(const TKey &key) const
    {
        auto index = hash_(key) % slots_.size();

        while (occupied_[index]) {
            if (equal_(slots_[index].first, key)) {
                return slots_[index].second;
            }
            index = (index + 1) % slots_.size();
        }
        return TValue();
    }

 private:
    THash hash_;
    TEqual equal_;
    std::vector<Pair> slots_;
    std::vector<bool> occupied_;
    std::size_t size_;
};

#endif  // HASHTABLE_H_
