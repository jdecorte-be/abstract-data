#ifndef SET_HPP
#define SET_HPP

#include "utility.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"
#include "exception.hpp"
#include "type_traits.hpp"
#include "string.hpp"
#include "tree.hpp"
#include <memory>
#include <functional>

namespace ft
{

    template <class T, class Compare = std::less<T>, class Alloc = std::allocator<T> >
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
        typedef ft::set_iterator<const key_type, bt_node<key_type> > iterator;
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
        explicit set(const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
            : tree(comp, alloc) {}

        template <class InputIt>
        set(InputIt first, InputIt last, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
            : tree(comp, alloc)
        {
            insert(first, last);
        }

        set(const set &other) : tree(other.tree._comp, other.tree._alloc)
        {
            clear();
            insert(other.begin(), other.end());
        }

        ~set() { clear(); }

        set &operator=(const set &other)
        {
            if (this == &other)
                return *this;
            clear();
            insert(other.begin(), other.end());
            tree._comp = other.tree._comp;
            tree._alloc = other.tree._alloc;
            return *this;
        }

        //
        // Iterators
        //
        iterator begin()
        {
            return iterator(tree.left_most(tree.root));
        }

        const_iterator begin() const
        {
            return const_iterator(tree.left_most(tree.root));
        }

        iterator end()
        {
            return iterator(NULL, tree.right_most(tree.root));
        }

        const_iterator end() const
        {
            return const_iterator(NULL, tree.right_most(tree.root));
        }

        //
        // Reverse Iterators
        //
        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

        //
        // Capacity
        //
        bool empty() const
        {
            return begin() == end();
        }

        size_type size() const
        {
            size_type n = 0;
            for (const_iterator it = begin(); it != end(); ++it)
                n++;
            return n;
        }

        size_type max_size() const
        {
            return tree._alloc.max_size();
        }

        //
        // Modifiers
        //
        void clear() 
        {
            if (tree.root)
            {
                delete_tree(tree.root);
                tree.root = NULL;
            }
        }

        void swap(set &x) 
        {
            ft::swap(tree.root, x.tree.root);
            ft::swap(tree._comp, x.tree._comp);
            ft::swap(tree._alloc, x.tree._alloc);
        }

        //
        // Modifiers Insert
        //
        ft::pair<iterator, bool> insert(const value_type &val)
        {
            iterator ite = find(val);
            if (ite != end())
                return ft::make_pair(ite, false);
            return ft::make_pair(iterator(tree.insert_node(val)), true);
        }

        iterator insert(iterator position, const value_type &val)
        {
            (void)position;
            return insert(val).first;
        }

        template <class InputIterator>
        void insert(InputIterator first, InputIterator last) 
        {
            while (first != last)
                insert(*first++);
        }

        //
        // Modifiers Erase
        //
        void erase(iterator pos) 
        {
            if (pos != end())
                tree.delete_node(pos.base());
        }

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

        void erase(iterator first, iterator last) 
        {
            while (first != last)
                erase(first++);
        }

        //
        // Lookup
        //
        iterator find(const value_type &val) 
        {
            return iterator(find_node(val));
        }

        const_iterator find(const value_type &val) const 
        {
            return const_iterator(find_node(val));
        }

        size_type count(const value_type &val) const 
        {
            return find(val) != end();
        }

        ft::pair<iterator, iterator> equal_range(const value_type &key)
        {
            return ft::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
        }

        ft::pair<const_iterator, const_iterator> equal_range(const value_type &key) const
        {
            return ft::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
        }

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
        key_compare key_comp() const
        {
            return tree._comp;
        }

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

    template <class T, class Compare, class Alloc>
    bool operator!=(const ft::set<T, Compare, Alloc> &lhs, const ft::set<T, Compare, Alloc> &rhs)
    {
        return !(lhs == rhs);
    }


    template <class T, class Compare, class Alloc>
    bool operator<(const ft::set<T, Compare, Alloc> &lhs, const ft::set<T, Compare, Alloc> &rhs)
    {
        return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class T, class Compare, class Alloc>
    bool operator<=(const ft::set<T, Compare, Alloc> &lhs, const ft::set<T, Compare, Alloc> &rhs)
    {
        return !(lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()));
    }

    template <class T, class Compare, class Alloc>
    bool operator>(const ft::set<T, Compare, Alloc> &lhs, const ft::set<T, Compare, Alloc> &rhs)
    {
        return lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
    }

    template <class T, class Compare, class Alloc>
    bool operator>=(const ft::set<T, Compare, Alloc> &lhs, const ft::set<T, Compare, Alloc> &rhs)
    {
        return !(lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
    }

    template <class T, class Compare, class Alloc>
    void swap(ft::set<T, Compare, Alloc> &lhs, ft::set<T, Compare, Alloc> &rhs)
    {
        lhs.swap(rhs);
    }

} // namespace ft

#endif
