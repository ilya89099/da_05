#include <iostream>
#include <set>

using namespace std;

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

set<size_t> find_all_substrings(const string& text, const string& pattern) {
    set<size_t> result;
    for (size_t i = 0; i < text.size() - pattern.size() + 1; ++i) {
        bool failed = false;
        for (size_t j = 0; j < pattern.size(); ++j) {
            if (text[i + j] != pattern[j]) {
                failed = true;
                break;
            }
        }
        if (!failed) {
            result.insert(i + 1);
        }
    }
    return result;
}

int main() {
    string text;
    getline(cin, text);
    string pattern;
    size_t count = 1;
    while(getline(cin, pattern)) {
        if (pattern.size() > text.size()) {
        } else {
            std::set<size_t> result = find_all_substrings(text, pattern);
            if (result.empty()) {
            } else {
                std::cout << count << ": " << result << "\n";
            }

        }
        count++;

    }
    return 0;

}
