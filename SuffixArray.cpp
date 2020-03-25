#include <functional>
#include <algorithm>
#include "SuffixArray.h"

SuffixArray::SuffixArray(std::vector<long long> newArray, std::string newData)
: data(std::move(newData)), array(std::move(newArray)) {}

std::ostream& operator << (std::ostream& os, const SuffixArray& arr) {
    for (long long elem : arr.array) {
        os << elem << " ";
    }
    return os;
}


struct Compare {
    Compare(size_t newCompareIndex)
    : compareIndex(newCompareIndex) {}

    bool operator () (const char* lhs, const char* rhs) {
        return *(lhs + compareIndex) < *(rhs + compareIndex);
    }

private:
    size_t compareIndex;
};

std::set<long long> SuffixArray::SearchForString(std::string sub) const {
    std::set<long long> result;
    long long left = 0;
    long long right = array.size();
    std::vector<const char*> tempArray(array.size());
    for (size_t i = 0; i < array.size(); ++i) {
        tempArray[i] = data.data() + (array[i] - 1);
    }
    for (size_t i = 0; i < sub.size(); ++i) {
        long long localLeft = left;
        long long localRight = right;
        left = lower_bound(tempArray.begin() + localLeft, tempArray.begin() + localRight, sub.data(), Compare(i)) - tempArray.begin();
        right = upper_bound(tempArray.begin() + localLeft, tempArray.begin() + localRight, sub.data(), Compare(i)) - tempArray.begin();
    }
    for (size_t j = left; j < right; ++j) {
        result.insert(array[j]);
    }

    return result;
}