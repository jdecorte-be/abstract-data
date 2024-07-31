#ifndef SET_HPP
#define SET_HPP

#include "utility.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"
#include "exception.hpp"
#include "type_traits.hpp"
#include "__tree.hpp"

namespace ft
{

    template <class T, class Compare = ft::less<T>, class Alloc = std::allocator<T> >
    class set
    {
    public:
        //
        // Member types
        //
        typedef T key_type;
        typedef key_type value_type;
        typedef Compare key_compare;
        typedef Alloc allocator_type;
        typedef typename Alloc::reference reference;
        typedef typename Alloc::const_reference const_reference;
        typedef typename Alloc::pointer pointer;
        typedef typename Alloc::const_pointer const_pointer;
        typedef ft::set_iterator<key_type, bt_node<key_type> > iterator;
        typedef ft::set_iterator<const key_type, const bt_node<key_type> > const_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef typename Alloc::difference_type difference_type;
        typedef typename Alloc::size_type size_type;

        //
        // Value comp (nested class)
        //

        class value_compare
        {
            friend class set;

        protected:
            key_compare comp;
            value_compare(key_compare c) : comp(c) {}

        public:
            typedef value_type first_argument_type;
            typedef value_type second_argument_type;
            bool operator()(const first_argument_type &x, const second_argument_type &y) const { return comp(x, y); }
        };

    private:
        RedBlackTree<value_type, Compare, allocator_type> tree;

    public:
        //
        // Constructor
        //
        /**
         * Constructs an empty set with the given comparison object and allocator.
         * 
         * @param comp Comparison function object
         * @param alloc Allocator object
         * 
         * @complexity O(1)
         */
        explicit set(const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
            : tree(comp, alloc) {}

        /**
         * Constructs a set with the elements in the range [first, last).
         * 
         * @param first Iterator to the first element in the range
         * @param last Iterator to the last element in the range
         * @param comp Comparison function object
         * @param alloc Allocator object
         * 
         * @complexity O(n log n)
         */
        template <class InputIt>
        set(InputIt first, InputIt last, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
            : tree(comp, alloc)
        {
            insert(first, last);
        }

        /**
         * Copy constructor.
         * 
         * @param other Another set to be copied
         * 
         * @complexity O(n log n)
         */
        set(const set &other) : tree(other.tree._comp, other.tree._alloc)
        {
            clear();
            insert(other.begin(), other.end());
        }

        /**
         * Destructor. Clears the set.
         * 
         * @complexity O(n)
         */
        ~set() { clear(); }

        /**
         * Copy assignment operator.
         * 
         * @param other Another set to be assigned
         * @return Reference to the assigned set
         * 
         * @complexity O(n log n)
         */
        set &operator=(const set &other)
        {
            if (this != &other)
            {
                set tmp(other);
                this->swap(tmp);
            }
            return *this;
        }

        //
        // Iterators
        //
        /**
         * Returns an iterator to the first element.
         * 
         * @return Iterator to the first element
         * 
         * @complexity O(log n)
         */
        iterator begin()
        {
            return iterator(tree.left_most(tree.root));
        }

        /**
         * Returns a const iterator to the first element.
         * 
         * @return Const iterator to the first element
         * 
         * @complexity O(log n)
         */
        const_iterator begin() const
        {
            return const_iterator(tree.left_most(tree.root));
        }

        /**
         * Returns an iterator to the element following the last element.
         * 
         * @return Iterator to the element following the last element
         * 
         * @complexity O(log n)
         */
        iterator end()
        {
            return iterator(NULL, tree.right_most(tree.root));
        }

        /**
         * Returns a const iterator to the element following the last element.
         * 
         * @return Const iterator to the element following the last element
         * 
         * @complexity O(log n)
         */
        const_iterator end() const
        {
            return const_iterator(NULL, tree.right_most(tree.root));
        }

        //
        // Reverse Iterators
        //
        /**
         * Returns a reverse iterator to the first element of the reversed set.
         * 
         * @return Reverse iterator to the first element
         * 
         * @complexity O(1)
         */
        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }

        /**
         * Returns a const reverse iterator to the first element of the reversed set.
         * 
         * @return Const reverse iterator to the first element
         * 
         * @complexity O(1)
         */
        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }

