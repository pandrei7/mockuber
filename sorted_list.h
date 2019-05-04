// Copyright 2019 Preda Andrei

#ifndef SORTED_LIST_H_
#define SORTED_LIST_H_

#include <cstddef>
#include <vector>

#include <iostream>

template <typename T>
struct SortedListNode
{
    T data;
    SortedListNode *left;
    SortedListNode *right;

    explicit SortedListNode(const T &data)
        : data(data), left(nullptr), right(nullptr) {}

    bool IsLeaf() const { return !left && !right; }
};

template <typename T>
class SortedList
{
 public:
    SortedList() : root_(nullptr) {}

    ~SortedList()
    {
        if (root_) {
            RemoveAll(root_);
        }
    }

    explicit SortedList(const SortedList &other)
    {
        root_ = nullptr;
        CopyAll(other.root_);
    }

    SortedList& operator=(const SortedList &other)
    {
        if (root_) {
            RemoveAll(root_);
        }
        root_ = nullptr;
        CopyAll(other.root_);
        return *this;
    }

    void Add(const T &data)
    {
        root_ = Add(root_, data);
    }

    void Remove(const T &data)
    {
        root_ = Remove(root_, data);
    }

    std::vector<T> Get(std::size_t count) const
    {
        std::vector<T> vec;
        Get(root_, count, vec);
        return vec;
    }

 private:
    SortedListNode<T>* Add(SortedListNode<T> *node, const T &data)
    {
        if (!node) {
            return new SortedListNode<T>(data);
        }

        if (node->data < data) {
            node->right = Add(node->right, data);
        } else if (node->data > data) {
            node->left = Add(node->left, data);
        }
        return node;
    }

    SortedListNode<T>* Remove(SortedListNode<T> *node, const T &data)
    {
        if (!node) {
            return nullptr;
        }

        if (node->data < data) {
            node->right = Remove(node->right, data);
            return node;
        } else if (node->data > data) {
            node->left = Remove(node->left, data);
            return node;
        }

        if (node->IsLeaf()) {
            delete node;
            return nullptr;
        }
        if (!node->left) {
            auto new_root = node->right;
            delete node;
            return new_root;
        }
        if (!node->right) {
            auto new_root = node->left;
            delete node;
            return new_root;
        }

        auto aux = node->left;
        while (aux->right) {
            aux = aux->right;
        }

        node->data = aux->data;
        node->left = Remove(node->left, aux->data);
        return node;
    }

    void Get(const SortedListNode<T> *node, std::size_t count,
                                            std::vector<T> &vec) const
    {
        if (!node) {
            return;
        }

        Get(node->left, count, vec);

        if (vec.size() < count) {
            vec.push_back(node->data);
        }
        if (vec.size() < count) {
            Get(node->right, count, vec);
        }
    }

    void RemoveAll(SortedListNode<T> *node)
    {
        if (node->left) {
            RemoveAll(node->left);
        }
        if (node->right) {
            RemoveAll(node->right);
        }
        delete node;
    }

    void CopyAll(const SortedListNode<T> *node)
    {
        Add(node->data);
        if (node->left) {
            CopyAll(node->left);
        }
        if (node->right) {
            CopyAll(node->right);
        }
    }

    SortedListNode<T> *root_;
};

#endif  // SORTED_LIST_H_
