#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "utility.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"
#include "exception.hpp"
#include "type_traits.hpp"

namespace ft
{
    template <class T, class Allocator = std::allocator<T> >
    class vector
    {
    public:
        typedef Allocator allocator_type;
        typedef typename allocator_type::size_type size_type;
        typedef T value_type;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef typename allocator_type::difference_type difference_type;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef pointer iterator;
        typedef const_pointer const_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

        template <bool IsConst>
        class vectorIterator
        {
        public:
            typedef typename ft::conditional<IsConst, const T, T>::type value_type;
            typedef value_type &reference;
            typedef value_type *pointer;
            typedef ft::ptrdiff_t difference_type;
            typedef ft::size_t size_type;
            typedef ft::random_access_iterator_tag iterator_category;

            /**
             * Default constructor for vectorIterator. Initializes the pointer to NULL.
             * 
             * @complexity O(1)
             */
            vectorIterator(void) { _ptr = NULL; }

            /**
             * Destructor for vectorIterator.
             * 
             * @complexity O(1)
             */
            ~vectorIterator(void) {}

            /**
             * Copy constructor for vectorIterator.
             * 
             * @tparam B Boolean value indicating whether the iterator is const
             * @param x The iterator to copy from
             * 
             * @complexity O(1)
             */
            template <bool B>
            vectorIterator(const vectorIterator<B> &x, typename ft::enable_if<!B>::type * = 0) { _ptr = x.getPtr(); }

            /**
             * Assignment operator for vectorIterator.
             * 
             * @param x The iterator to assign from
             * @return Reference to this iterator
             * 
             * @complexity O(1)
             */
            vectorIterator &operator=(const vectorIterator &x)
            {
                _ptr = x.getPtr();
                return (*this);
            }

            /**
             * Addition assignment operator.
             * 
             * @param n The number of elements to advance
             * @return Reference to this iterator
             * 
             * @complexity O(1)
             */
            vectorIterator &operator+=(int n)
            {
                _ptr += n;
                return (*this);
            }

            /**
             * Subtraction assignment operator.
             * 
             * @param n The number of elements to retreat
             * @return Reference to this iterator
             * 
             * @complexity O(1)
             */
            vectorIterator &operator-=(int n)
            {
                _ptr -= n;
                return (*this);
            }

            /**
             * Equality operator for comparing two iterators.
             * 
             * @tparam B Boolean value indicating whether the iterator is const
             * @param x The iterator to compare with
             * @return True if the iterators are equal, false otherwise
             * 
             * @complexity O(1)
             */
            template <bool B>
            bool operator==(const vectorIterator<B> &x) const { return (_ptr == x.getPtr()); }

            /**
             * Inequality operator for comparing two iterators.
             * 
             * @tparam B Boolean value indicating whether the iterator is const
             * @param x The iterator to compare with
             * @return True if the iterators are not equal, false otherwise
             * 
             * @complexity O(1)
             */
            template <bool B>
            bool operator!=(const vectorIterator<B> &x) const { return (_ptr != x.getPtr()); }

            /**
             * Less-than operator for comparing two iterators.
             * 
             * @tparam B Boolean value indicating whether the iterator is const
             * @param x The iterator to compare with
             * @return True if this iterator is less than the other, false otherwise
             * 
             * @complexity O(1)
             */
            template <bool B>
            bool operator<(const vectorIterator<B> &x) const { return (_ptr < x.getPtr()); }

            /**
             * Greater-than operator for comparing two iterators.
             * 
             * @tparam B Boolean value indicating whether the iterator is const
             * @param x The iterator to compare with
             * @return True if this iterator is greater than the other, false otherwise
             * 
             * @complexity O(1)
             */
            template <bool B>
            bool operator>(const vectorIterator<B> &x) const { return (_ptr > x.getPtr()); }

