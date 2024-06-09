#ifndef UTILITY_HPP
#define UTILITY_HPP

namespace ft
{
    // pair
    template <class T1, class T2>
    struct pair
    {
        typedef T1 first_type;
        typedef T2 second_type;

        first_type first;
        second_type second;

        pair() : first(), second() {}
        pair(const T1 &x, const T2 &y) : first(x), second(y) {}

        template <class U1, class U2>
        pair(const pair<U1, U2> &p) : first(p.first), second(p.second) {}

        pair &operator=(const pair &src)
        {
            first = src.first;
            second = src.second;
            return *this;
        }
    };

    template <class T1, class T2>
    pair<T1, T2> make_pair(T1 x1, T2 x2)
    {
        return pair<T1, T2>(x1, x2);
    }

    template <class T1, class T2>
    bool operator==(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        return (lhs.first == rhs.first && lhs.second == rhs.second);
    }

    template <class T1, class T2>
    bool operator<(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        return (lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second));
    }

    template <class T1, class T2>
    bool operator!=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        return (!(lhs == rhs));
    }

    template <class T1, class T2>
    bool operator<=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        return (!(rhs < lhs));
    }

    template <class T1, class T2>
    bool operator>(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        return (rhs < lhs);
    }

    template <class T1, class T2>
    bool operator>=(const pair<T1, T2> &lhs, const pair<T1, T2> &rhs)
    {
        return (!(lhs < rhs));
    }

    template <typename T>
    struct is_integral
    {
        static const bool value = false;
    };

    template <>
    struct is_integral<bool>
    {
        static const bool value = true;
    };

    template <>
    struct is_integral<char>
    {
        static const bool value = true;
    };

    template <>
    struct is_integral<unsigned char>
    {
        static const bool value = true;
    };

    template <>
    struct is_integral<int>
    {
        static const bool value = true;
    };

    template <>
    struct is_integral<unsigned int>
    {
        static const bool value = true;
    };

    template <>
    struct is_integral<long>
    {
        static const bool value = true;
    };
    template <>
    struct is_integral<long unsigned>
    {
        static const bool value = true;
    };
    template <>
    struct is_integral<short>
    {
        static const bool value = true;
    };

    template <>
    struct is_integral<short unsigned>
    {
        static const bool value = true;
    };

    template <>
    struct is_integral<long long>
    {
        static const bool value = true;
    };

    template <>
    struct is_integral<long long unsigned>
    {
        static const bool value = true;
    };

    template <class T>
	void swap (T & a, T & b)
	{
		T	tmp(a);
		a = b;
		b = tmp;
	}

}

#endif