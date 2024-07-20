#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

namespace ft
{
    typedef long ptrdiff_t;
    typedef unsigned long size_t;

    template <class Arg1, class Arg2, class Result>
    struct binary_function
    {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };

    template <class T>
    struct less : binary_function<T, T, bool>
    {
        bool operator()(const T &x, const T &y) const { return x < y; }
    };

    template <typename Key, typename Value>
    struct less<ft::pair<Key, Value> >
    {
        bool operator()(const ft::pair<Key, Value> &lhs, const ft::pair<Key, Value> &rhs) const
        {
            return lhs.first < rhs.first;
        }
    };
}

#endif