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

    // * Map iterator =================================================================================================

    template <class Key, class T, class Node>
    class map_iterator
    {
    public:
        typedef ft::pair<const Key, T> pair_type;
        typedef typename ft::conditional<ft::is_const<Node>::value, const pair_type, pair_type>::type value_type;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef ft::ptrdiff_t difference_type;
        typedef ft::size_t size_type;
        typedef value_type *pointer;
        typedef value_type &reference;

    public:
        Node *ptr;
        Node *_end;

    public:
        //
        // Constructor
        //
        map_iterator() : ptr(nullptr), _end(NULL) {}

        map_iterator(Node *ptr) : ptr(ptr), _end(NULL) {}

        map_iterator(Node *ptr, Node *end) : ptr(ptr), _end(end) {}

        map_iterator(const map_iterator &src) : ptr(src.ptr), _end(src._end) {}

        map_iterator &operator=(const map_iterator &src)
        {
            ptr = src.ptr;
            _end = src._end;
            return *this;
        }

        //
        // Const casting
        //
        template <class OtherNode>
        map_iterator(const map_iterator<Key, T, OtherNode> &src) : ptr(src.ptr), _end(src._end) {}

        //
        // Operators
        //
        Node *base() const
        {
            return ptr;
        }

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

        map_iterator operator++(int)
        {
            map_iterator<Key, T, Node> tmp = *this;
            this->operator++();
            return tmp;
        }

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

        map_iterator operator--(int)
        {
            map_iterator<Key, T, Node> tmp = *this;
            this->operator--();
            return tmp;
        }

        value_type &operator*() const
        {
            return ptr->data;
        }

        value_type *operator->() const
        {
            return &(ptr->data);
        }

        template <class OtherNode>
        bool operator==(const map_iterator<Key, T, OtherNode> &tocomp) const
        {
            return ptr == tocomp.base();
        }

        template <class OtherNode>
        bool operator!=(const map_iterator<Key, T, OtherNode> &tocomp) const
        {
            return ptr != tocomp.base();
        }


    private: // Private function like Next and Prev Node // Post order (root is end)
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

        Node *down_bigger_node()
        {
            Node *tmp = this->ptr->right;

            while (tmp->left)
                tmp = tmp->left;
            return tmp;
        }

        // Return next smalles
        Node *down_smallest_node()
        {
            Node *tmp = this->ptr->left;

            while (tmp->right)
                tmp = tmp->right;
            return tmp;
        }

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
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef ft::ptrdiff_t difference_type;
        typedef ft::size_t size_type;

        typedef const T &reference;
        typedef const T *pointer;

    private:
        Node *ptr;
        Node *_end;

    public:
        //
        // Constructor
        //
        set_iterator() : ptr(nullptr), _end(nullptr) {}

        set_iterator(Node *ptr, Node *end = nullptr) : ptr(ptr), _end(end) {}

        template <class U>
        set_iterator(const set_iterator<T, U> &other,
                     typename ft::enable_if<!ft::is_const<U>::value, U>::type * = 0)
            : ptr(other.base()), _end(other.end_base()) {}

        template <class U>
        set_iterator(const set_iterator<const T, U> &other,
                     typename ft::enable_if<ft::is_const<U>::value, U>::type * = 0)
            : ptr(const_cast<Node *>(other.base())), _end(const_cast<Node *>(other.end_base())) {}

        set_iterator &operator=(const set_iterator &other)
        {
            if (this != &other)
            {
                this->ptr = other.ptr;
                this->_end = other._end;
            }
            return *this;
        }

        //
        // Operators
        //
        Node *base() const
        {
            return ptr;
        }

        Node *end_base() const
        {
            return _end;
        }

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

        set_iterator operator++(int)
        {
            set_iterator<T, Node> tmp = *this;
            this->operator++();
            return tmp;
        }

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

        set_iterator operator--(int)
        {
            set_iterator<T, Node> tmp = *this;
            this->operator--();
            return tmp;
        }

        reference operator*() const
        {
            return ptr->data;
        }

        pointer operator->() const
        {
            return &(operator*());
        }

        template <class OtherNode>
        bool operator==(const set_iterator<T, OtherNode> &tocomp) const
        {
            return ptr == tocomp.base();
        }

        template <class OtherNode>
        bool operator!=(const set_iterator<T, OtherNode> &tocomp) const
        {
            return ptr != tocomp.base();
        }
        operator set_iterator<const T, const Node>() const
        {
            return set_iterator<const T, const Node>(ptr, _end);
        }
        

    private: // Private function like Next and Prev Node // Post order (root is end)
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

        Node *down_bigger_node()
        {
            Node *tmp = this->ptr->right;

            while (tmp->left)
                tmp = tmp->left;
            return tmp;
        }

        // Return next smallest
        Node *down_smallest_node()
        {
            Node *tmp = this->ptr->left;

            while (tmp->right)
                tmp = tmp->right;
            return tmp;
        }

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

    template <class T1, class Node1, class T2, class Node2>
    bool operator==(const set_iterator<T1, Node1> &lhs, const set_iterator<T2, Node2> &rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <class T1, class Node1, class T2, class Node2>
    bool operator!=(const set_iterator<T1, Node1> &lhs, const set_iterator<T2, Node2> &rhs)
    {
        return lhs.base() != rhs.base();
    }


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
