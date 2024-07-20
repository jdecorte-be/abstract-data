#ifndef MULTIMAP_HPP
#define MULTIMAP_HPP

#include "utility.hpp" // for pair
#include "iterator.hpp" // for iterator types
#include "algorithm.hpp" // if needed for any algorithms
#include "exception.hpp" // for exception handling
#include "tree.hpp" // Red-Black tree as underlying structure

namespace ft
{
    template <class Key, class T, class Compare = ft::less<Key>, class Alloc = std::allocator<ft::pair<const Key, T> > >
    class multimap
    {
    public:
        typedef Key key_type;
        typedef T mapped_type;
        typedef ft::pair<const key_type, mapped_type> value_type;
        typedef Compare key_compare;
        typedef typename Alloc::template rebind<bt_node<value_type> >::other allocator_type;

        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef ft::map_iterator<Key, T, bt_node<value_type> > iterator;
        typedef ft::map_iterator<Key, T, const bt_node<value_type> > const_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef typename iterator::difference_type difference_type;
        typedef typename iterator::size_type size_type;

        class value_compare
        {
            friend class multimap;
        protected:
            key_compare comp;
            value_compare(key_compare c) : comp(c) {}
        public:
            bool operator()(const value_type& x, const value_type& y) const
            {
                return comp(x.first, y.first);
            }
        };

    private:
        RedBlackTree<value_type, Compare, allocator_type> tree;

    public:
        explicit multimap(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
            : tree(comp, alloc) {}

        template <class InputIt>
        multimap(InputIt first, InputIt last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
            : tree(comp, alloc)
        {
            insert(first, last);
        }

        multimap(const multimap& other) : tree(other.tree._comp, other.tree._alloc)
        {
            insert(other.begin(), other.end());
        }

        ~multimap() { clear(); }

        multimap& operator=(const multimap& other)
        {
            if (this != &other)
            {
                clear();
                tree = other.tree;
                insert(other.begin(), other.end());
            }
            return *this;
        }

        iterator begin() { return iterator(tree.left_most(tree.root)); }
        const_iterator begin() const { return const_iterator(tree.left_most(tree.root)); }
        iterator end() { return iterator(NULL, tree.right_most(tree.root)); }
        const_iterator end() const { return const_iterator(NULL, tree.right_most(tree.root)); }

        reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
        reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

        bool empty() const { return begin() == end(); }
        size_type size() const
        {
            size_type n = 0;
            for (const_iterator it = begin(); it != end(); ++it)
                n++;
            return n;
        }
        size_type max_size() const { return tree._alloc.max_size(); }

        iterator insert(const value_type& val)
        {
            return iterator(tree.insert_node(val));
        }

        template <class InputIterator>
        void insert(InputIterator first, InputIterator last)
        {
            while (first != last)
            {
                insert(*first++);
            }
        }

        void erase(iterator pos) { if (pos != end()) tree.delete_node(pos.base()); }
        size_type erase(const key_type& k)
        {
            size_type count = 0;
            iterator it = find(k);
            while (it != end() && it->first == k)
            {
                erase(it++);
                count++;
            }
            return count;
        }
        void erase(iterator first, iterator last)
        {
            while (first != last)
                erase(first++);
        }

        iterator find(const key_type& k) { return iterator(tree.find_node(k)); }
        const_iterator find(const key_type& k) const { return const_iterator(tree.find_node(k)); }

        size_type count(const key_type& k) const
        {
            size_type n = 0;
            const_iterator it = find(k);
            while (it != end() && it->first == k) {
                ++n;
                ++it;
            }
            return n;
        }

        iterator lower_bound(const key_type& key) { return find(key); }
        const_iterator lower_bound(const key_type& key) const { return find(key); }
        iterator upper_bound(const key_type& key)
        {
            iterator it = lower_bound(key);
            while (it != end() && it->first == key) ++it;
            return it;
        }
        const_iterator upper_bound(const key_type& key) const
        {
            const_iterator it = lower_bound(key);
            while (it != end() && it->first == key) ++it;
            return it;
        }

        ft::pair<iterator, iterator> equal_range(const key_type& key)
        {
            return ft::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
        }
        ft::pair<const_iterator, const_iterator> equal_range(const key_type& key) const
        {
            return ft::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
        }

        void clear()
        {
            if (tree.root)
            {
                tree.delete_tree(tree.root);
                tree.root = NULL;
            }
        }

        void swap(multimap& x)
        {
            ft::swap(tree.root, x.tree.root);
            ft::swap(tree._comp, x.tree._comp);
            ft::swap(tree._alloc, x.tree._alloc);
        }

        key_compare key_comp() const { return tree._comp; }
        value_compare value_comp() const { return value_compare(tree._comp); }
    };

    template <class Key, class T, class Compare, class Alloc>
    bool operator==(const ft::multimap<Key, T, Compare, Alloc>& lhs, const ft::multimap<Key, T, Compare, Alloc>& rhs)
    {
        if (lhs.size() != rhs.size()) return false;
        return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <class Key, class T, class Compare, class Alloc>
    bool operator!=(const ft::multimap<Key, T, Compare, Alloc>& lhs, const ft::multimap<Key, T, Compare, Alloc>& rhs)
    {
        return !(lhs == rhs);
    }

} // namespace ft

#endif
