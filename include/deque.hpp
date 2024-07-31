#ifndef DEQUE_HPP
#define DEQUE_HPP

#include "utility.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"
#include "exception.hpp"
#include "type_traits.hpp"
namespace ft
{

    template <class T, class Alloc = std::allocator<T> >
    class deque
    {
    public:
        template <bool IsConst>
        class dequeIterator
        {
        public:
            // Member types
            typedef typename ft::conditional<IsConst, const T, T>::type value_type;
            typedef value_type &reference;
            typedef value_type *pointer;
            typedef ft::ptrdiff_t difference_type;
            typedef ft::size_t size_type;
            typedef ft::random_access_iterator_tag iterator_category;

            /**
             * Default constructor for dequeIterator
             * 
             * @throws None
             * @complexity O(1)
             */
            dequeIterator(void)
            {
                _ptr = NULL;
                _deq = NULL;
            }

            /**
             * Constructor with parameters for dequeIterator
             * 
             * @param ptr Pointer to the value
             * @param deq Pointer to the deque
             * 
             * @throws None
             * @complexity O(1)
             */
            dequeIterator(value_type *const ptr, const deque *deq) { this->init(ptr, deq); }

            /**
             * Destructor for dequeIterator
             * 
             * @throws None
             * @complexity O(1)
             */
            ~dequeIterator(void) {}

            /**
             * Copy constructor for dequeIterator
             * 
             * @param x Another dequeIterator to copy from
             * 
             * @throws None
             * @complexity O(1)
             */
            template <bool B>
            dequeIterator(const dequeIterator<B> &x, typename ft::enable_if<!B>::type * = 0) { this->init(x.getPtr(), x.getDeq()); }

            /**
             * Assignment operator for dequeIterator
             * 
             * @param x Another dequeIterator to assign from
             * @return Reference to the assigned dequeIterator
             * 
             * @throws None
             * @complexity O(1)
             */
            dequeIterator &operator=(const dequeIterator &x)
            {
                this->init(x.getPtr(), x.getDeq());
                return (*this);
            }

            /**
             * Addition assignment operator for dequeIterator
             * 
             * @param n Number of positions to advance
             * @return Reference to the modified dequeIterator
             * 
             * @throws None
             * @complexity O(n)
             */
            dequeIterator &operator+=(int n)
            {
                this->plus(n);
                return (*this);
            }

            /**
             * Subtraction assignment operator for dequeIterator
             * 
             * @param n Number of positions to move back
             * @return Reference to the modified dequeIterator
             * 
             * @throws None
             * @complexity O(n)
             */
            dequeIterator &operator-=(int n)
            {
                this->minus(n);
                return (*this);
            }

            /**
             * Equality comparison operator for dequeIterator
             * 
             * @param x Another dequeIterator to compare with
             * @return True if equal, false otherwise
             * 
             * @throws None
             * @complexity O(1)
             */
            template <bool B>
            bool operator==(const dequeIterator<B> &x) const { return (_ptr == x.getPtr()); }

            /**
             * Inequality comparison operator for dequeIterator
             * 
             * @param x Another dequeIterator to compare with
             * @return True if not equal, false otherwise
             * 
             * @throws None
             * @complexity O(1)
             */
            template <bool B>
            bool operator!=(const dequeIterator<B> &x) const { return (_ptr != x.getPtr()); }

            /**
             * Less than comparison operator for dequeIterator
             * 
             * @param x Another dequeIterator to compare with
             * @return True if less, false otherwise
             * 
             * @throws None
             * @complexity O(1)
             */
            template <bool B>
            bool operator<(const dequeIterator<B> &x) const { return (this->diff(x) < 0); }

            /**
             * Greater than comparison operator for dequeIterator
             * 
             * @param x Another dequeIterator to compare with
             * @return True if greater, false otherwise
             * 
             * @throws None
             * @complexity O(1)
             */
            template <bool B>
            bool operator>(const dequeIterator<B> &x) const { return (this->diff(x) > 0); }

