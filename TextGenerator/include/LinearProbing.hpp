#pragma once
#include "ArrayList.hpp"
#include <functional>
#include <utility>
#include <stdexcept>
#include <optional>

template <typename Key, typename Value>
class LinearProbing {
public:
    LinearProbing(size_t initialCapacity = 101);

    void insert(const Key& key, const Value& value);

    bool remove(const Key& key);

    Value* find(const Key& key);

    Value& operator[](const Key& key);

    size_t size() const;

    size_t capacity() const;

private:
    enum class SlotStatus { EMPTY, OCCUPIED, DELETED };

    struct Slot {
        std::optional<Key> key;
        std::optional<Value> value;
        SlotStatus status = SlotStatus::EMPTY;
    };

    ArrayList<Slot> table_;
    size_t count_;
    size_t capacity_;
    static constexpr double LOAD_FACTOR = 0.7;

    size_t hash(const Key& key) const;
    void rehash();
};

// Implementation

template <typename Key, typename Value>
LinearProbing<Key, Value>::LinearProbing(size_t initialCapacity)
    : table_(initialCapacity), count_(0), capacity_(initialCapacity) {}

template <typename Key, typename Value>
size_t LinearProbing<Key, Value>::hash(const Key& key) const {
    // TODO: implement hash function
    size_t hashCode = std::hash<Key>{}(key);
    return hashCode % capacity_;
}

template <typename Key, typename Value>
void LinearProbing<Key, Value>::insert(const Key& key, const Value& value) {
    // TODO: implement insert
    // rehash if load factor exceeded
    
    /*
        some condition: rehash
        load factor??
            ● n / m is called the load factor
                ○ load factor tell us how full the hash table is

            n = elements
            m = buckets

            so elements/size
            so when 70% full (since load factor is .7) rehash
            consider the element that is being added to so +1 to count

        check if the position is occupied, if it isn't insert it there
        else keep going by incrementing index, make sure to do % size so
        it wraps around since index not guaranteed to start at 0.

        DOUBLE CHECK: if elements already there then replace the value
        just check if the keys match

        if full then rehash or something so I think it'll never be a continuous loop?

        std::optional, if you want the actual key value or value value, call .key.value() or .value.value()
        here should just be .key since key parameter is of type Key.
    */

    double one = 1.0;
    if (((count_ + 1) * one / capacity_) > LOAD_FACTOR) { rehash(); }

    size_t index = hash(key);
    while (table_.get(index).status == SlotStatus::OCCUPIED) {
        if (table_.get(index).key == key) {
            table.get(index).value = value; 
            return;
        }
        index = (index + 1) % capacity_;
    }

    auto slot = table.get(index);
    slot.key = key;
    slot.value = value;
    slot.status = SlotStatus::OCCUPIED;
    count_++;
    
}

template <typename Key, typename Value>
bool LinearProbing<Key, Value>::remove(const Key& key) {
    // TODO: implement remove
}

template <typename Key, typename Value>
Value* LinearProbing<Key, Value>::find(const Key& key) {
    // TODO: implement find
}

template <typename Key, typename Value>
Value& LinearProbing<Key, Value>::operator[](const Key& key) {
    // TODO: implement operator[] to return reference to value associated with key
    // if key not found, insert default-constructed Value and return its reference

}

template <typename Key, typename Value>
size_t LinearProbing<Key, Value>::size() const {
    return count_;
}

template <typename Key, typename Value>
size_t LinearProbing<Key, Value>::capacity() const {
    return capacity_;
}

template <typename Key, typename Value>
void LinearProbing<Key, Value>::rehash() {
    // TODO: Implement rehashing by doubling capacity and reinserting all items
    
    // note to self when implementing this, use the built in hash. std::optional for key and value
    // has a function that you can call named .value(). You cant just call slot.key, call slot.key.value()

    size_t newCap = capacity_ * 2;
    ArrayList<Slot> newArrayList(newCap);

    for (size_t i = 0; i < table_.size(); i++) {
        if (table_.get(i).status == SlotStatus::OCCUPIED) {

        }
    }
}