            /**
             * Less-than-or-equal-to operator for comparing two iterators.
             * 
             * @tparam B Boolean value indicating whether the iterator is const
             * @param x The iterator to compare with
             * @return True if this iterator is less than or equal to the other, false otherwise
             * 
             * @complexity O(1)
             */
            template <bool B>
            bool operator<=(const vectorIterator<B> &x) const { return (_ptr <= x.getPtr()); }

            /**
             * Greater-than-or-equal-to operator for comparing two iterators.
             * 
             * @tparam B Boolean value indicating whether the iterator is const
             * @param x The iterator to compare with
             * @return True if this iterator is greater than or equal to the other, false otherwise
             * 
             * @complexity O(1)
             */
            template <bool B>
            bool operator>=(const vectorIterator<B> &x) const { return (_ptr >= x.getPtr()); }

            /**
             * Pre-increment operator. Advances the iterator to the next element.
             * 
             * @return Reference to this iterator
             * 
             * @complexity O(1)
             */
            vectorIterator &operator++(void)
            {
                _ptr++;
                return (*this);
            }

            /**
             * Pre-decrement operator. Retreats the iterator to the previous element.
             * 
             * @return Reference to this iterator
             * 
             * @complexity O(1)
             */
            vectorIterator &operator--(void)
            {
                _ptr--;
                return (*this);
            }

            /**
             * Post-increment operator. Advances the iterator to the next element.
             * 
             * @return Copy of this iterator before the increment
             * 
             * @complexity O(1)
             */
            vectorIterator operator++(int)
            {
                vectorIterator<IsConst> x(*this);
                _ptr++;
                return (x);
            }

            /**
             * Post-decrement operator. Retreats the iterator to the previous element.
             * 
             * @return Copy of this iterator before the decrement
             * 
             * @complexity O(1)
             */
            vectorIterator operator--(int)
            {
                vectorIterator<IsConst> x(*this);
                _ptr--;
                return (x);
            }

            /**
             * Addition operator. Advances the iterator by n elements.
             * 
             * @param n The number of elements to advance
             * @return A new iterator advanced by n elements
             * 
             * @complexity O(1)
             */
            vectorIterator operator+(int n) const { return (_ptr + n); }

            /**
             * Subtraction operator. Retreats the iterator by n elements.
             * 
             * @param n The number of elements to retreat
             * @return A new iterator retreated by n elements
             * 
             * @complexity O(1)
             */
            vectorIterator operator-(int n) const { return (_ptr - n); }

            /**
             * Difference operator. Returns the distance between two iterators.
             * 
             * @param x The iterator to subtract
             * @return The distance between this iterator and x
             * 
             * @complexity O(1)
             */
            difference_type operator-(const vectorIterator &x) const { return (_ptr - x.getPtr()); }

            /**
             * Subscript operator. Accesses the element at the given index.
             * 
             * @param n The index of the element
             * @return Reference to the element at the given index
             * 
             * @complexity O(1)
             */
            value_type &operator[](size_type n) const { return (*(_ptr + n)); }

            /**
             * Dereference operator. Accesses the element pointed to by the iterator.
             * 
             * @return Reference to the element
             * 
             * @complexity O(1)
             */
            value_type &operator*(void) const { return (*_ptr); }

            /**
             * Member access operator. Accesses the element pointed to by the iterator.
             * 
             * @return Pointer to the element
             * 
             * @complexity O(1)
             */
            value_type *operator->(void) const { return (_ptr); }

            /**
             * Returns the pointer held by the iterator.
             * 
             * @return The pointer
             * 
             * @complexity O(1)
             */
            value_type *getPtr(void) const { return (_ptr); }

            /**
             * Addition operator. Advances the iterator by n elements.
             * 
             * @param n The number of elements to advance
             * @param x The iterator
             * @return A new iterator advanced by n elements
             * 
             * @complexity O(1)
             */
            friend vectorIterator operator+(int n, const vectorIterator &x) { return (x.getPtr() + n); }

