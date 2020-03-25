#include <iostream>
#include "SuffixTree.h"

template <typename T>
std::ostream& operator << (std::ostream& os, const std::set<T>& v) {
    bool first = true;
    for (const T& i : v) {
        if (!first) {
            os << ", ";
        } else {
            first = false;
        }
        os << i;
    }
    return os;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::string text;
    std::getline(std::cin, text);
    SuffixTree tree(std::move(text));
    SuffixArray array = tree.ConstructArray();

    std::string pattern;
    int count = 1;
    while(getline(std::cin, pattern)) {
        std::set<long long> result = array.SearchForString(pattern);
        if (!result.empty()) {
            std::cout << count << ": " << result << "\n";
        }
        count++;
    }
    return 0;
}