            /**
             * Less than or equal comparison operator for dequeIterator
             * 
             * @param x Another dequeIterator to compare with
             * @return True if less or equal, false otherwise
             * 
             * @throws None
             * @complexity O(1)
             */
            template <bool B>
            bool operator<=(const dequeIterator<B> &x) const { return (this->diff(x) <= 0); }

            /**
             * Greater than or equal comparison operator for dequeIterator
             * 
             * @param x Another dequeIterator to compare with
             * @return True if greater or equal, false otherwise
             * 
             * @throws None
             * @complexity O(1)
             */
            template <bool B>
            bool operator>=(const dequeIterator<B> &x) const { return (this->diff(x) >= 0); }

            /**
             * Pre-increment operator for dequeIterator
             * 
             * @return Reference to the incremented dequeIterator
             * 
             * @throws None
             * @complexity O(1)
             */
            dequeIterator &operator++(void)
            {
                this->plus(1);
                return (*this);
            }

            /**
             * Pre-decrement operator for dequeIterator
             * 
             * @return Reference to the decremented dequeIterator
             * 
             * @throws None
             * @complexity O(1)
             */
            dequeIterator &operator--(void)
            {
                this->minus(1);
                return (*this);
            }

            /**
             * Post-increment operator for dequeIterator
             * 
             * @return Copy of the dequeIterator before increment
             * 
             * @throws None
             * @complexity O(1)
             */
            dequeIterator operator++(int)
            {
                dequeIterator<IsConst> x(*this);
                this->plus(1);
                return (x);
            }

            /**
             * Post-decrement operator for dequeIterator
             * 
             * @return Copy of the dequeIterator before decrement
             * 
             * @throws None
             * @complexity O(1)
             */
            dequeIterator operator--(int)
            {
                dequeIterator<IsConst> x(*this);
                this->minus(1);
                return (x);
            }

            /**
             * Addition operator for dequeIterator
             * 
             * @param n Number of positions to advance
             * @return New dequeIterator advanced by n positions
             * 
             * @throws None
             * @complexity O(n)
             */
            dequeIterator operator+(int n) const
            {
                dequeIterator<IsConst> x(*this);
                x.plus(n);
                return (x);
            }

            /**
             * Subtraction operator for dequeIterator
             * 
             * @param n Number of positions to move back
             * @return New dequeIterator moved back by n positions
             * 
             * @throws None
             * @complexity O(n)
             */
            dequeIterator operator-(int n) const
            {
                dequeIterator<IsConst> x(*this);
                x.minus(n);
                return (x);
            }

            /**
             * Subtraction operator for dequeIterator
             * 
             * @param x Another dequeIterator to subtract
             * @return Difference in positions between two dequeIterators
             * 
             * @throws None
             * @complexity O(1)
             */
            difference_type operator-(const dequeIterator &x) const { return (this->diff(x)); }

            /**
             * Subscript operator for dequeIterator
             * 
             * @param n Index
             * @return Reference to the value at index n
             * 
             * @throws None
             * @complexity O(1)
             */
            value_type &operator[](size_type n) const
            {
                dequeIterator<IsConst> x(*this);
                x.plus(n);
                return (*(x.getPtr()));
            }

            /**
             * Dereference operator for dequeIterator
             * 
             * @return Reference to the value pointed by the iterator
             * 
             * @throws None
             * @complexity O(1)
             */
            value_type &operator*(void) const { return (*_ptr); }

            /**
             * Member access operator for dequeIterator
             * 
             * @return Pointer to the value pointed by the iterator
             * 
             * @throws None
             * @complexity O(1)
             */
            value_type *operator->(void) const { return (_ptr); }

            /**
             * Get the pointer to the value
             * 
             * @return Pointer to the value
             * 
             * @throws None
             * @complexity O(1)
             */
            value_type *getPtr(void) const { return (_ptr); }