            /**
             * Constructor for vectorIterator with a pointer.
             * 
             * @param ptr The pointer
             * 
             * @complexity O(1)
             */
            vectorIterator(value_type *const ptr) { _ptr = ptr; }

        private:
            value_type *_ptr;
        };

    private:
        value_type *arr;
        size_type _size;
        size_type _cap;
        allocator_type _alloc;

    public:
        /**
         * Default constructor. Constructs an empty vector.
         * 
         * @param alloc Allocator object
         * 
         * @complexity O(1)
         */
        explicit vector(const allocator_type &alloc = allocator_type())
            : _size(0), _cap(0), _alloc(alloc)
        {
            arr = _alloc.allocate(0);
        }

        /**
         * Constructs a vector with n elements, each initialized to val.
         * 
         * @param n Number of elements
         * @param val Value to initialize elements with
         * @param alloc Allocator object
         * 
         * @complexity O(n)
         */
        explicit vector(size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type())
            : arr(NULL), _size(0), _cap(0), _alloc(alloc)
        {
            resize(n, val);
        }

        /**
         * Constructs a vector with the elements in the range [first, last).
         * 
         * @tparam InputIterator Iterator type
         * @param first Iterator to the first element
         * @param last Iterator to the last element
         * @param alloc Allocator object
         * 
         * @complexity O(n)
         */
        template <typename InputIterator>
        vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type(), typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type * = 0)
            : arr(NULL), _size(0), _cap(0), _alloc(alloc)
        {
            assign(first, last);
        }

        /**
         * Copy constructor. Constructs a vector with a copy of the elements in x.
         * 
         * @param x The vector to copy
         * 
         * @complexity O(n)
         */
        vector(const vector &x)
            : _size(x._size), _cap(x._cap), _alloc(x._alloc)
        {
            if (_cap > 0)
            {
                arr = _alloc.allocate(_cap);
                for (size_type i = 0; i < _size; i++)
                    _alloc.construct(&arr[i], x.arr[i]);
            }
        }

        /**
         * Destructor. Destroys the vector and deallocates its elements.
         * 
         * @complexity O(n)
         */
        ~vector()
        {
            clear();
            if (_cap)
                _alloc.deallocate(arr, _cap);
        }

        /**
         * Assignment operator. Assigns the contents of x to this vector.
         * 
         * @param x The vector to assign from
         * @return Reference to this vector
         * 
         * @complexity O(n)
         */
        vector &operator=(const vector &x)
        {
            if (this == &x)
                return *this;
            for (size_type i = 0; i < _size; i++)
                _alloc.destroy(&arr[i]);
            _size = x._size;
            _cap = x._cap;
            arr = _alloc.allocate(_cap);

            const_iterator first = x.begin();
            const_iterator last = x.end();
            for (size_type i = 0; first != last; ++first)
                _alloc.construct(&arr[i++], *first);
            return *this;
        }

        /**
         * Returns an iterator to the first element of the vector.
         * 
         * @return Iterator to the first element
         * 
         * @complexity O(1)
         */
        iterator begin()
        {
            return arr;
        }

        /**
         * Returns a const iterator to the first element of the vector.
         * 
         * @return Const iterator to the first element
         * 
         * @complexity O(1)
         */
        const_iterator begin() const
        {
            return arr;
        }

        /**
         * Returns an iterator to the element following the last element of the vector.
         * 
         * @return Iterator to the element following the last element
         * 
         * @complexity O(1)
         */
        iterator end()
        {
            return arr + _size;
        }

        /**
         * Returns a const iterator to the element following the last element of the vector.
         * 
         * @return Const iterator to the element following the last element
         * 
         * @complexity O(1)
         */
        const_iterator end() const
        {
            return arr + _size;
        }