        /**
         * Returns a reverse iterator to the element following the last element of the reversed set.
         * 
         * @return Reverse iterator to the element following the last element
         * 
         * @complexity O(1)
         */
        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }

        /**
         * Returns a const reverse iterator to the element following the last element of the reversed set.
         * 
         * @return Const reverse iterator to the element following the last element
         * 
         * @complexity O(1)
         */
        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

        //
        // Capacity
        //
        /**
         * Checks if the set is empty.
         * 
         * @return True if the set is empty, false otherwise
         * 
         * @complexity O(1)
         */
        bool empty() const
        {
            return begin() == end();
        }

        /**
         * Returns the number of elements in the set.
         * 
         * @return Number of elements in the set
         * 
         * @complexity O(n)
         */
        size_type size() const
        {
            size_type n = 0;
            for (const_iterator it = begin(); it != end(); ++it)
                n++;
            return n;
        }

        /**
         * Returns the maximum possible number of elements in the set.
         * 
         * @return Maximum possible number of elements
         * 
         * @complexity O(1)
         */
        size_type max_size() const
        {
            return tree._alloc.max_size();
        }

        //
        // Modifiers
        //
        /**
         * Clears the set, removing all elements.
         * 
         * @complexity O(n)
         */
        void clear()
        {
            if (tree.root)
            {
                delete_tree(tree.root);
                tree.root = NULL;
            }
        }

        /**
         * Swaps the contents of this set with another set.
         * 
         * @param x Another set to swap with
         * 
         * @complexity O(1)
         */
        void swap(set &x)
        {
            ft::swap(tree.root, x.tree.root);
            ft::swap(tree._comp, x.tree._comp);
            ft::swap(tree._alloc, x.tree._alloc);
        }

        //
        // Modifiers Insert
        //
        /**
         * Inserts a value into the set.
         * 
         * @param val Value to be inserted
         * @return Pair consisting of an iterator to the inserted element (or to the element that prevented the insertion) and a bool denoting whether the insertion took place
         * 
         * @complexity O(log n)
         */
        ft::pair<iterator, bool> insert(const value_type &val)
        {
            iterator ite = find(val);
            if (ite != end())
                return ft::make_pair(ite, false);
            return ft::make_pair(iterator(tree.insert_node(val)), true);
        }

        /**
         * Inserts a value into the set at the given position.
         * 
         * @param position Hint for the position where the value should be inserted
         * @param val Value to be inserted
         * @return Iterator to the inserted element
         * 
         * @complexity O(log n)
         */
        iterator insert(iterator position, const value_type &val)
        {
            (void)position;
            return insert(val).first;
        }

        /**
         * Inserts a range of values into the set.
         * 
         * @param first Iterator to the first element in the range
         * @param last Iterator to the last element in the range
         * 
         * @complexity O(n log n)
         */
        template <class InputIterator>
        void insert(InputIterator first, InputIterator last)
        {
            for (InputIterator it = first; it != last; ++it)
            {
                iterator ite = find(*it);
                if (ite == end())
                    tree.insert_node(*it);
            }
        }

        //
        // Modifiers Erase


        //
        /**
         * Erases the element at the given position.
         * 
         * @param pos Iterator to the element to be erased
         * 
         * @complexity O(log n)
         */
        void erase(iterator pos)
        {
            if (pos != end())
                tree.delete_node(pos.base());
        }

        /**
         * Erases the element with the given key.
         * 
         * @param k Key of the element to be erased
         * @return Number of elements erased
         * 
         * @complexity O(log n)
         */
        size_type erase(const key_type &k)
        {
            iterator it = find(k);
            if (it != end())
            {
                erase(it++);
                return 1;
            }
            return 0;
        }

