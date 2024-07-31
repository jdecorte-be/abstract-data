#ifndef UTILITY_HPP
#define UTILITY_HPP

namespace ft
{
    // pair
    /**
     * A template structure to hold a pair of values.
     * 
     * @tparam T1 Type of the first value
     * @tparam T2 Type of the second value
     * 
     * @complexity O(1)
     */
    template <class T1, class T2>
    struct pair
    {
        typedef T1 first_type;
        typedef T2 second_type;

        first_type first;
        second_type second;

        /**
         * Default constructor. Initializes first and second to their default values.
         * 
         * @complexity O(1)
         */
        pair() : first(), second() {}

        /**
         * Constructs a pair with the given values.
         * 
         * @param x First value
         * @param y Second value
         * 
         * @complexity O(1)
         */
        pair(const T1 &x, const T2 &y) : first(x), second(y) {}

        /**
         * Constructs a pair from another pair of possibly different types.
         * 
         * @tparam U1 Type of the first value in the other pair
         * @tparam U2 Type of the second value in the other pair
         * @param p The other pair
         * 
         * @complexity O(1)
         */
        template <class U1, class U2>
        pair(const pair<U1, U2> &p) : first(p.first), second(p.second) {}

        /**
         * Assigns the values from another pair to this pair.
         * 
         * @param src The other pair
         * @return Reference to this pair
         * 
         * @complexity O(1)
         */
        pair &operator=(const pair &src)
        {
            first = src.first;
            second = src.second;
            return *this;
        }
    };

    /**
     * Creates a pair from two values.
     * 
     * @tparam T1 Type of the first value
     * @tparam T2 Type of the second value
     * @param x1 First value
     * @param x2 Second value
     * @return A pair containing the two values
     * 
     * @complexity O(1)
     */
    template <class T1, class T2>
    pair<T1, T2> make_pair(T1 x1, T2 x2)
    {
        return pair<T1, T2>(x1, x2);
    }

    /**
     * Checks if two pairs are equal.
     * 
     * @tparam T1 Type of the first value in the pairs
     * @tparam T2 Type of the second value in the pairs
     * @param lhs Left-hand side pair
     * @param rhs Right-hand side pair
     * @return True if the pairs are equal, false otherwise
     * 
     * @complexity O(1)
     */
    template <class T1, class T2>
    bool operator==(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        return (lhs.first == rhs.first && lhs.second == rhs.second);
    }

    /**
     * Checks if one pair is less than another.
     * 
     * @tparam T1 Type of the first value in the pairs
     * @tparam T2 Type of the second value in the pairs
     * @param lhs Left-hand side pair
     * @param rhs Right-hand side pair
     * @return True if lhs is less than rhs, false otherwise
     * 
     * @complexity O(1)
     */
    template <class T1, class T2>
    bool operator<(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        return (lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second));
    }

    /**
     * Checks if two pairs are not equal.
     * 
     * @tparam T1 Type of the first value in the pairs
     * @tparam T2 Type of the second value in the pairs
     * @param lhs Left-hand side pair
     * @param rhs Right-hand side pair
     * @return True if the pairs are not equal, false otherwise
     * 
     * @complexity O(1)
     */
    template <class T1, class T2>
    bool operator!=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        return (!(lhs == rhs));
    }

    /**
     * Checks if one pair is less than or equal to another.
     * 
     * @tparam T1 Type of the first value in the pairs
     * @tparam T2 Type of the second value in the pairs
     * @param lhs Left-hand side pair
     * @param rhs Right-hand side pair
     * @return True if lhs is less than or equal to rhs, false otherwise
     * 
     * @complexity O(1)
     */
    template <class T1, class T2>
    bool operator<=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        return (!(rhs < lhs));
    }

    /**
     * Checks if one pair is greater than another.
     * 
     * @tparam T1 Type of the first value in the pairs
     * @tparam T2 Type of the second value in the pairs
     * @param lhs Left-hand side pair
     * @param rhs Right-hand side pair
     * @return True if lhs is greater than rhs, false otherwise
     * 
     * @complexity O(1)
     */
    template <class T1, class T2>
    bool operator>(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        return (rhs < lhs);
    }

    /**
     * Checks if one pair is greater than or equal to another.
     * 
     * @tparam T1 Type of the first value in the pairs
     * @tparam T2 Type of the second value in the pairs
     * @param lhs Left-hand side pair
     * @param rhs Right-hand side pair
     * @return True if lhs is greater than or equal to rhs, false otherwise
     * 
     * @complexity O(1)
     */
    template <class T1, class T2>
    bool operator>=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        return (!(lhs < rhs));
    }

    /**
     * Checks if a type is an integral type.
     * 
     * @tparam T Type to check
     * @return value True if T is an integral type, false otherwise
     * 
     * @complexity O(1)
     */
    template <typename T>
    struct is_integral
    {
        static const bool value = false;
    };

    /**
     * Specialization of is_integral for bool.
     * 
     * @return value True if T is bool
     * 
     * @complexity O(1)
     */
    template <>
    struct is_integral<bool>
    {
        static const bool value = true;
    };

    /**
     * Specialization of is_integral for char.
     * 
     * @return value True if T is char
     * 
     * @complexity O(1)
     */
    template <>
    struct is_integral<char>
    {
        static const bool value = true;
    };

    /**
     * Specialization of is_integral for unsigned char.
     * 
     * @return value True if T is unsigned char
     * 
     * @complexity O(1)
     */
    template <>
    struct is_integral<unsigned char>
    {
        static const bool value = true;
    };

    /**
     * Specialization of is_integral for int.
     * 
     * @return value True if T is int
     * 
     * @complexity O(1)
     */
    template <>
    struct is_integral<int>
    {
        static const bool value = true;
    };

    /**
     * Specialization of is_integral for unsigned int.
     * 
     * @return value True if T is unsigned int
     * 
     * @complexity O(1)
     */
    template <>
    struct is_integral<unsigned int>
    {
        static const bool value = true;
    };

    /**
     * Specialization of is_integral for long.
     * 
     * @return value True if T is long
     * 
     * @complexity O(1)
     */
    template <>
    struct is_integral<long>
    {
        static const bool value = true;
    };

    /**
     * Specialization of is_integral for unsigned long.
     * 
     * @return value True if T is unsigned long
     * 
     * @complexity O(1)
     */
    template <>
    struct is_integral<long unsigned>
    {
        static const bool value = true;
    };

    /**
     * Specialization of is_integral for short.
     * 
     * @return value True if T is short
     * 
     * @complexity O(1)
     */
    template <>
    struct is_integral<short>
    {
        static const bool value = true;
    };

    /**
     * Specialization of is_integral for unsigned short.
     * 
     * @return value True if T is unsigned short
     * 
     * @complexity O(1)
     */
    template <>
    struct is_integral<short unsigned>
    {
        static const bool value = true;
    };

    /**
     * Specialization of is_integral for long long.
     * 
     * @return value True if T is long long
     * 
     * @complexity O(1)
     */
    template <>
    struct is_integral<long long>
    {
        static const bool value = true;
    };

    /**
     * Specialization of is_integral for unsigned long long.
     * 
     * @return value True if T is unsigned long long
     * 
     * @complexity O(1)
     */
    template <>
    struct is_integral<long long unsigned>
    {
        static const bool value = true;
    };

    /**
     * Swaps the values of two variables.
     * 
     * @tparam T Type of the variables
     * @param a First variable
     * @param b Second variable
     * 
     * @complexity O(1)
     */
    template <class T>
    void swap(T &a, T &b)
    {
        T tmp(a);
        a = b;
        b = tmp;
    }

}

#endif
