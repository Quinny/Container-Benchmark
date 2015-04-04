#ifndef QP_CONTAINER_TRAITS_H_
#define QP_CONTAINER_TRAITS_H_
#include <type_traits>

namespace qap {

namespace traits {

// Types to represent true and false values
// on almost every compiler these will be different sizes
using true_t = char;
using false_t = struct {char _[2]; };

// But just in case...
static_assert(sizeof(true_t) != sizeof(false_t),
        "ERROR! true and false types have same size!");

// Checks if a given type has a push_back function
// with argument T::value_type
//
// Useful for identifying sequence containers
template<typename T>
class has_push_back {
    struct Fallback { int push_back; };
    struct Derived : T, Fallback { };

    template<typename U, U> struct Check;

    template<typename U>
    static false_t test(Check<int Fallback::*, &U::push_back> *);

    template<typename U>
    static true_t test(...);

    public:
    enum { value = sizeof(test<Derived>(0)) == sizeof(true_t) };
};

template<typename T>
class has_insert {
    struct Fallback { int insert; };
    struct Derived : T, Fallback { };

    template<typename U, U> struct Check;

    template<typename U>
    static false_t test(Check<int Fallback::*, &U::insert> *);

    template<typename U>
    static true_t test(...);

    public:
    enum { value = sizeof(test<Derived>(0)) == sizeof(true_t) };
};

template<typename T>
class has_push {
    struct Fallback { int push; };
    struct Derived : T, Fallback { };

    template<typename U, U> struct Check;

    template<typename U>
    static false_t test(Check<int Fallback::*, &U::push> *);

    template<typename U>
    static true_t test(...);

    public:
    enum { value = sizeof(test<Derived>(0)) == sizeof(true_t) };
};

// Check to see if a class has a begin() function
// Used to check whether the class can be iterated over
template<typename T>
class has_begin {
    struct Fallback { int begin; };
    struct Derived : T, Fallback { };

    template<typename U, U> struct Check;

    template<typename U>
    static false_t test(Check<int Fallback::*, &U::begin> *);

    template<typename U>
    static true_t test(...);

    public:
    enum { value = sizeof(test<Derived>(0)) == sizeof(true_t) };
};

} // namespace traits

} // namespace qap
#endif // include guards
