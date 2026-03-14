#include "FreqList.hpp"
#include <stdexcept>
#include <sstream>

void FreqList::add(const std::string& word) {
    // TODO: implement add
    if (flist_.find(word) == flist_.end()) {
        flist_[word] = 1;
        totalFreq_++;
    } else {
        flist_[word]++;
        totalFreq_++;
    }
}

std::string FreqList::get(double p) const {
    // TODO: implement get
    if (p < 0 || p >= 1) {
        throw std::invalid_argument("p isn't within the range of [0.0, 1.0)");
    }

    if (flist_.empty()) { return ""; }

    double pToNumber = p * totalFreq_;

    double overall = 0.0;
    for (auto& pair : flist_) {
        overall += pair.second;
        if (pToNumber < overall) {
            return pair.first;
        }
    }
}

std::string FreqList::toString() const {
    std::ostringstream rep;
    rep << "Frequency List: ";
    for (const auto& kv : flist_) {
        rep << "<" << kv.first << "=" << kv.second << ">";
    }
    return rep.str();
}

size_t FreqList::size() const {
    return flist_.size();
}