        /**
         * Returns a reverse iterator to the first element of the reversed vector.
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
         * Returns a const reverse iterator to the first element of the reversed vector.
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
         * Returns a reverse iterator to the element following the last element of the reversed vector.
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
         * Returns a const reverse iterator to the element following the last element of the reversed vector.
         * 
         * @return Const reverse iterator to the element following the last element
         * 
         * @complexity O(1)
         */
        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

        /**
         * Checks if the vector is empty.
         * 
         * @return True if the vector is empty, false otherwise
         * 
         * @complexity O(1)
         */
        bool empty() const
        {
            return _size == 0;
        }

        /**
         * Returns the number of elements in the vector.
         * 
         * @return Number of elements in the vector
         * 
         * @complexity O(1)
         */
        size_type size() const
        {
            return _size;
        }

        /**
         * Returns the maximum number of elements the vector can hold.
         * 
         * @return Maximum number of elements
         * 
         * @complexity O(1)
         */
        size_type max_size() const
        {
            return _alloc.max_size();
        }

        /**
         * Resizes the vector to contain n elements.
         * 
         * @param n New size of the vector
         * @param val Value to initialize new elements with
         * 
         * @complexity O(n)
         */
        void resize(size_type n, value_type val = value_type())
        {
            if (n > _cap)
                reserve(n);
            if (n < size())
            {
                for (size_type i = n; i < _size; i++)
                    _alloc.destroy(&arr[i]);
            }
            else
            {
                for (size_type i = _size; i < n; i++)
                    _alloc.construct(&arr[i], val);
            }
            _size = n;
        }

        /**
         * Returns the number of elements that can be held in currently allocated storage.
         * 
         * @return Capacity of the vector
         * 
         * @complexity O(1)
         */
        size_type capacity() const
        {
            return _cap;
        }

        /**
         * Increases the capacity of the vector to a value that's greater or equal to n.
         * 
         * @param n New capacity of the vector
         * 
         * @throws ft::length_error if n is greater than max_size()
         * @complexity O(n)
         */
        void reserve(size_type n)
        {
            if (n > this->_alloc.max_size())
                throw ft::length_error("vector");
            if (n > this->_cap)
            {
                pointer new_arr = this->_alloc.allocate(n);
                for (size_type i = 0; i < this->_size && i < n; i++)
                {
                    this->_alloc.construct(&new_arr[i], this->arr[i]);
                    this->_alloc.destroy(&this->arr[i]);
                }
                if (_cap)
                    this->_alloc.deallocate(this->arr, this->_cap);

                this->_cap = n;
                this->arr = new_arr;
            }
        }

        /**
         * Accesses the element at the given index.
         * 
         * @param n Index of the element
         * @return Reference to the element
         * 
         * @complexity O(1)
         */
        reference operator[](size_type n)
        {
            return arr[n];
        }

        /**
         * Accesses the element at the given index (const version).
         * 
         * @param n Index of the element
         * @return Const reference to the element
         * 
         * @complexity O(1)
         */
        const_reference operator[](size_type n) const
        {
            return arr[n];
        }

        /**
         * Accesses the element at the given index with bounds checking.
         * 
         * @param n Index of the element
         * @return Reference to the element
         * 
         * @throws ft::out_of_range if n is out of bounds
         * @complexity O(1)
         */
        reference at(size_type n)
        {
            if (n >= _size)
                throw ft::out_of_range("vector");
            return arr[n];
        }

        /**
         * Accesses the element at the given index with bounds checking (const version).
         * 
         * @param n Index of the element
         * @return Const reference to the element
         * 
         * @throws ft::out_of_range if n is out of bounds
         * @complexity O(1)
         */
        const_reference at(size_type n) const
        {
            if (n >= _size)
                throw ft::out_of_range("vector");
            return arr[n];
        }

        /**
         * Accesses the first element.
         * 
         * @return Reference to the first element
         * 
         * @complexity O(1)
         */
        reference front()
        {
            return arr[0];
        }

