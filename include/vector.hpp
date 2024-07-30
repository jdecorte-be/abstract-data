#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "utility.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"
#include "exception.hpp"
#include "type_traits.hpp"
#include "string.hpp"

namespace ft
{
    template <class T, class Allocator = std::allocator<T> >
    class vector
    {
    public:
        typedef Allocator allocator_type;
        typedef std::allocator_traits<allocator_type> alloc_traits;
        typedef typename alloc_traits::size_type size_type;
        typedef T value_type;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef typename alloc_traits::difference_type difference_type;
        typedef typename alloc_traits::pointer pointer;
        typedef typename alloc_traits::const_pointer const_pointer;
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

            vectorIterator(void) { _ptr = NULL; }
            ~vectorIterator(void) {}

            template <bool B>
            vectorIterator(const vectorIterator<B> &x, typename ft::enable_if<!B>::type * = 0) { _ptr = x.getPtr(); }

            vectorIterator &operator=(const vectorIterator &x)
            {
                _ptr = x.getPtr();
                return (*this);
            }
            vectorIterator &operator+=(int n)
            {
                _ptr += n;
                return (*this);
            }
            vectorIterator &operator-=(int n)
            {
                _ptr -= n;
                return (*this);
            }

            template <bool B>
            bool operator==(const vectorIterator<B> &x) const { return (_ptr == x.getPtr()); }
            template <bool B>
            bool operator!=(const vectorIterator<B> &x) const { return (_ptr != x.getPtr()); }
            template <bool B>
            bool operator<(const vectorIterator<B> &x) const { return (_ptr < x.getPtr()); }
            template <bool B>
            bool operator>(const vectorIterator<B> &x) const { return (_ptr > x.getPtr()); }
            template <bool B>
            bool operator<=(const vectorIterator<B> &x) const { return (_ptr <= x.getPtr()); }
            template <bool B>
            bool operator>=(const vectorIterator<B> &x) const { return (_ptr >= x.getPtr()); }

            vectorIterator &operator++(void)
            {
                _ptr++;
                return (*this);
            }
            vectorIterator &operator--(void)
            {
                _ptr--;
                return (*this);
            }
            vectorIterator operator++(int)
            {
                vectorIterator<IsConst> x(*this);
                _ptr++;
                return (x);
            }
            vectorIterator operator--(int)
            {
                vectorIterator<IsConst> x(*this);
                _ptr--;
                return (x);
            }

            vectorIterator operator+(int n) const { return (_ptr + n); }
            vectorIterator operator-(int n) const { return (_ptr - n); }
            difference_type operator-(const vectorIterator &x) const { return (_ptr - x.getPtr()); }

            value_type &operator[](size_type n) const { return (*(_ptr + n)); }
            value_type &operator*(void) const { return (*_ptr); }
            value_type *operator->(void) const { return (_ptr); }

            value_type *getPtr(void) const { return (_ptr); }

            friend vectorIterator operator+(int n, const vectorIterator &x) { return (x.getPtr() + n); }

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
        explicit vector(const allocator_type &alloc = allocator_type())
            : _size(0), _cap(0), _alloc(alloc)
        {
            arr = _alloc.allocate(0);
        }

        explicit vector(size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type())
            : arr(NULL), _size(0), _cap(0), _alloc(alloc)
        {
            resize(n, val);
        }

        template <typename InputIterator>
        vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type(), typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type * = 0)
            : arr(NULL), _size(0), _cap(0), _alloc(alloc)
        {
            assign(first, last);
        }

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

        ~vector()
        {
            clear();
            if (_cap)
                _alloc.deallocate(arr, _cap);
        }

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

        iterator begin()
        {
            return arr;
        }

        const_iterator begin() const
        {
            return arr;
        }

        iterator end()
        {
            return arr + _size;
        }

        const_iterator end() const
        {
            return arr + _size;
        }

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

        bool empty() const
        {
            return _size == 0;
        }

        size_type size() const
        {
            return _size;
        }

        size_type max_size() const
        {
            return _alloc.max_size();
        }

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

        size_type capacity() const
        {
            return _cap;
        }

        void reserve(size_type n)
        {
            if (n > this->_alloc.max_size())
                throw std::length_error("vector");
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

        reference operator[](size_type n)
        {
            return arr[n];
        }

        const_reference operator[](size_type n) const
        {
            return arr[n];
        }

        reference at(size_type n)
        {
            if (n >= _size)
                throw ft::out_of_range("vector");
            return arr[n];
        }

        const_reference at(size_type n) const
        {
            if (n >= _size)
                throw ft::out_of_range("vector");
            return arr[n];
        }

        reference front()
        {
            return arr[0];
        }

        const_reference front() const
        {
            return arr[0];
        }

        reference back()
        {
            return arr[_size - 1];
        }

        const_reference back() const
        {
            return arr[_size - 1];
        }

        template <class InputIterator>
        void assign(InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type * = 0)
        {
            clear();
            for (InputIterator i = first; i != last; ++i)
                push_back(*i);
        }

        void assign(size_type n, const value_type &val)
        {
            clear();
            while (n--)
                push_back(val);
        }

        void push_back(const value_type &val)
        {
            if (_size + 1 > _cap)
                reserve(_cap == 0 ? 1 : _cap * 2);
            _alloc.construct(&arr[_size++], val);
        }

        void pop_back()
        {
            _alloc.destroy(&arr[_size - 1]);
            _size--;
        }

        iterator insert(iterator position, const value_type &val)
        {
            size_type off = position - this->begin();
            this->insert(position, 1, val);
            return (iterator(arr + off));
        }

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

        template <class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type * = 0)
        {
            while (first != last)
            {
                position = insert(position, *first) + 1;
                ++first;
            }
        }

        iterator erase(iterator position)
        {
            if (position + 1 != end())
            {
                std::copy(position + 1, end(), position);
            }
            _alloc.destroy(&arr[--_size]);
            return position;
        }

        iterator erase(iterator first, iterator last)
        {
            if (first != last)
            {
                iterator new_end = std::copy(last, end(), first);
                for (iterator it = new_end; it != end(); ++it)
                {
                    _alloc.destroy(&*it);
                }
                _size -= (last - first);
            }
            return first;
        }

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

    template <class T, class Alloc>
    bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        if (lhs.size() != rhs.size())
            return false;
        return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
    }

    template <class T, class Alloc>
    bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(lhs == rhs);
    }

    template <class T, class Alloc>
    bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class T, class Alloc>
    bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(rhs < lhs);
    }

    template <class T, class Alloc>
    bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return rhs < lhs;
    }

    template <class T, class Alloc>
    bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
    {
        return !(lhs < rhs);
    }

    template <class T, class Alloc>
    void swap(vector<T, Alloc> &x, vector<T, Alloc> &y)
    {
        x.swap(y);
    }
};

#endif // !VECTOR_HPP
