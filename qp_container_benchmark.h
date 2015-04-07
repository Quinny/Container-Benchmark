#ifndef QP_BENCH_MARK_H_
#define QP_BENCH_MARK_H_

#include <chrono>
#include <iostream>
#include <cstddef>    // for size_t
#include <type_traits>
#include <algorithm> // for std::find
#include "qp_container_traits.h"
#include "qp_colors.h"
#include <cxxabi.h>

// Quinn Perfetto 2015
//
// A generic library for comparing operations on containers.
// It SHOULD work with all containers (with a few exceptions, explained in
// the comments below).
//
// All times reported/returned are in seconds and stored in doubles.

namespace qap {

namespace bm {

using namespace std::chrono;
qap::color::writer _(std::cout);

// ---------------------------------------
// Insert Benchmarks
// --------------------------------------

// If the container is associative then we enable this
// function which uses .insert to add elements to the container
template <typename Container, typename Gen>
typename std::enable_if<
    qap::traits::is_associative<Container>::value,
    double
>::type
insert(Container& c, Gen g, std::size_t n) {
    auto t1 = high_resolution_clock::now();
    while (n--)
        c.insert(g());
    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double>>(t2 - t1);
    return time_span.count();
}

// For sequence containers we want to use push_back
template <typename Container, typename Gen>
typename std::enable_if<
    qap::traits::is_sequence_container<Container>::value,
    double
>::type
insert(Container& c, Gen g, std::size_t n) {
    auto t1 = high_resolution_clock::now();
    while (n--)
        c.push_back(g());
    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double>>(t2 - t1);
    return time_span.count();
}

// Limited access containers (queue, stack, etc)
// generally use push for additions
template <typename Container, typename Gen>
typename std::enable_if<
    qap::traits::has_push<Container>::value,
    double
>::type
insert(Container& c, Gen g, std::size_t n) {
    auto t1 = high_resolution_clock::now();
    while (n--)
        c.push(g());
    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double>>(t2 - t1);
    return time_span.count();
}

// ---------------------------------------
// Iteration benchmarks
// --------------------------------------

// If the container has a begin function,
// then we assume that it can be iterated over
template <typename Container>
typename std::enable_if<
    qap::traits::has_begin<Container>::value,
    double
>::type
iterate(Container const& c) {
    auto t1 = high_resolution_clock::now();
    for (auto i : c)
    ; // suppress warnings with semi colon on next line
    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double>>(t2 - t1);
    return time_span.count();
}

template <typename Container>
typename std::enable_if<
    !qap::traits::has_begin<Container>::value,
    double
>::type
iterate(Container& c) {
    _("No begin() function found", qap::color::red) << std::endl;
    return 0;
}

// ---------------------------------------
// Copy benchmarks
// --------------------------------------
template <typename Container>
double copy(Container const& c) {
    auto t1 = high_resolution_clock::now();
    auto c2 = c;
    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double>>(t2 - t1);
    return time_span.count();
}

// ---------------------------------------
// Move benchmarks
// --------------------------------------
template <typename Container>
double move(Container const& c) {
    auto t1 = high_resolution_clock::now();
    auto c2 = std::move(c);
    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double>>(t2 - t1);
    return time_span.count();
}

// ---------------------------------------
// Look up benchmarks
// --------------------------------------

// If no find function exists then we cant do anything
template <typename Container, typename Gen>
typename std::enable_if<
    !qap::traits::has_find<Container>::value,
    double
>::type
find(Container const& c, Gen g, std::size_t n) {
    _("No find() function found", qap::color::red) << std::endl;
    return 0;
}

// If its not a pair type (e.g. vector, set)
// then we can just look up the straight value
// from the generating function
template <typename Container, typename Gen>
typename std::enable_if<
    qap::traits::has_find<Container>::value,
    double
>::type
find(Container const& c, Gen g, std::size_t n)
{
    auto t1 = high_resolution_clock::now();
    while (n--)
        c.find(g());
    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double>>(t2 - t1);
    return time_span.count();
}

// ---------------------------------------
// Utility functions
// --------------------------------------
void print_dif(double t1, double t2) {
    double dif = t1 - t2;
    std::cout << "Dif: ";
    if (dif < 0)
        _(dif, qap::color::green);
    else
        _(dif, qap::color::red);
    std::cout << std::endl;
}


template <typename Container1, typename Container2,
         typename Gen1, typename Gen2,
         typename Find1, typename Find2
>
void compare_all(Container1& c1, Gen1 gen1, Find1 find1,
        Container2& c2, Gen2 gen2, Find2 find2,
        std::size_t n, bool show_full_type = false) {
    std::string typename1, typename2;
    if (show_full_type) {
        int status;
        typename1 = abi::__cxa_demangle(typeid(c1).name(), 0, 0, &status);
        typename2 = abi::__cxa_demangle(typeid(c2).name(), 0, 0, &status);
    }
    else {
        typename1 = "Container 1";
        typename2 = "Container 2";
    }

    std::cout << "Inserting " << n << " elements:" << std::endl;
    _(typename1, qap::color::cyan) << std::endl;
    auto t1 = qap::bm::insert(c1, gen1, n);
    std::cout << t1 << std::endl;
    _(typename2, qap::color::cyan) << std::endl;
    auto t2 = qap::bm::insert(c2, gen2, n);
    std::cout << t2 << std::endl;
    print_dif(t1, t2);
    std::cout << "---------------------" << std::endl;

    std::cout << "Iterating over all elements:" << std::endl;
    _(typename1, qap::color::cyan) << std::endl;
    auto t3 = qap::bm::iterate(c1);
    std::cout << t3 << std::endl;
    _(typename2, qap::color::cyan) << std::endl;
    auto t4 = qap::bm::iterate(c2);
    std::cout << t4 << std::endl;
    print_dif(t3, t4);
    std::cout << "---------------------" << std::endl;

    std::cout << "Copying containers:" << std::endl;
    _(typename1, qap::color::cyan) << std::endl;
    auto t5 = qap::bm::copy(c1);
    std::cout << t5 << std::endl;
    _(typename2, qap::color::cyan) << std::endl;
    auto t6 = qap::bm::copy(c2);
    std::cout << t6 << std::endl;
    print_dif(t5, t6);
    std::cout << "---------------------" << std::endl;

    std::cout << "Moving containers:" << std::endl;
    _(typename1, qap::color::cyan) << std::endl;
    auto t7 = qap::bm::move(c1);
    std::cout << t7 << std::endl;
    _(typename2, qap::color::cyan) << std::endl;
    auto t8 = qap::bm::move(c2);
    std::cout << t8 << std::endl;
    print_dif(t7, t8);
    std::cout << "---------------------" << std::endl;

    std::cout << "Finding " << n << " elements" << std::endl;
    _(typename1, qap::color::cyan) << std::endl;
    auto t9 = qap::bm::find(c1, find1, n);
    std::cout << t9 << std::endl;
    _(typename2, qap::color::cyan) << std::endl;
    auto t10 = qap::bm::find(c2, find2, n);
    std::cout << t10 << std::endl;
    print_dif(t9, t10);
    std::cout << "---------------------" << std::endl;
}

} // bm namespace

} // qap namespace

#endif // include guard