            /**
             * Get the deque associated with the iterator
             * 
             * @return Pointer to the deque
             * 
             * @throws None
             * @complexity O(1)
             */
            const deque *getDeq(void) const { return (_deq); }

            /**
             * Get the index in the deque
             * 
             * @return Index in the deque
             * 
             * @throws None
             * @complexity O(1)
             */
            size_type getI(void) const { return (_i); }

            /**
             * Check if the iterator is out of bounds
             * 
             * @return True if out of bounds, false otherwise
             * 
             * @throws None
             * @complexity O(1)
             */
            bool getOut(void) const { return (_out); }

            /**
             * Addition operator for dequeIterator with integer
             * 
             * @param n Number of positions to advance
             * @param x dequeIterator to add to
             * @return New dequeIterator advanced by n positions
             * 
             * @throws None
             * @complexity O(n)
             */
            friend dequeIterator operator+(int n, const dequeIterator &x) { return (x + n); }

        private:
            value_type *_ptr;
            const deque *_deq;
            size_type _i;
            bool _out;

            /**
             * Initialize the dequeIterator
             * 
             * @param ptr Pointer to the value
             * @param deq Pointer to the deque
             * 
             * @throws None
             * @complexity O(n^2)
             */
            void init(value_type *const ptr, const deque *deq)
            {
                _ptr = ptr;
                _deq = deq;
                _i = 0;
                _out = true;

                if (!_deq)
                    return;
                for (size_type i = 0; i < _deq->_size; i++)
                {
                    for (size_type j = 0; j < _deq->_node_size; j++)
                    {
                        if (_ptr == _deq->_map[i] + j)
                        {
                            _i = i;
                            _out = false;
                        }
                    }
                }
            }

            /**
             * Move the iterator forward or backward by n positions
             * 
             * @param n Number of positions to move
             * 
             * @throws None
             * @complexity O(n)
             */
            void plus(int n)
            {
                while (n > 0 && n--)
                    this->next();
                while (n < 0 && n++)
                    this->prev();
            }

            /**
             * Move the iterator backward or forward by n positions
             * 
             * @param n Number of positions to move
             * 
             * @throws None
             * @complexity O(n)
             */
            void minus(int n)
            {
                while (n > 0 && n--)
                    this->prev();
                while (n < 0 && n++)
                    this->next();
            }

            /**
             * Move the iterator to the next position
             * 
             * @throws None
             * @complexity O(1)
             */
            void next(void)
            {
                if (!_deq)
                    return;
                if (_ptr == _deq->_lastNode() + _deq->_node_size - 1)
                    _out = true;
                else if (_ptr == _deq->_firstNode() - 1)
                    _out = false;
                else if (_ptr == _deq->_map[_i] + _deq->_node_size - 1)
                {
                    _ptr = _deq->_map[++_i];
                    return;
                }
                _ptr++;
            }

            /**
             * Move the iterator to the previous position
             * 
             * @throws None
             * @complexity O(1)
             */
            void prev(void)
            {
                if (!_deq)
                    return;
                if (_ptr == _deq->_firstNode())
                    _out = true;
                else if (_ptr == _deq->_lastNode() + _deq->_node_size)
                    _out = false;
                else if (_ptr == _deq->_map[_i])
                {
                    _ptr = _deq->_map[--_i] + _deq->_node_size - 1;
                    return;
                }
                _ptr--;
            }

            /**
             * Calculate the difference between two iterators
             * 
             * @param x Another dequeIterator to calculate the difference with
             * @return Difference in positions between two dequeIterators
             * 
             * @throws None
             * @complexity O(1)
             */
            template <bool B>
            difference_type diff(const dequeIterator<B> &x) const
            {
                if (!_deq)
                    return (0);
                if (_out || x.getOut())
                    return (_ptr - x.getPtr());
                else
                    return ((_i - x.getI()) * _deq->_node_size + _ptr - x.getPtr());
            }
        }; // Iterator

