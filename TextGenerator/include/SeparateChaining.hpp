#pragma once
#include "ArrayList.hpp"
#include "SinglyLinkedList.hpp"
#include <functional>
#include <utility>
#include <stdexcept>

template <typename Key, typename Value>
class SeparateChaining {
public:
    SeparateChaining(size_t bucketCount = 128);

    void insert(const Key& key, const Value& value);
    Value* find(const Key& key);
    bool remove(const Key& key);
    Value& operator[](const Key& key);
    size_t size() const;
    size_t bucketCount() const;

private:
    ArrayList<SinglyLinkedList<std::pair<Key, Value>>> buckets_;
    size_t hash(const Key& key) const;
};

// Implementation

template <typename Key, typename Value>
SeparateChaining<Key, Value>::SeparateChaining(size_t bucketCount)
    : buckets_(bucketCount > 0 ? bucketCount : 128) {}

template <typename Key, typename Value>
void SeparateChaining<Key, Value>::insert(const Key& key, const Value& value) {
    // TODO: implement insert

    /*
    ● Hash
        ○ Map key to integer i between 0 and b - 1
        -> function was basically given on slideshow 14
    ● Insert
        ○ Add to i-th chain
            ■ if not already there
             -> replace it with the new value, so check for matching key, if matching
                then replace the value with the new value
            ■ What does this require?
             -> iterating through that i-th chain and checking for matching keys
    
    */

    size_t index = hash(key);
    SinglyLinkedList<std::pair<Key, Value>>& chain = buckets_.get(index);
    auto currentNode = chain.getHead();

    while (currentNode != nullptr) {
        if (currentNode->item.first == key) {
            currentNode->item.second = value;
            return;
        }
        currentNode = currentNode->next;
    }

    chain.add({key, value});
}

template <typename Key, typename Value>
Value* SeparateChaining<Key, Value>::find(const Key& key) {
    // TODO: implement find
    size_t index = hash(key);
    SinglyLinkedList<std::pair<Key, Value>>& chain = buckets_.get(index);
    auto currentNode = chain.getHead();

    while (currentNode != nullptr) {
        if (currentNode->item.first == key) {
            return &currentNode->item.second;
        }
        currentNode = currentNode->next;
    }

    return nullptr;
}

template <typename Key, typename Value>
bool SeparateChaining<Key, Value>::remove(const Key& key) {
    // TODO: implement remove
    size_t hash_index = hash(key);
    SinglyLinkedList<std::pair<Key, Value>>& chain = buckets_.get(hash_index);
    auto currentNode = chain.getHead();
    int chain_index = 0;

    // if this causes a bug then maybe manually remove it instead of calling the function
    // something like currentNode->next != nullptr -> if next == key then tmp variable to next
    // set current next = tmp next then delete tmp

    while (currentNode != nullptr) {
        if (currentNode->item.first == key) {
            chain.remove(chain_index);
            return true;
        }
        currentNode = currentNode->next;
        chain_index++;
    }

    return false;
}

template <typename Key, typename Value>
Value& SeparateChaining<Key, Value>::operator[](const Key& key) {
    // TODO: implement operator[] to return reference to value associated with key
    // If key not found, insert default-constructed Value and return its reference
    /*
    size_t index = hash(key);
    SinglyLinkedList<std::pair<Key, Value>>& chain = buckets_.get(index);
    auto currentNode = chain.getHead();

    
    while(currentNode != nullptr) {
        if (currentNode->item.first == key) {
            return currentNode->item.second;
        }
    }
    same as find just use that instead
    */

    if (find(key)) { return *find(key); }
    insert(key, Value{});
    return *find(key);
}

template <typename Key, typename Value>
size_t SeparateChaining<Key, Value>::size() const {
    size_t count = 0;
    for (size_t i = 0; i < buckets_.size(); ++i) {
        const auto& list = buckets_.get(i);
        auto node = list.getHead();
        while (node != nullptr) {
            ++count;
            node = node->next;
        }
    }
    return count;
}

template <typename Key, typename Value>
size_t SeparateChaining<Key, Value>::bucketCount() const {
    return buckets_.size();
}

template <typename Key, typename Value>
size_t SeparateChaining<Key, Value>::hash(const Key& key) const {
    // TODO: implement hash function
    size_t hashCode = std::hash<Key>{}(key);
    size_t numBuckets = buckets_.size();
    return hashCode % numBuckets;
}
