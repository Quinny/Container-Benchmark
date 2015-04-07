#include <random>
#include <vector>
#include <map>
#include <unordered_map>
#include <stack>
#include "qp_bm"

// Just some simple functions for testing purposes
std::string random_string() {
    std::string letters = "abcdefghijklmnopqrstuvwxyz";
    std::string s = "";
    for (int i = 0; i < rand() % 20; ++i)
        s += letters[rand() % 26];
    return s;
}

std::pair<std::string, int> pair_gen() {
    return std::make_pair(random_string(), rand());
}

int main() {
    std::map<std::string, int> omap;
    // Insert 100 elements into umap.  The elements
    // will be provided by pair_gen
    // (which must be a no argument function)
    auto t1 = qap::bm::insert(omap, pair_gen, 100);
    // the return value is the time in seconds (common for all functions)
    // that it took for the 100 inserts
    std::cout << "Inserting 100 elements took " << t1 << " seconds" << std::endl;

    // Performs 1000 look ups on the container using the generating
    // function random_string and reports the time in seconds
    // if the class does not have a find function,
    // 0 will be returned and an error will be displayed.
    auto t2 = qap::bm::find(omap, random_string, 1000);

    // If the container supports iteration, this function will record
    // the time taken to iterate through each element.
    // If the container does not support iteration, 0 will be returned
    // and an error message will be displayed
    auto t3 = qap::bm::iterate(omap);
    std::cout << "Iteration took " << t2 << " seconds" << std::endl;

    // Time taken to copy construct the container
    auto t4 = qap::bm::copy(omap);
    // Time taken to move construct the container
    auto t5 = qap::bm::move(omap);

    std::unordered_map<std::string, int> umap;
    std::map<std::string, int> omap2;

    // Compare the operatings on ordered and unordered maps
    // using the pattern (container, insert_gen, lookup_gen),
    // number of inserts/lookups
    // The output is pretty
    qap::bm::compare_all(umap, pair_gen, random_string,
                         omap2, pair_gen, random_string,
                         10000);

    // Now comes the fancy stuff...
    // All the functions are specialized so you can compare
    // two completely different containers super easy
    // for example...
    std::vector<int> v;
    std::stack<int> s;
    qap::bm::compare_all(v, rand, rand,
                         s, rand, rand,
                         1000, true);
    // The final true the call enables the full (demangled) type to be
    // displayed in the output (beware, it gets messy)
    return 0;
}
