#pragma once
#include <random>
#include <string>
#include <cctype>
#include "StringPair.hpp"
#include "FreqList.hpp"
#include "WordStream.hpp"

template <typename HashTable>
class TextGenerator {
public:
    TextGenerator();
    
    TextGenerator(size_t bucketCount = 101)
        : stringPairList_(bucketCount), rng_(std::random_device{}()) {}

    void enter(const std::string& first, const std::string& second, const std::string& third);

    std::string getNextWord(const std::string& first, const std::string& second);

    static std::string toLower(const std::string& s) {
        std::string result;
        for (char c : s) result += std::tolower(c);
        return result;
    }

private:
    HashTable stringPairList_;
    std::mt19937 rng_;
};

// --- Implementation ---

template <typename HashTable>
TextGenerator<HashTable>::TextGenerator() : rng_(std::random_device{}()) {}

template <typename HashTable>
void TextGenerator<HashTable>::enter(const std::string& first, const std::string& second, const std::string& third) {
    // TODO: implement enter

}

template <typename HashTable>
std::string TextGenerator<HashTable>::getNextWord(const std::string& first, const std::string& second) {
    // TODO: implement getNextWord

}

