#include <iostream>
#include <string>
#include <random>

using namespace std;

default_random_engine rng;

uint64_t get_number(uint64_t min = 0,uint64_t max = numeric_limits<unsigned long long>::max()) {
    uniform_int_distribution<unsigned long long> dist_ab(min, max);
    return dist_ab(rng);
}

string get_string(int size) {
    size_t string_size = get_number(size,size);
    string string;
    string.resize(string_size);
    for (size_t i = 0; i < string_size; ++i) {
        string[i] = 'a' + get_number(0,25);
    }
    return string;
}


int main(int argc, char** argv) {

    string str = get_string(stoi(argv[1]));
    vector<string> substrings;
    for (int i = 1; i <= str.size(); ++i) {
        for (int j = 0; j <= str.size() - i; ++j) {
            substrings.push_back(str.substr(j,i));
        }
    }
    std::cout << str << "\n";
    for (auto& i : substrings) {
        std::cout << i << "\n";
    }
    return 0;

}