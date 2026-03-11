#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <iterator>

#include "functional.hpp"
#include "type_traits.hpp"

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
        typedef ft::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef ft::random_access_iterator_tag iterator_category;
    };
    template <class T>
    struct iterator_traits<T *const>
    {
    public:
        typedef ft::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T *pointer;
        typedef const T &reference;
        typedef ft::random_access_iterator_tag iterator_category;
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
        /**
         * Default constructor
         *
         * @complexity O(1)
         */
        reverse_iterator() : iter() {}

        /**
         * Initializes reverse_iterator with an iterator
         *
         * @param it An iterator to initialize with
         *
         * @complexity O(1)
         */
        explicit reverse_iterator(iterator_type it) : iter(it) {}

        /**
         * Copy constructor
         *
         * @param rev_it A reverse_iterator to copy from
         *
         * @complexity O(1)
         */
        template <class Iter>
        reverse_iterator(const reverse_iterator<Iter> &rev_it) : iter(rev_it.base()) {}

        /**
         * Returns the base iterator
         *
         * @return The base iterator
         *
         * @complexity O(1)
         */
        iterator_type base() const { return iter; }

        /**
         * Dereferences the iterator
         *
         * @return A reference to the element pointed
         *
         * @complexity O(1)
         */
        reference operator*() const
        {
            Iterator tmp = iter;
            return *--tmp;
        }

        /**
         * Returns a reverse iterator pointing to the element located n positions away
         *
         * @param n Number of positions to advance
         * @return A reverse iterator pointing to the element located n positions away
         *
         * @complexity O(1)
         */
        reverse_iterator operator+(difference_type n) const { return reverse_iterator(iter - n); }

        /**
         * Decrements the base iterator
         *
         * @return A reference to the current iterator
         *
         * @complexity O(1)
         */
        reverse_iterator &operator++()
        {
            --iter;
            return *this;
        }

        /**
         * Decrements the base iterator (postfix)
         *
         * @return A reverse iterator before decrement
         *
         * @complexity O(1)
         */
        reverse_iterator operator++(int)
        {
            reverse_iterator tmp(*this);
            --iter;
            return tmp;
        }

        /**
         * Decreases the base iterator by n
         *
         * @param n Number of positions to decrement
         * @return A reference to the current iterator
         *
         * @complexity O(1)
         */
        reverse_iterator &operator+=(difference_type n)
        {
            iter -= n;
            return *this;
        }

        /**
         * Returns a reverse iterator pointing to the element located n positions before
         *
         * @param n Number of positions to advance backwards
         * @return A reverse iterator pointing to the element located n positions before
         *
         * @complexity O(1)
         */
        reverse_iterator operator-(difference_type n) const { return reverse_iterator(iter + n); }

        /**
         * Returns the distance between two reverse iterators
         *
         * @param x A reverse iterator to subtract
         * @return The distance between the two reverse iterators
         *
         * @complexity O(1)
         */
        ft::ptrdiff_t operator-(const reverse_iterator &x) const { return (x.base() - iter); }

        /**
         * Increments the base iterator
         *
         * @return A reference to the current iterator
         *
         * @complexity O(1)
         */
        reverse_iterator &operator--()
        {
            ++iter;
            return *this;
        }

        /**
         * Increments the base iterator (postfix)
         *
         * @return A reverse iterator before increment
         *
         * @complexity O(1)
         */
        reverse_iterator operator--(int)
        {
            reverse_iterator tmp(*this);
            ++iter;
            return tmp;
        }

        /**
         * Increases the base iterator by n
         *
         * @param n Number of positions to increment
         * @return A reference to the current iterator
         *
         * @complexity O(1)
         */
        reverse_iterator &operator-=(difference_type n)
        {
            iter += n;
            return *this;
        }

        /**
         * Dereferences the iterator
         *
         * @return A pointer to the element pointed
         *
         * @complexity O(1)
         */
        pointer operator->() const { return &(operator*()); }

        /**
         * Accesses the element located n positions away
         *
         * @param n Number of positions to advance
         * @return A reference to the element located n positions away
         *
         * @complexity O(1)
         */
        reference operator[](difference_type n) const { return *(*this + n); }
    };

    /**
     * Returns a reverse iterator pointing to the element located n positions away
     *
     * @param n Number of positions to advance
     * @param rev_it A reverse iterator
     * @return A reverse iterator pointing to the element located n positions away
     *
     * @complexity O(1)
     */
    template <typename It>
    reverse_iterator<It> operator+(typename reverse_iterator<It>::difference_type n, reverse_iterator<It> const &rev_it)
    {
        return (reverse_iterator<It>(rev_it.base() - n));
    }

    /**
     * Compares two reverse iterators for equality
     *
     * @param lhs Left-hand side reverse iterator
     * @param rhs Right-hand side reverse iterator
     * @return True if equal, otherwise false
     *
     * @complexity O(1)
     */
    template <typename It1, typename It2>
    typename reverse_iterator<It1>::difference_type operator==(const reverse_iterator<It1> &lhs, const reverse_iterator<It2> &rhs) { return lhs.base() == rhs.base(); }

    /**
     * Compares two reverse iterators for inequality
     *
     * @param lhs Left-hand side reverse iterator
     * @param rhs Right-hand side reverse iterator
     * @return True if not equal, otherwise false
     *
     * @complexity O(1)
     */
    template <typename It1, typename It2>
    bool operator!=(const reverse_iterator<It1> &lhs, const reverse_iterator<It2> &rhs) { return !(lhs.base() == rhs.base()); }

    /**
     * Compares two reverse iterators for less than
     *
     * @param lhs Left-hand side reverse iterator
     * @param rhs Right-hand side reverse iterator
     * @return True if lhs is less than rhs, otherwise false
     *
     * @complexity O(1)
     */
    template <typename It1, typename It2>
    bool operator<(const reverse_iterator<It1> &lhs, const reverse_iterator<It2> &rhs) { return lhs.base() > rhs.base(); }

    /**
     * Compares two reverse iterators for less than or equal to
     *
     * @param lhs Left-hand side reverse iterator
     * @param rhs Right-hand side reverse iterator
     * @return True if lhs is less than or equal to rhs, otherwise false
     *
     * @complexity O(1)
     */
    template <typename It1, typename It2>
    bool operator<=(const reverse_iterator<It1> &lhs, const reverse_iterator<It2> &rhs) { return lhs.base() >= rhs.base(); }

    /**
     * Compares two reverse iterators for greater than
     *
     * @param lhs Left-hand side reverse iterator
     * @param rhs Right-hand side reverse iterator
     * @return True if lhs is greater than rhs, otherwise false
     *
     * @complexity O(1)
     */
    template <typename It1, typename It2>
    bool operator>(const reverse_iterator<It1> &lhs, const reverse_iterator<It2> &rhs) { return lhs.base() < rhs.base(); }

    /**
     * Compares two reverse iterators for greater than or equal to
     *
     * @param lhs Left-hand side reverse iterator
     * @param rhs Right-hand side reverse iterator
     * @return True if lhs is greater than or equal to rhs, otherwise false
     *
     * @complexity O(1)
     */
    template <typename It1, typename It2>
    bool operator>=(const reverse_iterator<It1> &lhs, const reverse_iterator<It2> &rhs) { return lhs.base() <= rhs.base(); }

    // * Map iterator =================================================================================================

    template <class Key, class T, class Node>
    class map_iterator
    {
    public:
        typedef ft::pair<const Key, T> pair_type;
        typedef typename ft::conditional<ft::is_const<Node>::value, const pair_type, pair_type>::type value_type;
        typedef ft::bidirectional_iterator_tag iterator_category;
        typedef ft::ptrdiff_t difference_type;
        typedef ft::size_t size_type;
        typedef value_type *pointer;
        typedef value_type &reference;

    public:
        Node *ptr;
        Node *_end;

    public:
        /**
         * Default constructor
         *
         * @complexity O(1)
         */
        map_iterator() : ptr(NULL), _end(NULL) {}

        /**
         * Initializes map_iterator with a node pointer
         *
         * @param ptr A node pointer
         *
         * @complexity O(1)
         */
        map_iterator(Node *ptr) : ptr(ptr), _end(NULL) {}

        /**
         * Initializes map_iterator with a node pointer and an end pointer
         *
         * @param ptr A node pointer
         * @param end An end pointer
         *
         * @complexity O(1)
         */
        map_iterator(Node *ptr, Node *end) : ptr(ptr), _end(end) {}

        /**
         * Copy constructor
         *
         * @param src A map_iterator to copy from
         *
         * @complexity O(1)
         */
        map_iterator(const map_iterator &src) : ptr(src.ptr), _end(src._end) {}

        /**
         * Assignment operator
         *
         * @param src A map_iterator to assign from
         * @return A reference to the assigned map_iterator
         *
         * @complexity O(1)
         */
        map_iterator &operator=(const map_iterator &src)
        {
            ptr = src.ptr;
            _end = src._end;
            return *this;
        }

        /**
         * Const casting constructor
         *
         * @param src A map_iterator to cast from
         *
         * @complexity O(1)
         */
        template <class OtherNode>
        map_iterator(const map_iterator<Key, T, OtherNode> &src) : ptr(src.ptr), _end(src._end) {}

        /**
         * Returns the base node pointer
         *
         * @return The base node pointer
         *
         * @complexity O(1)
         */
        Node *base() const
        {
            return ptr;
        }

        /**
         * Increments the iterator
         *
         * @return A reference to the current iterator
         *
         * @complexity O(1)
         */
        map_iterator &operator++()
        {
            if (this->ptr == NULL)
                return *this;
            else if (!this->ptr->parent && !this->ptr->right)
            {
                this->_end = this->ptr;
                this->ptr = NULL;
            }
            else if (this->ptr->right)
                this->ptr = down_bigger_node();
            else
                this->ptr = up_bigger_node(this->ptr);
            return *this;
        }

        /**
         * Increments the iterator (postfix)
         *
         * @return A map_iterator before increment
         *
         * @complexity O(1)
         */
        map_iterator operator++(int)
        {
            map_iterator<Key, T, Node> tmp = *this;
            this->operator++();
            return tmp;
        }

        /**
         * Decrements the iterator
         *
         * @return A reference to the current iterator
         *
         * @complexity O(1)
         */
        map_iterator &operator--()
        {
            if (this->ptr == NULL)
            {
                if (_end)
                    ptr = _end;
            }
            else if (!ptr->parent && !ptr->left)
                ptr = NULL;
            else if (ptr->left)
                ptr = down_smallest_node();
            else
                ptr = up_smallest_node(ptr);
            return *this;
        }

        /**
         * Decrements the iterator (postfix)
         *
         * @return A map_iterator before decrement
         *
         * @complexity O(1)
         */
        map_iterator operator--(int)
        {
            map_iterator<Key, T, Node> tmp = *this;
            this->operator--();
            return tmp;
        }

        /**
         * Dereferences the iterator
         *
         * @return A reference to the element pointed
         *
         * @complexity O(1)
         */
        value_type &operator*() const
        {
            return ptr->data;
        }

        /**
         * Dereferences the iterator
         *
         * @return A pointer to the element pointed
         *
         * @complexity O(1)
         */
        value_type *operator->() const
        {
            return &(ptr->data);
        }

        /**
         * Compares two map_iterators for equality
         *
         * @param tocomp A map_iterator to compare with
         * @return True if equal, otherwise false
         *
         * @complexity O(1)
         */
        template <class OtherNode>
        bool operator==(const map_iterator<Key, T, OtherNode> &tocomp) const
        {
            return ptr == tocomp.base();
        }

        /**
         * Compares two map_iterators for inequality
         *
         * @param tocomp A map_iterator to compare with
         * @return True if not equal, otherwise false
         *
         * @complexity O(1)
         */
        template <class OtherNode>
        bool operator!=(const map_iterator<Key, T, OtherNode> &tocomp) const
        {
            return ptr != tocomp.base();
        }

    private: // Private function like Next and Prev Node // Post order (root is end)
        /**
         * Returns the next larger node
         *
         * @param node A node pointer
         * @return The next larger node
         *
         * @complexity O(log n)
         */
        Node *up_bigger_node(Node *node)
        {
            Node *next;

            if (!node->right)
            {
                next = node;
                if (next->parent->right != next && next != next->parent->left)
                    return (NULL);
                while (next->parent != NULL && next == next->parent->right)
                    next = next->parent;
                next = next->parent;
            }
            else
            {
                next = node->right;
                while (next->left)
                    next = next->left;
            }
            return (next);
        }

        /**
         * Returns the next larger node in the down direction
         *
         * @return The next larger node
         *
         * @complexity O(log n)
         */
        Node *down_bigger_node()
        {
            Node *tmp = this->ptr->right;

            while (tmp->left)
                tmp = tmp->left;
            return tmp;
        }

        /**
         * Returns the next smaller node in the down direction
         *
         * @return The next smaller node
         *
         * @complexity O(log n)
         */
        Node *down_smallest_node()
        {
            Node *tmp = this->ptr->left;

            while (tmp->right)
                tmp = tmp->right;
            return tmp;
        }

        /**
         * Returns the next smaller node
         *
         * @param node A node pointer
         * @return The next smaller node
         *
         * @complexity O(log n)
         */
        Node *up_smallest_node(Node *node)
        {
            Node *prev;

            if (node->left == NULL || node->left == NULL)
            {
                prev = node;
                while (prev->parent != NULL && prev == prev->parent->left)
                    prev = prev->parent;
                prev = prev->parent;
            }
            else
            {
                prev = node->left;
                while (prev->right != NULL)
                    prev = prev->right;
            }
            return (prev);
        }
    };

    // * Set iterator =================================================================================================

    template <class T, class Node>
    class set_iterator
    {
    public:
        typedef T value_type;
        typedef ft::bidirectional_iterator_tag iterator_category;
        typedef ft::ptrdiff_t difference_type;
        typedef ft::size_t size_type;

        typedef const T &reference;
        typedef const T *pointer;

    private:
        Node *ptr;
        Node *_end;

    public:
        /**
         * Default constructor
         *
         * @complexity O(1)
         */
        set_iterator() : ptr(NULL), _end(NULL) {}

        /**
         * Initializes set_iterator with a node pointer and an optional end pointer
         *
         * @param ptr A node pointer
         * @param end An optional end pointer
         *
         * @complexity O(1)
         */
        set_iterator(Node *ptr, Node *end = NULL) : ptr(ptr), _end(end) {}

        /**
         * Copy constructor
         *
         * @param other A set_iterator to copy from
         *
         * @complexity O(1)
         */
        template <class U>
        set_iterator(const set_iterator<T, U> &other,
                     typename ft::enable_if<!ft::is_const<U>::value, U>::type * = 0)
            : ptr(other.base()), _end(other.end_base()) {}

        /**
         * Const casting constructor
         *
         * @param other A set_iterator to cast from
         *
         * @complexity O(1)
         */
        template <class U>
        set_iterator(const set_iterator<const T, U> &other,
                     typename ft::enable_if<ft::is_const<U>::value, U>::type * = 0)
            : ptr(const_cast<Node *>(other.base())), _end(const_cast<Node *>(other.end_base())) {}

        /**
         * Assignment operator
         *
         * @param other A set_iterator to assign from
         * @return A reference to the assigned set_iterator
         *
         * @complexity O(1)
         */
        set_iterator &operator=(const set_iterator &other)
        {
            if (this != &other)
            {
                this->ptr = other.ptr;
                this->_end = other._end;
            }
            return *this;
        }

        /**
         * Returns the base node pointer
         *
         * @return The base node pointer
         *
         * @complexity O(1)
         */
        Node *base() const
        {
            return ptr;
        }

        /**
         * Returns the end node pointer
         *
         * @return The end node pointer
         *
         * @complexity O(1)
         */
        Node *end_base() const
        {
            return _end;
        }

        /**
         * Increments the iterator
         *
         * @return A reference to the current iterator
         *
         * @complexity O(1)
         */
        set_iterator &operator++()
        {
            if (this->ptr == NULL)
                return *this;
            else if (!this->ptr->parent && !this->ptr->right)
            {
                this->_end = this->ptr;
                this->ptr = NULL;
            }
            else if (this->ptr->right)
                this->ptr = down_bigger_node();
            else
                this->ptr = up_bigger_node(this->ptr);
            return *this;
        }

        /**
         * Increments the iterator (postfix)
         *
         * @return A set_iterator before increment
         *
         * @complexity O(1)
         */
        set_iterator operator++(int)
        {
            set_iterator<T, Node> tmp = *this;
            this->operator++();
            return tmp;
        }

        /**
         * Decrements the iterator
         *
         * @return A reference to the current iterator
         *
         * @complexity O(1)
         */
        set_iterator &operator--()
        {
            if (this->ptr == NULL)
            {
                if (_end)
                    ptr = _end;
            }
            else if (!ptr->parent && !ptr->left)
                ptr = NULL;
            else if (ptr->left)
                ptr = down_smallest_node();
            else
                ptr = up_smallest_node(ptr);
            return *this;
        }

        /**
         * Decrements the iterator (postfix)
         *
         * @return A set_iterator before decrement
         *
         * @complexity O(1)
         */
        set_iterator operator--(int)
        {
            set_iterator<T, Node> tmp = *this;
            this->operator--();
            return tmp;
        }

        /**
         * Dereferences the iterator
         *
         * @return A reference to the element pointed
         *
         * @complexity O(1)
         */
        reference operator*() const
        {
            return ptr->data;
        }

        /**
         * Dereferences the iterator
         *
         * @return A pointer to the element pointed
         *
         * @complexity O(1)
         */
        pointer operator->() const
        {
            return &(operator*());
        }

        /**
         * Compares two set_iterators for equality
         *
         * @param tocomp A set_iterator to compare with
         * @return True if equal, otherwise false
         *
         * @complexity O(1)
         */
        template <class OtherNode>
        bool operator==(const set_iterator<T, OtherNode> &tocomp) const
        {
            return ptr == tocomp.base();
        }

        /**
         * Compares two set_iterators for inequality
         *
         * @param tocomp A set_iterator to compare with
         * @return True if not equal, otherwise false
         *
         * @complexity O(1)
         */
        template <class OtherNode>
        bool operator!=(const set_iterator<T, OtherNode> &tocomp) const
        {
            return ptr != tocomp.base();
        }

        /**
         * Const casting operator
         *
         * @return A const set_iterator
         *
         * @complexity O(1)
         */
        operator set_iterator<const T, const Node>() const
        {
            return set_iterator<const T, const Node>(ptr, _end);
        }

    private: // Private function like Next and Prev Node // Post order (root is end)
        /**
         * Returns the next larger node
         *
         * @param node A node pointer
         * @return The next larger node
         *
         * @complexity O(log n)
         */
        Node *up_bigger_node(Node *node)
        {
            Node *next;

            if (!node->right)
            {
                next = node;
                if (next->parent->right != next && next != next->parent->left)
                    return (NULL);
                while (next->parent != NULL && next == next->parent->right)
                    next = next->parent;
                next = next->parent;
            }
            else
            {
                next = node->right;
                while (next->left)
                    next = next->left;
            }
            return (next);
        }

        /**
         * Returns the next larger node in the down direction
         *
         * @return The next larger node
         *
         * @complexity O(log n)
         */
        Node *down_bigger_node()
        {
            Node *tmp = this->ptr->right;

            while (tmp->left)
                tmp = tmp->left;
            return tmp;
        }

        /**
         * Returns the next smaller node in the down direction
         *
         * @return The next smaller node
         *
         * @complexity O(log n)
         */
        Node *down_smallest_node()
        {
            Node *tmp = this->ptr->left;

            while (tmp->right)
                tmp = tmp->right;
            return tmp;
        }

        /**
         * Returns the next smaller node
         *
         * @param node A node pointer
         * @return The next smaller node
         *
         * @complexity O(log n)
         */
        Node *up_smallest_node(Node *node)
        {
            Node *prev;

            if (node->left == NULL || node->left == NULL)
            {
                prev = node;
                while (prev->parent != NULL && prev == prev->parent->left)
                    prev = prev->parent;
                prev = prev->parent;
            }
            else
            {
                prev = node->left;
                while (prev->right != NULL)
                    prev = prev->right;
            }
            return (prev);
        }
    };

    /**
     * Compares two set_iterators for equality
     *
     * @param lhs Left-hand side set_iterator
     * @param rhs Right-hand side set_iterator
     * @return True if equal, otherwise false
     *
     * @complexity O(1)
     */
    template <class T1, class Node1, class T2, class Node2>
    bool operator==(const set_iterator<T1, Node1> &lhs, const set_iterator<T2, Node2> &rhs)
    {
        return lhs.base() == rhs.base();
    }

    /**
     * Compares two set_iterators for inequality
     *
     * @param lhs Left-hand side set_iterator
     * @param rhs Right-hand side set_iterator
     * @return True if not equal, otherwise false
     *
     * @complexity O(1)
     */
    template <class T1, class Node1, class T2, class Node2>
    bool operator!=(const set_iterator<T1, Node1> &lhs, const set_iterator<T2, Node2> &rhs)
    {
        return lhs.base() != rhs.base();
    }

    /**
     * Computes the distance between two input iterators
     *
     * @param first Iterator to the beginning of the range
     * @param last Iterator to the end of the range
     * @return The distance between the two iterators
     *
     * @complexity O(n)
     */
    template <class InputIterator>
    size_t distance(InputIterator first, InputIterator last)
    {
        size_t n = 0;
        while (first != last)
        {
            ++first;
            ++n;
        }
        return n;
    }
}

#endif