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
        /**
         * Comparison operator
         * 
         * @param x First value to compare
         * @param y Second value to compare
         * @return True if x is less than y, false otherwise
         * 
         * @throws None
         * @complexity O(1)
         */
        bool operator()(const T &x, const T &y) const { return x < y; }
    };

    template <typename Key, typename Value>
    struct less<ft::pair<Key, Value> >
    {
        /**
         * Comparison operator for pairs
         * 
         * @param lhs First pair to compare
         * @param rhs Second pair to compare
         * @return True if lhs.first is less than rhs.first, false otherwise
         * 
         * @throws None
         * @complexity O(1)
         */
        bool operator()(const ft::pair<Key, Value> &lhs, const ft::pair<Key, Value> &rhs) const
        {
            return lhs.first < rhs.first;
        }
    };
}

#endif