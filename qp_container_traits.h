#ifndef QP_CONTAINER_TRAITS_H_
#define QP_CONTAINER_TRAITS_H_
#include <type_traits>

// Quinn Perfetto 2015
//
// Type traits for identifing different types of containers and functions
// provided by them.  They are used for deciding which functions to use as
// benchmarks

namespace qap {

namespace traits {

// Types to represent true and false values
// on almost every compiler these will be different sizes
using true_t = char;
using false_t = struct {char _[2]; };

// But just in case...
static_assert(sizeof(true_t) != sizeof(false_t),
        "ERROR! true and false types have same size!");

// Check if a class has a push_back
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

// Check if a class has a push
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

// Generally, only sequence containers have a push_back function
template <typename T>
struct is_sequence_container {
    enum { value = has_push_back<T>::value };
};

// Check if an insert function exists
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

// We cannot solely rely on the insert function to deduce if a container
// is associative (map, unordered_map, set, etc).  The reason is most sequence
// containers have an insert function, and the call will be ambiguous
template <typename T>
struct is_associative {
    enum { value = has_insert<T>::value && !has_push_back<T>::value };
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

// Check if a find function exists for look ups
template<typename T>
class has_find {
    struct Fallback { int find; };
    struct Derived : T, Fallback { };

    template<typename U, U> struct Check;

    template<typename U>
    static false_t test(Check<int Fallback::*, &U::find> *);

    template<typename U>
    static true_t test(...);

    public:
    enum { value = sizeof(test<Derived>(0)) == sizeof(true_t) };
};


// Recognizing pair types
//
// If a type is non-fundamental, and has
// first and second members, then it is a pair
template<typename T>
class has_first {
    struct Fallback { int first; };
    struct Derived : T, Fallback { };

    template<typename U, U> struct Check;

    template<typename U>
    static false_t test(Check<int Fallback::*, &U::first> *);

    template<typename U>
    static true_t test(...);

    public:
    enum { value = sizeof(test<Derived>(0)) == sizeof(true_t) };
};

template<typename T>
class has_second {
    struct Fallback { int second; };
    struct Derived : T, Fallback { };

    template<typename U, U> struct Check;

    template<typename U>
    static false_t test(Check<int Fallback::*, &U::second> *);

    template<typename U>
    static true_t test(...);

    public:
    enum { value = sizeof(test<Derived>(0)) == sizeof(true_t) };
};


// if a type is non-fundamental (not a primitive type)
// and has first and second members, then it is assumed
// to be a pair type
template <typename T, bool B>
struct is_pair_type_impl {
    enum {
        value = has_first<T>::value &&
                has_second<T>::value
    };
};


template <typename T>
struct is_pair_type_impl<T, true> {
    enum { value = 0 };
};

template <typename T>
struct is_pair_type {
    enum {
        value = is_pair_type_impl<
            T, std::is_fundamental<T>::value
        >::value
    };
};


} // namespace traits

} // namespace qap
#endif // include guards
