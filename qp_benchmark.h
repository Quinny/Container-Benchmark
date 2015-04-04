#ifndef QP_BENCH_MARK_H_
#define QP_BENCH_MARK_H_

#include <chrono>
#include <iostream>
#include <cstddef> // for size_t
#include <type_traits>
#include "qp_traits.h"
#include "qp_colors.h"

namespace qap {

namespace bm {

using namespace std::chrono;
qap::color::writer _(std::cout);

// Takes a container and a generating function (no arguments)
// and performs n inserts on the container with the output of the generating
// function.  The time taken to do the inserts is reported in seconds.
//
//
// enable_if is used to determine whether the container has a push_back
// or insert function, and calls the appropriate one.  This is pretty cool
// because you can call the same function on a wide variety of containers
//
// If both push_back and insert functions exist, push_back will be used
// The reason for this is all the standard sequence containers have both
// push_back and insert functions, but no stanard associative container
// has a push_back function

// ---------------------------------------
// Insert Benchmarks
// --------------------------------------
template <typename Container, typename Gen>
typename std::enable_if<
    qap::traits::has_insert<Container>::value &&
    !qap::traits::has_push_back<Container>::value,
    double
>::type
insert(Container& c, Gen g, std::size_t n) {
    auto t1 = high_resolution_clock::now();
    while (n--)
        c.insert(g());
    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << time_span.count() << std::endl;
    return time_span.count();
}

// Insert version for push_back containers
template <typename Container, typename Gen>
typename std::enable_if<
    qap::traits::has_push_back<Container>::value &&
    !qap::traits::has_push<Container>::value,
    double
>::type
insert(Container& c, Gen g, std::size_t n) {
    auto t1 = high_resolution_clock::now();
    while (n--)
        c.push_back(g());
    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << time_span.count() << std::endl;
    return time_span.count();
}

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
    std::cout << time_span.count() << std::endl;
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
iterate(Container& c) {
    auto t1 = high_resolution_clock::now();
    for (auto i : c)
    ; // suppress warnings with semi colon on next line
    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << time_span.count() << std::endl;
    return time_span.count();
}

template <typename Container>
typename std::enable_if<
    !qap::traits::has_begin<Container>::value,
    double
>::type
iterate(Container& c) {
    _("No defined begin() function", qap::color::red) << std::endl;
    return 0;
}

// copy the container
template <typename Container>
double copy(Container const& c) {
    auto t1 = high_resolution_clock::now();
    auto c2 = c;
    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << time_span.count() << std::endl;
    return time_span.count();
}

// move the container
template <typename Container>
double move(Container const& c) {
    auto t1 = high_resolution_clock::now();
    auto c2 = std::move(c);
    auto t2 = high_resolution_clock::now();
    auto time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << time_span.count() << std::endl;
    return time_span.count();
}


void print_dif(double t1, double t2) {
    double dif = t1 - t2;
    std::cout << "Dif: ";
    if (dif < 0)
        _(dif, qap::color::green);
    else
        _(dif, qap::color::red);
    std::cout << std::endl;
}


// Compare the operatings of two different containers
// 2 generating functions are required (one for each container)
template <typename Container1, typename Container2, typename Gen1, typename Gen2>
void compare_all(Container1& c1, Gen1 f, Container2& c2, Gen2 g, std::size_t n) {
    std::cout << "Inserting " << n << " elements:" << std::endl;
    
    std::cout << "Container 1" << std::endl;
    auto t1 = qap::bm::insert(c1, f, n);
    std::cout << "Container 2" << std::endl;
    auto t2 = qap::bm::insert(c2, g, n);
    print_dif(t1, t2);
    std::cout << "---------------------" << std::endl;

    std::cout << "Iterating over all elements:" << std::endl;
    std::cout << "Container 1" << std::endl;
    auto t3 = qap::bm::iterate(c1);
    std::cout << "Container 2" << std::endl;
    auto t4 = qap::bm::iterate(c2);
    print_dif(t3, t4);
    std::cout << "---------------------" << std::endl;

    std::cout << "Copying containers:" << std::endl;
    std::cout << "Container 1" << std::endl;
    auto t5 = qap::bm::copy(c1);
    std::cout << "Container 2" << std::endl;
    auto t6 = qap::bm::copy(c2);
    print_dif(t5, t6);
    std::cout << "---------------------" << std::endl;

    std::cout << "Moving containers:" << std::endl;
    std::cout << "Container 1" << std::endl;
    auto t7 = qap::bm::move(c1);
    std::cout << "Container 2" << std::endl;
    auto t8 = qap::bm::move(c2);
    print_dif(t7, t8);
    std::cout << "---------------------" << std::endl;
}

} // bm namespace

} // qap namespace

#endif // include guard
