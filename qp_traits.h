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