        //////////////////////////////
        // Member types
        //////////////////////////////

        typedef T value_type;
        typedef Alloc allocator_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef dequeIterator<false> iterator;
        typedef dequeIterator<true> const_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef typename dequeIterator<false>::difference_type difference_type;
        typedef typename dequeIterator<false>::size_type size_type;

        private:
            allocator_type _alloc;
            value_type **_map;
            size_type _size;
            size_type _start;
            size_type _end;
            static const size_type _node_size = 8;

        public:

        //////////////////////////////
        // Constructors
        //////////////////////////////

        /**
         * Default constructor for deque
         * 
         * @param alloc Allocator to use
         * 
         * @throws None
         * @complexity O(1)
         */
        explicit deque(const allocator_type &alloc = allocator_type())
        {
            this->_init(alloc);
        }

        /**
         * Fill constructor for deque
         * 
         * @param n Number of elements
         * @param val Value to fill the deque with
         * @param alloc Allocator to use
         * 
         * @throws None
         * @complexity O(n)
         */
        explicit deque(size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type())
        {
            this->_init(alloc);
            this->assign(n, val);
        }

        /**
         * Range constructor for deque
         * 
         * @param first Iterator to the beginning of the range
         * @param last Iterator to the end of the range
         * @param alloc Allocator to use
         * 
         * @throws Exception if allocation fails
         * @complexity O(n)
         */
        template <class InputIterator>
        deque(InputIterator first, InputIterator last,
              const allocator_type &alloc = allocator_type(),
              typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type * = 0)
            : _alloc(alloc)
        {
            this->_init(alloc);

            for (; first != last; ++first)
                this->push_back(*first);
        }

        /**
         * Copy constructor for deque
         * 
         * @param x Another deque to copy from
         * 
         * @throws None
         * @complexity O(n)
         */
        deque(const deque &x)
        {
            this->_init(x._alloc);
            *this = x;
        }

        /**
         * Destructor for deque
         * 
         * @throws None
         * @complexity O(n)
         */
        ~deque(void)
        {
            this->clear();
            _alloc.deallocate(_map[0], this->_node_size);
            delete[] _map;
        }

        /**
         * Assignment operator for deque
         * 
         * @param x Another deque to assign from
         * @return Reference to the assigned deque
         * 
         * @throws None
         * @complexity O(n)
         */
        deque &operator=(const deque &x)
        {
            if (this == &x)
                return (*this);

            this->assign(x.begin(), x.end());
            return (*this);
        }

        //////////////////////////////
        // Iterators
        //////////////////////////////

        /**
         * Get iterator to the beginning of the deque
         * 
         * @return Iterator to the beginning
         * 
         * @throws None
         * @complexity O(1)
         */
        iterator begin(void)
        {
            return (iterator(this->_firstNode() + _start, this));
        }

        /**
         * Get constant iterator to the beginning of the deque
         * 
         * @return Constant iterator to the beginning
         * 
         * @throws None
         * @complexity O(1)
         */
        const_iterator begin(void) const
        {
            return (const_iterator(this->_firstNode() + _start, this));
        }

        /**
         * Get iterator to the end of the deque
         * 
         * @return Iterator to the end


         * 
         * @throws None
         * @complexity O(1)
         */
        iterator end(void)
        {
            return (iterator(this->_lastNode() + _end, this));
        }

        /**
         * Get constant iterator to the end of the deque
         * 
         * @return Constant iterator to the end
         * 
         * @throws None
         * @complexity O(1)
         */
        const_iterator end(void) const
        {
            return (const_iterator(this->_lastNode() + _end, this));
        }

        //////////////////////////////
        // Reverse iterators
        //////////////////////////////

        /**
         * Get reverse iterator to the beginning of the reversed deque
         * 
         * @return Reverse iterator to the beginning
         * 
         * @throws None
         * @complexity O(1)
         */
        reverse_iterator rbegin(void)
        {
            return (reverse_iterator(this->end()));
        }