        /**
         * Erases elements in the range [first, last).
         * 
         * @param first Iterator to the first element in the range
         * @param last Iterator to the last element in the range
         * 
         * @complexity O(n log n)
         */
        void erase(iterator first, iterator last)
        {
            while (first != last)
                erase(first++);
        }

        //
        // Lookup
        //
        /**
         * Finds an element with the given value.
         * 
         * @param val Value to be found
         * @return Iterator to the found element, or end() if not found
         * 
         * @complexity O(log n)
         */
        iterator find(const value_type &val)
        {
            return iterator(find_node(val));
        }

        /**
         * Finds an element with the given value (const version).
         * 
         * @param val Value to be found
         * @return Const iterator to the found element, or end() if not found
         * 
         * @complexity O(log n)
         */
        const_iterator find(const value_type &val) const
        {
            return const_iterator(find_node(val));
        }

        /**
         * Counts the elements with a specific value.
         * 
         * @param val Value to count
         * @return Number of elements with the specified value
         * 
         * @complexity O(log n)
         */
        size_type count(const value_type &val) const
        {
            return find(val) != end();
        }

        /**
         * Returns a range containing all elements with the given key.
         * 
         * @param key Key to search for
         * @return Pair of iterators to the range of elements
         * 
         * @complexity O(log n)
         */
        ft::pair<iterator, iterator> equal_range(const value_type &key)
        {
            return ft::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
        }

        /**
         * Returns a range containing all elements with the given key (const version).
         * 
         * @param key Key to search for
         * @return Pair of const iterators to the range of elements
         * 
         * @complexity O(log n)
         */
        ft::pair<const_iterator, const_iterator> equal_range(const value_type &key) const
        {
            return ft::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
        }

        /**
         * Returns an iterator pointing to the first element that is not less than (i.e., greater or equal to) the given value.
         * 
         * @param val Value to compare
         * @return Iterator to the first element not less than the given value
         * 
         * @complexity O(log n)
         */
        iterator lower_bound(const value_type &val)
        {
            iterator it = begin();
            while (it != end())
            {
                if (!tree._comp(*it, val))
                    return it;
                it++;
            }
            return it;
        }

        /**
         * Returns a const iterator pointing to the first element that is not less than (i.e., greater or equal to) the given value.
         * 
         * @param key Key to compare
         * @return Const iterator to the first element not less than the given value
         * 
         * @complexity O(log n)
         */
        const_iterator lower_bound(const value_type &key) const
        {
            const_iterator it = begin();
            while (it != end())
            {
                if (!tree._comp(*it, key))
                    return it;
                it++;
            }
            return it;
        }

        /**
         * Returns an iterator pointing to the first element that is greater than the given value.
         * 
         * @param key Key to compare
         * @return Iterator to the first element greater than the given value
         * 
         * @complexity O(log n)
         */
        iterator upper_bound(const key_type &key)
        {
            iterator it = begin();
            while (it != end())
            {
                if (tree._comp(key, *it))
                    return it;
                it++;
            }
            return it;
        }

        /**
         * Returns a const iterator pointing to the first element that is greater than the given value.
         * 
         * @param key Key to compare
         * @return Const iterator to the first element greater than the given value
         * 
         * @complexity O(log n)
         */
        const_iterator upper_bound(const key_type &key) const
        {
            const_iterator it = begin();
            while (it != end())
            {
                if (tree._comp(key, *it))
                    return it;
                it++;
            }
            return it;
        }

        //
        // Observers
        //
        /**
         * Returns the function object that compares the keys.
         * 
         * @return Key comparison function object
         * 
         * @complexity O(1)
         */
        key_compare key_comp() const
        {
            return tree._comp;
        }

        /**
         * Returns the function object that compares the values.
         * 
         * @return Value comparison function object
         * 
         * @complexity O(1)
         */
        value_compare value_comp() const
        {
            return value_compare(tree._comp);
        }