        /**
         * Accesses the first element (const version).
         * 
         * @return Const reference to the first element
         * 
         * @complexity O(1)
         */
        const_reference front() const
        {
            return arr[0];
        }

        /**
         * Accesses the last element.
         * 
         * @return Reference to the last element
         * 
         * @complexity O(1)
         */
        reference back()
        {
            return arr[_size - 1];
        }

        /**
         * Accesses the last element (const version).
         * 
         * @return Const reference to the last element
         * 
         * @complexity O(1)
         */
        const_reference back() const
        {
            return arr[_size - 1];
        }

        /**
         * Assigns values to the vector from a range.
         * 
         * @tparam InputIterator Iterator type
         * @param first Iterator to the first element
         * @param last Iterator to the last element
         * 
         * @complexity O(n)
         */
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type * = 0)
        {
            clear();
            for (InputIterator i = first; i != last; ++i)
                push_back(*i);
        }

        /**
         * Assigns n values to the vector.
         * 
         * @param n Number of elements
         * @param val Value to assign to elements
         * 
         * @complexity O(n)
         */
        void assign(size_type n, const value_type &val)
        {
            clear();
            while (n--)
                push_back(val);
        }

        /**
         * Adds an element to the end of the vector.
         * 
         * @param val Value to add
         * 
         * @complexity O(1) amortized
         */
        void push_back(const value_type &val)
        {
            if (_size + 1 > _cap)
                reserve(_cap == 0 ? 1 : _cap * 2);
            _alloc.construct(&arr[_size++], val);
        }

        /**
         * Removes the last element of the vector.
         * 
         * @complexity O(1)
         */
        void pop_back()
        {
            _alloc.destroy(&arr[_size - 1]);
            _size--;
        }

        /**
         * Inserts an element at the given position.
         * 
         * @param position Iterator to the position
         * @param val Value to insert
         * @return Iterator to the inserted element
         * 
         * @complexity O(n)
         */
        iterator insert(iterator position, const value_type &val)
        {
            size_type off = position - this->begin();
            this->insert(position, 1, val);
            return (iterator(arr + off));
        }

        /**
         * Inserts n elements at the given position.
         * 
         * @param position Iterator to the position
         * @param n Number of elements to insert
         * @param val Value to insert
         * 
         * @complexity O(n)
         */
        void insert(iterator position, size_type n, const value_type &val)
        {
            size_type off = position - this->begin();

            if (_size + n > _cap)
            {
                if (_size + n > _cap * 2)
                    this->reserve(_size + n);
                else if (_cap > 0)
                    this->reserve(_cap * 2);
                else
                    this->reserve(1);
            }

            for (size_type i = 0; i < n; i++)
                _alloc.construct(arr + _size + i, val);
            for (int i = _size - 1; i >= 0 && i >= (int)off; i--)
                arr[i + n] = arr[i];
            for (size_type i = off; i < off + n; i++)
                arr[i] = val;
            _size = _size + n;
        }

        /**
         * Inserts elements from a range at the given position.
         * 
         * @tparam InputIterator Iterator type
         * @param position Iterator to the position
         * @param first Iterator to the first element
         * @param last Iterator to the last element
         * 
         * @complexity O(n)
         */
        template <class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type * = 0)
        {
            while (first != last)
            {
                position = insert(position, *first) + 1;
                ++first;
            }
        }

        /**
         * Erases an element at the given position.
         * 
         * @param position Iterator to the position
         * @return Iterator to the element following the erased element
         * 
         * @complexity O(n)
         */
        iterator erase(iterator position)
        {
            if (position + 1 != end())
            {
                ft::copy(position + 1, end(), position);
            }
            _alloc.destroy(&arr[--_size]);
            return position;
        }

        /**
         * Erases elements in the range [first, last).
         * 
         * @param first Iterator to the first element
         * @param last Iterator to the last element
         * @return Iterator to the element following the erased elements
         * 
         * @complexity O(n)
         */
        iterator erase(iterator first, iterator last)
        {
            if (first != last)
            {
                iterator new_end = ft::copy(last, end(), first);
                for (iterator it = new_end; it != end(); ++it)
                {
                    _alloc.destroy(&*it);
                }
                _size -= (last - first);
            }
            return first;
        }

        /**
         * Swaps the contents of this vector with another vector.
         * 
         * @param x The vector to swap with
         * 
         * @complexity O(1)
         */
        void swap(vector &x)
        {
            value_type *arr_tmp = x.arr;
            size_type size_tmp = x._size;
            size_type cap_tmp = x._cap;

            x.arr = arr;
            x._size = _size;
            x._cap = _cap;

            arr = arr_tmp;
            _size = size_tmp;
            _cap = cap_tmp;
        }

        /**
         * Clears the contents of the vector.
         * 
         * @complexity O(n)
         */
        void clear()
        {
            if (arr != NULL)
            {
                for (size_type i = 0; i < _size; i++)
                    _alloc.destroy(&arr[i]);
                _size = 0;
            }
        }
    };

    /**
     * Checks if the elements in two ranges are equal.
     * 
     * @tparam InputIterator1 Iterator type for the first range
     * @tparam InputIterator2 Iterator type for the second range
     * @param first1 Iterator to the first element in the first range
     * @param last1 Iterator to the last element in the first range
     * @param first2 Iterator to the first element in the second range
     * @return True if the elements are equal, false otherwise
     * 
     * @complexity O(n)
     */
    template <class InputIterator1, class InputIterator2>
    bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
    {
        while (first1 != last1)
        {
            if (!(*first1 == *first2))
                return false;
            ++first1;
            ++first2;
        }
        return true;
    }

    /**
     * Checks if two vectors are equal.
     * 
     * @tparam T Type of the elements
     * @tparam Alloc Allocator type
     * @param lhs Left-hand side vector
     * @param rhs Right-hand side vector
     * @return True if the vectors are equal, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        if (lhs.size() != rhs.size())
            return false;
        return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    /**
     * Checks if two vectors are not equal.
     * 
     * @tparam T Type of the elements
     * @tparam Alloc Allocator type
     * @param lhs Left-hand side vector
     * @param rhs Right-hand side vector
     * @return True if the vectors are not equal, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(lhs == rhs);
    }

    /**
     * Checks if one vector is less than another.
     * 
     * @tparam T Type of the elements
     * @tparam Alloc Allocator type
     * @param lhs Left-hand side vector
     * @param rhs Right-hand side vector
     * @return True if lhs is less than rhs, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    /**
     * Checks if one vector is less than or equal to another.
     * 
     * @tparam T Type of the elements
     * @tparam Alloc Allocator type
     * @param lhs Left-hand side vector
     * @param rhs Right-hand side vector
     * @return True if lhs is less than or equal to rhs, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(rhs < lhs);
    }

    /**
     * Checks if one vector is greater than another.
     * 
     * @tparam T Type of the elements
     * @tparam Alloc Allocator type
     * @param lhs Left-hand side vector
     * @param rhs Right-hand side vector
     * @return True if lhs is greater than rhs, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return rhs < lhs;
    }

    /**
     * Checks if one vector is greater than or equal to another.
     * 
     * @tparam T Type of the elements
     * @tparam Alloc Allocator type
     * @param lhs Left-hand side vector
     * @param rhs Right-hand side vector
     * @return True if lhs is greater than or equal to rhs, false otherwise
     * 
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(lhs < rhs);
    }

    /**
     * Swaps the contents of two vectors.
     * 
     * @tparam T Type of the elements
     * @tparam Alloc Allocator type
     * @param x First vector
     * @param y Second vector
     * 
     * @complexity O(1)
     */
    template <class T, class Alloc>
    void swap(vector<T, Alloc> &x, vector<T, Alloc> &y)
    {
        x.swap(y);
    }
};

#endif // !VECTOR_HPP