        /**
         * Get constant reverse iterator to the beginning of the reversed deque
         * 
         * @return Constant reverse iterator to the beginning
         * 
         * @throws None
         * @complexity O(1)
         */
        const_reverse_iterator rbegin(void) const
        {
            return (const_reverse_iterator(this->end()));
        }

        /**
         * Get reverse iterator to the end of the reversed deque
         * 
         * @return Reverse iterator to the end
         * 
         * @throws None
         * @complexity O(1)
         */
        reverse_iterator rend(void)
        {
            return (reverse_iterator(this->begin()));
        }

        /**
         * Get constant reverse iterator to the end of the reversed deque
         * 
         * @return Constant reverse iterator to the end
         * 
         * @throws None
         * @complexity O(1)
         */
        const_reverse_iterator rend(void) const
        {
            return (const_reverse_iterator(this->begin()));
        }

        //////////////////////////////
        // Capacity
        //////////////////////////////

        /**
         * Get the size of the deque
         * 
         * @return Number of elements in the deque
         * 
         * @throws None
         * @complexity O(1)
         */
        size_type size(void) const
        {
            if (this->empty())
                return (0);
            return ((_size - 1) * this->_node_size + _end - _start);
        }

        /**
         * Get the maximum size of the deque
         * 
         * @return Maximum number of elements the deque can hold
         * 
         * @throws None
         * @complexity O(1)
         */
        size_type max_size(void) const
        {
            return (_alloc.max_size());
        }

        /**
         * Resize the deque to contain n elements
         * 
         * @param n New size of the deque
         * @param val Value to initialize the new elements with
         * 
         * @throws None
         * @complexity O(n)
         */
        void resize(size_type n, value_type val = value_type())
        {
            if (n > this->size())
            {
                while (n > this->size())
                    this->push_back(val);
            }
            else if (n < this->size())
            {
                while (n < this->size())
                    this->pop_back();
            }
        }

        /**
         * Check if the deque is empty
         * 
         * @return True if the deque is empty, false otherwise
         * 
         * @throws None
         * @complexity O(1)
         */
        bool empty(void) const
        {
            return (_size == 1 && this->_start == this->_end);
        }

        //////////////////////////////
        // Member access
        //////////////////////////////

        /**
         * Subscript operator for deque
         * 
         * @param n Index of the element to access
         * @return Reference to the element at index n
         * 
         * @throws None
         * @complexity O(1)
         */
        reference operator[](size_type n)
        {
            if (_start + n < this->_node_size)
                return (_map[0][_start + n]);
            else
                n = n + _start - this->_node_size;
            return (_map[n / this->_node_size + 1][n % this->_node_size]);
        }

        /**
         * Subscript operator for constant deque
         * 
         * @param n Index of the element to access
         * @return Constant reference to the element at index n
         * 
         * @throws None
         * @complexity O(1)
         */
        const_reference operator[](size_type n) const
        {
            if (_start + n < this->_node_size)
                return (_map[0][_start + n]);
            else
                n = n + _start - this->_node_size;
            return (_map[n / this->_node_size + 1][n % this->_node_size]);
        }

        /**
         * Access element at position n with bounds checking
         * 
         * @param n Index of the element to access
         * @return Reference to the element at index n
         * 
         * @throws ft::out_of_range if n is out of range
         * @complexity O(1)
         */
        reference at(size_type n)
        {
            if (this->size() <= n)
                throw ft::out_of_range("deque");
            return ((*this)[n]);
        }

        /**
         * Access element at position n with bounds checking (const)
         * 
         * @param n Index of the element to access
         * @return Constant reference to the element at index n
         * 
         * @throws ft::out_of_range if n is out of range
         * @complexity O(1)
         */
        const_reference at(size_type n) const
        {
            if (this->size() <= n)
                throw ft::out_of_range("deque");
            return ((*this)[n]);
        }