    private:
        bt_node<value_type> *find_node(const key_type &key) const
        {
            return find_node(tree.root, key);
        }

        bt_node<value_type> *find_node(bt_node<value_type> *r, const key_type &key) const
        {
            if (!r || equal(r->data, key))
                return r;
            else if (tree._comp(key, r->data))
                return find_node(r->left, key);
            return find_node(r->right, key);
        }

        bool equal(const key_type &lhs, const key_type &rhs) const
        {
            return !tree._comp(lhs, rhs) && !tree._comp(rhs, lhs);
        }

        void delete_tree(bt_node<value_type> *ptr)
        {
            if (ptr->left)
                delete_tree(ptr->left);
            if (ptr->right)
                delete_tree(ptr->right);
            tree._alloc.destroy(ptr);
            tree._alloc.deallocate(ptr, 1);
        }
    };

    //
    // Non-member functions (operators)
    //
    /**
     * Checks if two sets are equal.
     * 
     * @param lhs Left-hand side set
     * @param rhs Right-hand side set
     * @return True if the sets are equal, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Compare, class Alloc>
    bool operator==(const ft::set<T, Compare, Alloc> &lhs, const ft::set<T, Compare, Alloc> &rhs)
    {
        if (lhs.size() == rhs.size())
        {
            typename ft::set<T, Compare, Alloc>::const_iterator ite_lhs = lhs.begin();
            typename ft::set<T, Compare, Alloc>::const_iterator ite_rhs = rhs.begin();
            while (ite_lhs != lhs.end())
            {
                if (*ite_lhs != *ite_rhs)
                    return false;
                ite_lhs++, ite_rhs++;
            }
            return true;
        }
        return false;
    }

    /**
     * Checks if two sets are not equal.
     * 
     * @param lhs Left-hand side set
     * @param rhs Right-hand side set
     * @return True if the sets are not equal, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Compare, class Alloc>
    bool operator!=(const ft::set<T, Compare, Alloc> &lhs, const ft::set<T, Compare, Alloc> &rhs)
    {
        return !(lhs == rhs);
    }

    /**
     * Checks if one set is less than another.
     * 
     * @param lhs Left-hand side set
     * @param rhs Right-hand side set
     * @return True if the lhs set is less than the rhs set, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Compare, class Alloc>
    bool operator<(const ft::set<T, Compare, Alloc> &lhs, const ft::set<T, Compare, Alloc> &rhs)
    {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    /**
     * Checks if one set is less than or equal to another.
     * 
     * @param lhs Left-hand side set
     * @param rhs Right-hand side set
     * @return True if the lhs set is less than or equal to

 the rhs set, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Compare, class Alloc>
    bool operator<=(const ft::set<T, Compare, Alloc> &lhs, const ft::set<T, Compare, Alloc> &rhs)
    {
        return !(ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()));
    }

    /**
     * Checks if one set is greater than another.
     * 
     * @param lhs Left-hand side set
     * @param rhs Right-hand side set
     * @return True if the lhs set is greater than the rhs set, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Compare, class Alloc>
    bool operator>(const ft::set<T, Compare, Alloc> &lhs, const ft::set<T, Compare, Alloc> &rhs)
    {
        return ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
    }

    /**
     * Checks if one set is greater than or equal to another.
     * 
     * @param lhs Left-hand side set
     * @param rhs Right-hand side set
     * @return True if the lhs set is greater than or equal to the rhs set, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Compare, class Alloc>
    bool operator>=(const ft::set<T, Compare, Alloc> &lhs, const ft::set<T, Compare, Alloc> &rhs)
    {
        return !(ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
    }

    /**
     * Swaps the contents of two sets.
     * 
     * @param lhs Left-hand side set
     * @param rhs Right-hand side set
     * 
     * @complexity O(1)
     */
    template <class T, class Compare, class Alloc>
    void swap(ft::set<T, Compare, Alloc> &lhs, ft::set<T, Compare, Alloc> &rhs)
    {
        lhs.swap(rhs);
    }

