#ifndef SET_HPP
#define SET_HPP

#include "utility.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"
#include "exception.hpp"
#include "type_traits.hpp"
#include "string.hpp"

namespace ft
{
    template <class T, class Compare = ft::less<T>, class Alloc = std::allocator<T>>
    class set
    {
    public:
        struct bt_node
        {
            bt_node *left;
            bt_node *right;
            bt_node *parent;

            ft::pair<const Key, T> data;

            bt_node(ft::pair<const Key, T> data) : left(NULL), right(NULL), parent(NULL), data(data) {}
            const Key &getKey() { return data.first; }
            T &getVal() { return data.second; }
        };

    public:
        typedef T key_type;
        typedef key_type value_type;
        typedef Compare key_compare;
        typedef Compare value_compare;
        typedef Alloc allocator_type;
        typedef typename Alloc::reference reference;
        typedef typename Alloc::const_reference const_reference;
        typedef typename Alloc::pointer pointer;
        typedef typename Alloc::const_pointer const_pointer;
        typedef ft::map_iterator<key_type, value_type, key_compare, Alloc> iterator;
        typedef ft::map_iterator<key_type, value_type, key_compare, Alloc> const_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef typename Alloc::difference_type difference_type;
        typedef typename Alloc::size_type size_type;

    private:
    public:

        // constructors
        explicit set(const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type());

        template <class InputIterator>
        set(InputIterator first, InputIterator last, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type());

        set(const set &x);


    };
}

#endif