        /**
         * Access the first element of the deque
         * 
         * @return Reference to the first element
         * 
         * @throws None
         * @complexity O(1)
         */
        reference front(void)
        {
            return (*this->begin());
        }

        /**
         * Access the first element of the deque (const)
         * 
         * @return Constant reference to the first element
         * 
         * @throws None
         * @complexity O(1)
         */
        const_reference front(void) const
        {
            return (*this->begin());
        }

        /**
         * Access the last element of the deque
         * 
         * @return Reference to the last element
         * 
         * @throws None
         * @complexity O(1)
         */
        reference back(void)
        {
            return (*this->rbegin());
        }

        /**
         * Access the last element of the deque (const)
         * 
         * @return Constant reference to the last element
         * 
         * @throws None
         * @complexity O(1)
         */
        const_reference back(void) const
        {
            return (*this->rbegin());
        }

        //////////////////////////////
        // Assignment modifiers
        //////////////////////////////

        /**
         * Assign a range of elements to the deque
         * 
         * @param first Iterator to the beginning of the range
         * @param last Iterator to the end of the range
         * 
         * @throws None
         * @complexity O(n)
         */
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last,
                    typename ft::enable_if<!ft::is_same<InputIterator, int>::value>::type * = 0)
        {
            this->clear();
            while (first != last)
                this->push_back(*first++);
        }

        /**
         * Assign n elements to the deque
         * 
         * @param n Number of elements
         * @param val Value to assign
         * 
         * @throws None
         * @complexity O(n)
         */
        void assign(size_type n, const value_type &val)
        {
            this->clear();
            while (n--)
                this->push_back(val);
        }

        //////////////////////////////
        // Insertion modifiers
        //////////////////////////////

        /**
         * Insert an element at a given position
         * 
         * @param position Iterator to the position
         * @param val Value to insert
         * @return Iterator to the inserted element
         * 
         * @throws None
         * @complexity O(n)
         */
        iterator insert(iterator position, const value_type &val)
        {
            this->push_back(val);
            for (iterator it = --(this->end()), prev = --iterator(it); it != position; it--, prev--)
                *it = *prev;
            *position = val;
            return (position);
        }

        /**
         * Insert n elements at a given position
         * 
         * @param position Iterator to the position
         * @param n Number of elements to insert
         * @param val Value to insert
         * 
         * @throws None
         * @complexity O(n^2)
         */
        void insert(iterator position, size_type n, const value_type &val)
        {
            while (n--)
                this->insert(position, val);
        }