#pragma region multiset
    // multiset * ========================================================================================================

    template <class T, class Compare = ft::less<T>, class Alloc = std::allocator<T> >
    class multiset
    {
    public:
        //
        // Member types
        //
        typedef T key_type;
        typedef key_type value_type;
        typedef Compare key_compare;
        typedef Alloc allocator_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef ft::set_iterator<key_type, bt_node<key_type> > iterator;
        typedef ft::set_iterator<const key_type, const bt_node<key_type> > const_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef typename allocator_type::difference_type difference_type;
        typedef typename allocator_type::size_type size_type;

        //
        // Value comp (nested class)
        //

        class value_compare
        {
            friend class multiset;

        protected:
            key_compare comp;
            value_compare(key_compare c) : comp(c) {}

        public:
            typedef value_type first_argument_type;
            typedef value_type second_argument_type;
            bool operator()(const first_argument_type &x, const second_argument_type &y) const { return comp(x, y); }
        };

    private:
        RedBlackTree<value_type, Compare, allocator_type> tree;

    public:
        /**
         * Constructs an empty multiset with the given comparison object and allocator.
         * 
         * @param comp Comparison function object
         * @param alloc Allocator object
         * 
         * @complexity O(1)
         */
        explicit multiset(const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
            : tree(comp, alloc) {}

        /**
         * Constructs a multiset with the elements in the range [first, last).
         * 
         * @param first Iterator to the first element in the range
         * @param last Iterator to the last element in the range
         * @param comp Comparison function object
         * @param alloc Allocator object
         * 
         * @complexity O(n log n)
         */
        template <class InputIt>
        multiset(InputIt first, InputIt last, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
            : tree(comp, alloc)
        {
            insert(first, last);
        }

        /**
         * Copy constructor.
         * 
         * @param other Another multiset to be copied
         * 
         * @complexity O(n log n)
         */
        multiset(const multiset &other) : tree(other.tree._comp, other.tree._alloc)
        {
            clear();
            insert(other.begin(), other.end());
        }

        /**
         * Destructor. Clears the multiset.
         * 
         * @complexity O(n)
         */
        ~multiset() { clear(); }

        /**
         * Copy assignment operator.
         * 
         * @param other Another multiset to be assigned
         * @return Reference to the assigned multiset
         * 
         * @complexity O(n log n)
         */
        multiset &operator=(const multiset &other)
        {
            if (this != &other)
            {
                multiset tmp(other);
                this->swap(tmp);
            }
            return *this;
        }

        //
        // Iterators
        //
        /**
         * Returns an iterator to the first element.
         * 
         * @return Iterator to the first element
         * 
         * @complexity O(log n)
         */
        iterator begin()
        {
            return iterator(tree.left_most(tree.root));
        }

        /**
         * Returns a const iterator to the first element.
         * 
         * @return Const iterator to the first element
         * 
         * @complexity O(log n)
         */
        const_iterator begin() const
        {
            return const_iterator(tree.left_most(tree.root));
        }

        /**
         * Returns an iterator to the element following the last element.
         * 
         * @return Iterator to the element following the last element
         * 
         * @complexity O(log n)
         */
        iterator end()
        {
            return iterator(NULL, tree.right_most(tree.root));
        }

        /**
         * Returns a const iterator to the element following the last element.
         * 
         * @return Const iterator to the element following the last element
         * 
         * @complexity O(log n)
         */
        const_iterator end() const
        {
            return const_iterator(NULL, tree.right_most(tree.root));
        }

        //
        // Reverse Iterators
        //
        /**
         * Returns a reverse iterator to the first element of the reversed multiset.
         * 
         * @return Reverse iterator to the first element
         * 
         * @complexity O(1)
         */
        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }

        /**
         * Returns a const reverse iterator to the first element of the reversed multiset.
         * 
         * @return Const reverse iterator to the first element
         * 
         * @complexity O(1)
         */
        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }

        /**
         * Returns a reverse iterator to the element following the last element of the reversed multiset.
         * 
         * @return Reverse iterator to the element following the last element
         * 
         * @complexity O(1)
         */
        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }

        /**
         * Returns a const reverse iterator to the element following the last element of the reversed multiset.
         * 
         * @return Const reverse iterator to the element following the last element
         * 
         * @complexity O(1)
         */
        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

        //
        // Capacity
        //
        /**
         * Checks if the multiset is empty.
         * 
         * @return True if the multiset is empty, false otherwise
         * 
         * @complexity O(1)
         */
        bool empty() const
        {
            return begin() == end();
        }

        /**
         * Returns the number of elements in the multiset.
         * 
         * @return Number of elements in the multiset
         * 
         * @complexity O(n)
         */
        size_type size() const
        {
            size_type n = 0;
            for (const_iterator it = begin(); it != end(); ++it)
                n++;
            return n;
        }

        /**
         * Returns the maximum possible number of elements in the multiset.
         * 
         * @return Maximum possible number of elements
         * 
         * @complexity O(1)
         */
        size_type max_size() const
        {
            return tree._alloc.max_size();
        }

        //
        // Modifiers
        //
        /**
         * Clears the multiset, removing all elements.
         * 
         * @complexity O(n)
         */
        void clear()
        {
            if (tree.root)
            {
                delete_tree(tree.root);
                tree.root = NULL;
            }
        }

        /**
         * Swaps the contents of this multiset with

 another multiset.
         * 
         * @param x Another multiset to swap with
         * 
         * @complexity O(1)
         */
        void swap(multiset &x)
        {
            ft::swap(tree.root, x.tree.root);
            ft::swap(tree._comp, x.tree._comp);
            ft::swap(tree._alloc, x.tree._alloc);
        }

        //
        // Modifiers Insert
        //
        /**
         * Inserts a value into the multiset.
         * 
         * @param val Value to be inserted
         * @return Iterator to the inserted element
         * 
         * @complexity O(log n)
         */
        iterator insert(const value_type &val)
        {
            return iterator(tree.insert_node(val));
        }

        /**
         * Inserts a value into the multiset at the given position.
         * 
         * @param position Hint for the position where the value should be inserted
         * @param val Value to be inserted
         * @return Iterator to the inserted element
         * 
         * @complexity O(log n)
         */
        iterator insert(iterator position, const value_type &val)
        {
            (void)position;
            return insert(val);
        }

        /**
         * Inserts a range of values into the multiset.
         * 
         * @param first Iterator to the first element in the range
         * @param last Iterator to the last element in the range
         * 
         * @complexity O(n log n)
         */
        template <class InputIterator>
        void insert(InputIterator first, InputIterator last)
        {
            for (InputIterator it = first; it != last; ++it)
                tree.insert_node(*it);
        }

        //
        // Modifiers Erase
        //
        /**
         * Erases the element at the given position.
         * 
         * @param pos Iterator to the element to be erased
         * 
         * @complexity O(log n)
         */
        void erase(iterator pos)
        {
            if (pos != end())
                tree.delete_node(pos.base());
        }

        /**
         * Erases the element with the given key.
         * 
         * @param k Key of the element to be erased
         * @return Number of elements erased
         * 
         * @complexity O(log n)
         */
        size_type erase(const key_type &k)
        {
            iterator it = find(k);
            if (it != end())
            {
                erase(it++);
                return 1;
            }
            return 0;
        }

        /**
         * Erases elements in the range [first, last).
         * 
         * @param first Iterator to the first element in the range
         * @param last Iterator to the last element in the range
         * 
         * @complexity O(n log n)
         */
        void erase(iterator first, iterator last)
        {
            while (first != last)
                erase(first++);
        }

        //
        // Lookup
        //
        /**
         * Finds an element with the given value.
         * 
         * @param val Value to be found
         * @return Iterator to the found element, or end() if not found
         * 
         * @complexity O(log n)
         */
        iterator find(const value_type &val)
        {
            return iterator(find_node(val));
        }

        /**
         * Finds an element with the given value (const version).
         * 
         * @param val Value to be found
         * @return Const iterator to the found element, or end() if not found
         * 
         * @complexity O(log n)
         */
        const_iterator find(const value_type &val) const
        {
            return const_iterator(find_node(val));
        }

        /**
         * Counts the elements with a specific value.
         * 
         * @param val Value to count
         * @return Number of elements with the specified value
         * 
         * @complexity O(log n)
         */
        size_type count(const value_type &val) const
        {
            return find(val) != end();
        }

        /**
         * Returns a range containing all elements with the given key.
         * 
         * @param key Key to search for
         * @return Pair of iterators to the range of elements
         * 
         * @complexity O(log n)
         */
        ft::pair<iterator, iterator> equal_range(const value_type &key)
        {
            return ft::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
        }

        /**
         * Returns a range containing all elements with the given key (const version).
         * 
         * @param key Key to search for
         * @return Pair of const iterators to the range of elements
         * 
         * @complexity O(log n)
         */
        ft::pair<const_iterator, const_iterator> equal_range(const value_type &key) const
        {
            return ft::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
        }

        /**
         * Returns an iterator pointing to the first element that is not less than (i.e., greater or equal to) the given value.
         * 
         * @param val Value to compare
         * @return Iterator to the first element not less than the given value
         * 
         * @complexity O(log n)
         */
        iterator lower_bound(const value_type &val)
        {
            iterator it = begin();
            while (it != end())
            {
                if (!tree._comp(*it, val))
                    return it;
                it++;
            }
            return it;
        }

        /**
         * Returns a const iterator pointing to the first element that is not less than (i.e., greater or equal to) the given value.
         * 
         * @param key Key to compare
         * @return Const iterator to the first element not less than the given value
         * 
         * @complexity O(log n)
         */
        const_iterator lower_bound(const value_type &key) const
        {
            const_iterator it = begin();
            while (it != end())
            {
                if (!tree._comp(*it, key))
                    return it;
                it++;
            }
            return it;
        }

        /**
         * Returns an iterator pointing to the first element that is greater than the given value.
         * 
         * @param key Key to compare
         * @return Iterator to the first element greater than the given value
         * 
         * @complexity O(log n)
         */
        iterator upper_bound(const key_type &key)
        {
            iterator it = begin();
            while (it != end())
            {
                if (tree._comp(key, *it))
                    return it;
                it++;
            }
            return it;
        }

        /**
         * Returns a const iterator pointing to the first element that is greater than the given value.
         * 
         * @param key Key to compare
         * @return Const iterator to the first element greater than the given value
         * 
         * @complexity O(log n)
         */
        const_iterator upper_bound(const key_type &key) const
        {
            const_iterator it = begin();
            while (it != end())
            {
                if (tree._comp(key, *it))
                    return it;
                it++;
            }
            return it;
        }

        //
        // Observers
        //
        /**
         * Returns the function object that compares the keys.
         * 
         * @return Key comparison function object
         * 
         * @complexity O(1)
         */
        key_compare key_comp() const
        {
            return tree._comp;
        }

        /**
         * Returns the function object that compares the values.
         * 
         * @return Value comparison function object
         * 
         * @complexity O(1)
         */
        value_compare value_comp() const
        {
            return value_compare(tree._comp);
        }

    private:
        bt_node<value_type> *find_node(const key_type &key) const
        {
            return find_node(tree.root, key);
        }

        bt_node<value_type> *find_node(bt_node<value_type> *r, const key_type &key) const
        {
            if (!r || equal(r->data, key))
                return r;
            else if (tree._comp(key, r->data))
                return find_node(r->left, key);
            return find_node(r->right, key);
        }

        bool equal(const key_type &lhs, const key_type &rhs) const
        {
            return !tree._comp(lhs, rhs) && !tree._comp(rhs, lhs);
        }

        void delete_tree(bt_node<value_type> *ptr)
        {
            if (ptr->left)
                delete_tree(ptr->left);
            if (ptr->right)
                delete_tree(ptr->right);
            tree._alloc.destroy(ptr);
            tree._alloc.deallocate(ptr, 1);
        }
    };

    //
    // Non-member functions (operators)
    //
    /**
     * Checks if two multisets are equal.
     * 
     * @param lhs Left-hand side multiset
     * @param rhs Right-hand side multiset
     * @return True if the multisets are equal, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Compare, class Alloc>
    bool operator==(const ft::multiset<T, Compare, Alloc> &lhs, const ft::multiset<T, Compare, Alloc> &rhs)
    {
        if (lhs.size() == rhs.size())
        {
            typename ft::multiset<T, Compare, Alloc>::const_iterator ite_lhs = lhs.begin();
            typename ft::multiset<T, Compare, Alloc>::const_iterator ite_rhs = rhs.begin();
            while (ite_lhs != lhs.end())
            {
                if (*ite_lhs != *ite_rhs)
                    return false;
                ite_lhs++, ite_rhs++;
            }
            return true;
        }
        return false;
    }

    /**
     * Checks if two multisets are not equal.
     * 
     * @param lhs Left-hand side multiset
     * @param rhs Right-hand side multiset
     * @return True if the multisets are not equal, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Compare, class Alloc>
    bool operator!=(const ft::multiset<T, Compare, Alloc> &lhs, const ft::multiset<T, Compare, Alloc> &rhs)
    {
        return !(lhs == rhs);
    }

    /**
     * Checks if one multiset is less than another.
     * 
     * @param lhs Left-hand side multiset
     * @param rhs Right-hand side multiset
     * @return True if the lhs multiset is less than the rhs multiset, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Compare, class Alloc>
    bool operator<(const ft::multiset<T, Compare, Alloc> &lhs, const ft::multiset<T, Compare, Alloc> &rhs)
    {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    /**
     * Checks if one multiset is less than or equal to another.
     * 
     * @param lhs Left-hand side multiset
     * @param rhs Right-hand side multiset
     * @return True if the lhs multiset is less than or equal to the rhs multiset, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Compare, class Alloc>
    bool operator<=(const ft::multiset<T, Compare, Alloc> &lhs, const ft::multiset<T, Compare, Alloc> &rhs)
    {
        return !(ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()));
    }

    /**
     * Checks if one multiset is greater than another.
     * 
     * @param lhs Left-hand side multiset
     * @param rhs Right-hand side multiset
     * @return True if the lhs multiset is greater than the rhs multiset, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Compare, class Alloc>
    bool operator>(const ft::multiset<T, Compare, Alloc> &lhs, const ft::multiset<T, Compare, Alloc> &rhs)
    {
        return ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
    }

    /**
     * Checks if one multiset is greater than or equal to another.
     * 
     * @param lhs Left-hand side multiset
     * @param rhs Right-hand side multiset
     * @return True if the lhs multiset is greater than or equal to the rhs multiset, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Compare, class Alloc>
    bool operator>=(const ft::multiset<T, Compare, Alloc> &lhs, const ft::multiset<T, Compare, Alloc> &rhs)
    {
        return !(ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
    }

    /**
     * Swaps the contents of two multisets.
     * 
     * @param lhs Left-hand side multiset
     * @param rhs Right-hand side multiset
     * 
     * @complexity O(1)
     */
    template <class T, class Compare, class Alloc>
    void swap(ft::multiset<T, Compare, Alloc> &lhs, ft::multiset<T, Compare, Alloc> &rhs)
    {
        lhs.swap(rhs);
    }

} // namespace ft

#endif