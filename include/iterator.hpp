#ifndef ITERATOR_HPP
#define ITERATOR_HPP

// https://www.cplusplus.com/reference/iterator/
// https://www.cplusplus.com/reference/iterator/iterator_traits/
// https://www.fluentcpp.com/2018/05/08/std-iterator-deprecated/

#include "cstddef.hpp"

namespace ft
{
    /* -------------------------------------------------------------------------- */
    /*                             // iterator_tags                               */
    /* -------------------------------------------------------------------------- */
    typedef std::input_iterator_tag input_iterator_tag;
    typedef std::output_iterator_tag output_iterator_tag;
    typedef std::forward_iterator_tag forward_iterator_tag;
    typedef std::bidirectional_iterator_tag bidirectional_iterator_tag;
    typedef std::random_access_iterator_tag random_access_iterator_tag;

    /* -------------------------------------------------------------------------- */
    /*                             // iterator_traits                             */
    /* -------------------------------------------------------------------------- */
    template <class Iterator>
    struct iterator_traits
    {
    public:
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
        typedef typename Iterator::iterator_category iterator_category;
    };
    template <class T>
    struct iterator_traits<T *>
    {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef std::random_access_iterator_tag iterator_category;
    };
    template <class T>
    struct iterator_traits<T *const>
    {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T *pointer;
        typedef const T &reference;
        typedef std::random_access_iterator_tag iterator_category;
    };

    /* -------------------------------------------------------------------------- */
    /*                             // reverse_iterator                            */
    /* -------------------------------------------------------------------------- */
    template <class Iterator>
    class reverse_iterator
    {

    public:
        typedef Iterator iterator_type;
        typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
        typedef typename iterator_traits<Iterator>::value_type value_type;
        typedef typename iterator_traits<Iterator>::difference_type difference_type;
        typedef typename iterator_traits<Iterator>::pointer pointer;
        typedef typename iterator_traits<Iterator>::reference reference;

    private:
        iterator_type iter;

    public:
        // * Default constructor
        reverse_iterator() : iter() {}                            // defailt constructor
        explicit reverse_iterator(iterator_type it) : iter(it) {} // initialize with iterator
        template <class Iter>
        reverse_iterator(const reverse_iterator<Iter> &rev_it) : iter(rev_it.base()) {}

        // return iterator
        iterator_type base() const { return iter; }

        // operator reverse_iterator<const Iterator>() const { return iter; }

        // reference to the element pointed
        reference operator*() const
        {
            Iterator tmp = iter;
            return *--tmp;
        }

        // Returns a reverse iterator pointing to the element located n positions away from the element the iterator currently points to
        reverse_iterator operator+(difference_type n) const { return reverse_iterator(iter - n); }

        // decrements the base iterator
        reverse_iterator &operator++()
        {
            --iter;
            return *this;
        }

        reverse_iterator operator++(int)
        {
            reverse_iterator tmp(*this);
            --iter;
            return tmp;
        }

        // function decreases by n
        reverse_iterator &operator+=(difference_type n)
        {
            iter -= n;
            return *this;
        }

        // applies the binary operator+ on the base iterator and returns a reverse iterator
        reverse_iterator operator-(difference_type n) const { return reverse_iterator(iter + n); }
        ft::ptrdiff_t operator-(const reverse_iterator &x) const { return (x.base() - iter); }

        // increments the base iterator
        reverse_iterator &operator--()
        {
            ++iter;
            return *this;
        }
        reverse_iterator operator--(int)
        {
            reverse_iterator tmp(*this);
            ++iter;
            return tmp;
        }

        // increases by n the base iterator
        reverse_iterator &operator-=(difference_type n)
        {
            iter += n;
            return *this;
        }

        // pointer to the element pointed
        pointer operator->() const { return &(operator*()); }

        // Accesses the element located n positions
        reference operator[](difference_type n) const { return *(*this + n); }
    };

    //* Arithmetic operators (+ and -) =================================================================
    template <typename It>
    reverse_iterator<It> operator+(typename reverse_iterator<It>::difference_type n, reverse_iterator<It> const &rev_it)
    {
        return (reverse_iterator<It>(rev_it.base() - n));
    }

    // *  Relational operators (==, != <, >, <= and >=) =================================================================================================
    template <typename It1, typename It2>
    typename reverse_iterator<It1>::difference_type operator==(const reverse_iterator<It1> &lhs, const reverse_iterator<It2> &rhs) { return lhs.base() == rhs.base(); }

    template <typename It1, typename It2>
    bool operator!=(const reverse_iterator<It1> &lhs, const reverse_iterator<It2> &rhs) { return !(lhs.base() == rhs.base()); }

    template <typename It1, typename It2>
    bool operator<(const reverse_iterator<It1> &lhs, const reverse_iterator<It2> &rhs) { return lhs.base() > rhs.base(); }

    template <typename It1, typename It2>
    bool operator<=(const reverse_iterator<It1> &lhs, const reverse_iterator<It2> &rhs) { return lhs.base() >= rhs.base(); }

    template <typename It1, typename It2>
    bool operator>(const reverse_iterator<It1> &lhs, const reverse_iterator<It2> &rhs) { return lhs.base() < rhs.base(); }

    template <typename It1, typename It2>
    bool operator>=(const reverse_iterator<It1> &lhs, const reverse_iterator<It2> &rhs) { return lhs.base() <= rhs.base(); }

}

#endif
