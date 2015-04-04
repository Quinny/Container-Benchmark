#include "qp_benchmark.h"
#include <random>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <queue>
#include <stack>

std::string random_string() {
    std::string letters = "abcdefghijklmnopqrstuvwxyz";
    std::string s = "";
    for (int i = 0; i < rand() % 20; ++i)
        s += letters[rand() % 26];
    return s;
}

std::pair<std::string, int> g() {
    return std::make_pair(random_string(), rand());
}

int main() {
    std::stack<std::pair<std::string, int>> vec;
    std::unordered_map<std::string, int> umap;
    qap::bm::compare_all(vec, g, umap, g, 100000);
    return 0;
}