        /**
         * Insert a range of elements at a given position
         * 
         * @param position Iterator to the position
         * @param first Iterator to the beginning of the range
         * @param last Iterator to the end of the range
         * 
         * @throws None
         * @complexity O(n^2)
         */
        template <class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last,
                    typename ft::enable_if<!ft::is_same<InputIterator, int>::value>::type * = 0)
        {
            while (first != last)
            {
                this->insert(position, *first++);
                position++;
            }
        }

        //////////////////////////////
        // Erasure modifiers
        //////////////////////////////

        /**
         * Erase an element at a given position
         * 
         * @param position Iterator to the position
         * @return Iterator to the next element
         * 
         * @throws None
         * @complexity O(n)
         */
        iterator erase(iterator position)
        {
            if (!this->empty())
            {
                iterator next = position;
                for (iterator it = next++; position != this->end() && next != this->end(); it++, next++)
                    *it = *next;
                this->pop_back();
            }
            return (position);
        }

        /**
         * Erase a range of elements
         * 
         * @param first Iterator to the beginning of the range
         * @param last Iterator to the end of the range
         * @return Iterator to the next element
         * 
         * @throws None
         * @complexity O(n)
         */
        iterator erase(iterator first, iterator last)
        {
            if (first == last)
                return first;

            difference_type n = ft::distance(first, last);
            iterator it = first;
            iterator end_it = this->end();

            // Move the elements after 'last' to 'first'
            while (last != end_it)
            {
                *first = *last;
                ++first;
                ++last;
            }

            // Destroy the now-redundant elements at the end
            while (n > 0)
            {
                this->pop_back();
                --n;
            }

            return it;
        }

        //////////////////////////////
        // Common modifiers
        //////////////////////////////

        /**
         * Add an element to the end of the deque
         * 
         * @param val Value to add
         * 
         * @throws None
         * @complexity O(1) amortized
         */
        void push_back(const value_type &val)
        {
            _alloc.construct(this->_lastNode() + _end, val);
            if (_end == this->_node_size - 1)
                this->_expandBack();
            else
                _end++;
        }

        /**
         * Add an element to the front of the deque
         * 
         * @param val Value to add
         * 
         * @throws None
         * @complexity O(1) amortized
         */
        void push_front(const value_type &val)
        {
            if (_start == 0)
                this->_expandFront();
            else
                _start--;
            _alloc.construct(this->_firstNode() + _start, val);
        }

        /**
         * Remove the last element from the deque
         * 
         * @throws None
         * @complexity O(1) amortized
         */
        void pop_back(void)
        {
            if (!this->empty())
            {
                if (_end == 0)
                    this->_contractBack();
                else
                    _end--;
                _alloc.destroy(this->_lastNode() + _end);
            }
        }

        /**
         * Remove the first element from the deque
         * 
         * @throws None
         * @complexity O(1) amortized
         */
        void pop_front(void)
        {
            if (!this->empty())
            {
                _alloc.destroy(this->_firstNode() + _start);
                if (_start == this->_node_size - 1)
                    this->_contractFront();
                else
                    _start++;
            }
        }

        /**
         * Swap the contents of two deques
         * 
         * @param x Another deque to swap with
         * 
         * @throws None
         * @complexity O(1)
         */
        void swap(deque &x)
        {
            ft::swap(_alloc, x._alloc);
            ft::swap(_map, x._map);
            ft::swap(_size, x._size);
            ft::swap(_start, x._start);
            ft::swap(_end, x._end);
        }

        /**
         * Clear the contents of the deque
         * 
         * @throws None
         * @complexity O(n)
         */
        void clear(void)
        {
            while (!this->empty())
                this->pop_back();
        }

        /**
         * Get the allocator used by the deque
         * 
         * @return Allocator
         * 
         * @throws None
         * @complexity O(1)
         */
        allocator_type get_allocator(void) const
        {
            return (allocator_type());
        }
        //////////////////////////////
        // Private functions
        //////////////////////////////

    private:
        /**
         * Initialize the deque
         * 
         * @param alloc Allocator to use
         * 
         * @throws None
         * @complexity O(1)
         */
        void _init(allocator_type alloc)
        {
            _alloc = alloc;
            _map = new value_type *[1];
            _map[0] = _alloc.allocate(this->_node_size);
            _size = 1;
            _start = 0;
            _end = 0;
        }

        /**
         * Get the first node in the deque
         * 
         * @return Pointer to the first node
         * 
         * @throws None
         * @complexity O(1)
         */
        value_type *_firstNode(void) const
        {
            return (_map[0]);
        }

        /**
         * Get the last node in the deque
         * 
         * @return Pointer to the last node
         * 
         * @throws None
         * @complexity O(1)
         */
        value_type *_lastNode(void) const
        {
            return (_map[_size - 1]);
        }

        /**
         * Expand the deque by adding a new node at the back
         * 
         * @throws None
         * @complexity O(n)
         */
        void _expandBack(void)
        {
            this->_resizeMap(_size + 1, 0, 0);
            _map[_size] = _alloc.allocate(this->_node_size);
            _size++;
            _end = 0;
        }

        /**
         * Expand the deque by adding a new node at the front
         * 
         * @throws None
         * @complexity O(n)
         */
        void _expandFront(void)
        {
            this->_resizeMap(_size + 1, 0, 1);
            _map[0] = _alloc.allocate(this->_node_size);
            _size++;
            _start = this->_node_size - 1;
        }

        /**
         * Contract the deque by removing the last node
         * 
         * @throws None
         * @complexity O(n)
         */
        void _contractBack(void)
        {
            _alloc.deallocate(_map[_size - 1], this->_node_size);
            this->_resizeMap(_size - 1, 0, 0);
            _size--;
            _end = this->_node_size - 1;
        }

        /**
         * Contract the deque by removing the first node
         * 
         * @throws None
         * @complexity O(n)
         */
        void _contractFront(void)
        {
            _alloc.deallocate(_map[0], this->_node_size);
            this->_resizeMap(_size - 1, 1, 0);
            _size--;
            _start = 0;
        }

        /**
         * Resize the internal map of the deque
         * 
         * @param new_size New size of the map
         * @param offmap Offset in the current map
         * @param offnew Offset in the new map
         * 
         * @throws None
         * @complexity O(n)
         */
        void _resizeMap(size_type new_size, size_type offmap, size_type offnew)
        {
            value_type **_new = new value_type *[new_size];

            while (offmap < _size && offnew < new_size)
                _new[offnew++] = _map[offmap++];

            delete[] _map;
            _map = _new;
        }


    }; // Deque

    //////////////////////////////
    // Relational operators
    //////////////////////////////

    /**
     * Equality comparison operator for deque
     * 
     * @param lhs Left-hand side deque
     * @param rhs Right-hand side deque
     * @return True if equal, false otherwise
     * 
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator==(const deque<T, Alloc> &lhs, const deque<T, Alloc> &rhs)
    {
        return (ft::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
    }

    /**
     * Less than comparison operator for deque
     * 
     * @param lhs Left-hand side deque
     * @param rhs Right-hand side deque
     * @return True if lhs is less than rhs, false otherwise
     * 
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator<(const deque<T, Alloc> &lhs, const deque<T, Alloc> &rhs)
    {
        return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
    }

    /**
     * Inequality comparison operator for deque
     * 
     * @param lhs Left-hand side deque
     * @param rhs Right-hand side deque
     * @return True if not equal, false otherwise
     * 
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator!=(const deque<T, Alloc> &lhs, const deque<T, Alloc> &rhs)
    {
        return (!(lhs == rhs));
    }

    /**
     * Less than or equal comparison operator for deque
     * 
     * @param lhs Left-hand side deque
     * @param rhs Right-hand side deque
     * @return True if lhs is less than or equal to rhs, false otherwise
     * 
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator<=(const deque<T, Alloc> &lhs, const deque<T, Alloc> &rhs)
    {
        return (!(rhs < lhs));
    }

    /**
     * Greater than comparison operator for deque
     * 
     * @param lhs Left-hand side deque
     * @param rhs Right-hand side deque
     * @return True if lhs is greater than rhs, false otherwise
     * 
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator>(const deque<T, Alloc> &lhs, const deque<T, Alloc> &rhs)
    {
        return (rhs < lhs);
    }

    /**
     * Greater than or equal comparison operator for deque
     * 
     * @param lhs Left-hand side deque
     * @param rhs Right-hand side deque
     * @return True if lhs is greater than or equal to rhs, false otherwise
     * 
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator>=(const deque<T, Alloc> &lhs, const deque<T, Alloc> &rhs)
    {
        return (!(lhs < rhs));
    }

    /**
     * Swap the contents of two deques
     * 
     * @param x First deque
     * @param y Second deque
     * 
     * @throws None
     * @complexity O(1)
     */
    template <class T, class Alloc>
    void swap(deque<T, Alloc> &x, deque<T, Alloc> &y)
    {
        x.swap(y);
    }

} // Namespace ft

#endif