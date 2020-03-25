#pragma once

#include <set>
#include <iostream>
#include <vector>



class SuffixArray {
friend class SuffixTree;

public:
    friend std::ostream& operator << (std::ostream& os, const SuffixArray& arr);

    std::set<long long> SearchForString(std::string sub) const;

private:
    SuffixArray(std::vector<long long> newArray, std::string newData);
    std::vector<long long> array;
    std::string data;
};