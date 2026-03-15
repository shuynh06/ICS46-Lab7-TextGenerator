#pragma once
#include "ArrayList.hpp"
#include <functional>
#include <utility>
#include <optional>
#include <stdexcept>
#include <cstdint>
#include <cmath>
#include <string>

// FNV-1a hash for hash1 (for std::string keys)
inline size_t fnv1a_hash1(const std::string& s, size_t cap) {
    size_t hash = 14695981039346656037ULL;
    for (char c : s) {
        hash ^= static_cast<unsigned char>(c);
        hash *= 1099511628211ULL;
    }
    return hash % cap;
}

// FNV-1a hash for hash2 (different offset basis and salt for independence)
inline size_t fnv1a_hash2(const std::string& s, size_t cap) {
    size_t hash = 1099511628211ULL; // Different offset basis
    for (char c : s) {
        hash ^= static_cast<unsigned char>(c + 31); // Add salt to each char
        hash *= 14695981039346656037ULL;
    }
    return hash % cap;
}

template <typename Key, typename Value>
class CuckooHash {
public:
    CuckooHash(size_t initialCapacity = 128, size_t maxKick = 256);

    void insert(const Key& key, const Value& value);
    bool remove(const Key& key);
    Value* find(const Key& key);
    Value& operator[](const Key& key);

    size_t size() const;
    size_t capacity() const;

private:
    struct Slot {
        std::optional<Key> key;
        std::optional<Value> value;
        bool occupied = false;
    };

    ArrayList<Slot> table_;
    size_t count_;
    size_t capacity_;
    size_t maxKick_;

    size_t hash1(const Key& key, size_t cap) const;
    size_t hash2(const Key& key, size_t cap) const;
    void rehash();

    static constexpr double LOAD_FACTOR = 0.5;
};

// Implementation

template <typename Key, typename Value>
CuckooHash<Key, Value>::CuckooHash(size_t initialCapacity, size_t maxKick)
    : table_(initialCapacity), count_(0), capacity_(initialCapacity)
{
    if (maxKick == 0) {
        maxKick_ = static_cast<size_t>(std::log2(capacity_) * 8);
        if (maxKick_ < 32) maxKick_ = 32;
    } else {
        maxKick_ = maxKick;
    }
}

template <typename Key, typename Value>
size_t CuckooHash<Key, Value>::hash1(const Key& key, size_t cap) const {
    if constexpr (std::is_same<Key, std::string>::value) {
        return fnv1a_hash1(key, cap);
    } else {
        return std::hash<Key>{}(key) % cap;
    }
}

template <typename Key, typename Value>
size_t CuckooHash<Key, Value>::hash2(const Key& key, size_t cap) const {
    if constexpr (std::is_same<Key, std::string>::value) {
        return fnv1a_hash2(key, cap);
    } else {
        // Use a different mixing for non-string keys
        std::size_t h = std::hash<Key>{}(key);
        h ^= (h << 13);
        h ^= (h >> 7);
        h ^= (h << 17);
        h *= 0x9e3779b97f4a7c15ULL;
        return h % cap;
    }
}

template <typename Key, typename Value>
void CuckooHash<Key, Value>::insert(const Key& key, const Value& value) {
    // TODO: Implement insert with cuckoo hashing and rehashing
    size_t index1 = hash1(key, capacity());
    size_t index2 = hash2(key, capacity());


    if (table_.get(index1).occupied && table_.get(index1).key == key) {
        table_.get(index1).value = value;
        return;
    } else if (table_.get(index1).occupied == false) {
        table_.get(index1).key = key;
        table_.get(index1).value = value;
        table_.get(index1).occupied = true;
        count_++;
        return;
    }

    if (table_.get(index2).occupied && table_.get(index2).key == key) {
        table_.get(index2).value = value;
        return;
    } else if (table_.get(index2).occupied == false) {
        table_.get(index2).key = key;
        table_.get(index2).value = value;
        table_.get(index2).occupied = true;
        count_++;
        return;
    }

    Key currentKey = key;
    Value currentValue = value;
    size_t kick = 0;
    while (kick < maxKick_) {
        Key oldKey = table_.get(index1).key.value();
        Value oldValue = table_.get(index1).value.value();

        table_.get(index1).key = currentKey;
        table_.get(index1).value = currentValue;
        table_.get(index1).occupied = true;

        size_t newIndex1 = hash1(oldKey, capacity());
        size_t newIndex2 = hash2(oldKey, capacity());    

        size_t newIndex = (newIndex1 == index1) ? newIndex2: newIndex1;

        if (table_.get(newIndex).occupied == false) {
            table_.get(newIndex).key = oldKey;
            table_.get(newIndex).value = oldValue;
            table_.get(newIndex).occupied = true;
            count_++;
            return;
        }

        currentKey = oldKey;
        currentValue = oldValue;
        index1 = newIndex;
        kick++;
    }

    rehash();
    insert(currentKey, currentValue);
}

template <typename Key, typename Value>
bool CuckooHash<Key, Value>::remove(const Key& key) {
    // TODO: Implement remove
    size_t index1 = hash1(key, capacity());
    size_t index2 = hash2(key, capacity());

    if (table_.get(index1).occupied == true && table_.get(index1).key == key) {
        table_.get(index1).occupied = false;
        count_--;
        return true;
    }

    if (table_.get(index2).occupied == true && table_.get(index2).key == key) {
        table_.get(index2).occupied = false;
        count_--;
        return true;
    }

    return false;
}

template <typename Key, typename Value>
Value* CuckooHash<Key, Value>::find(const Key& key) {
    // TODO: Implement find
    size_t index1 = hash1(key, capacity());
    size_t index2 = hash2(key, capacity());

    if (table_.get(index1).occupied == true && table_.get(index1).key == key) {
        return &table_.get(index1).value.value();
    }

    if (table_.get(index2).occupied == true && table_.get(index2).key == key) {
        return &table_.get(index2).value.value();
    }

    return nullptr;
}

template <typename Key, typename Value>
Value& CuckooHash<Key, Value>::operator[](const Key& key) {
    // TODO: implement operator[] to return reference to value associated with key
    // If key not found, insert default-constructed Value and return its reference
    if (find(key)) { return *find(key); }
    insert(key, Value{});
    return *find(key);
}

template <typename Key, typename Value>
size_t CuckooHash<Key, Value>::size() const {
    return count_;
}

template <typename Key, typename Value>
size_t CuckooHash<Key, Value>::capacity() const {
    return capacity_;
}

template <typename Key, typename Value>
void CuckooHash<Key, Value>::rehash() {
    // TODO: Implement rehashing by doubling capacity and reinserting all items
    ArrayList<Slot> oldTable = table_;
    capacity_ *= 2;

    table_ = ArrayList<Slot>(capacity_);
    count_ = 0;

    size_t index = 0;
    while (index < capacity_ / 2) { // says doubling capacity so hopefully this is fine?
        if (oldTable.get(index).occupied == true) {
            insert(oldTable.get(index).key.value(), oldTable.get(index).value.value());
        }
        index++;
    }
}