#ifndef LIST_HPP
#define LIST_HPP

#include "utility.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"
#include "exception.hpp"
#include "type_traits.hpp"

namespace ft
{

    template <class T, class Alloc = std::allocator<T> >
    class list
    {
    public:
        typedef struct s_node
        {
            T data;
            struct s_node *prev;
            struct s_node *next;
        } node;

        template <bool IsConst>
        class listIterator
        {
        public:
            // Member types
            typedef typename ft::conditional<IsConst, const T, T>::type value_type;
            typedef typename ft::conditional<IsConst, const node, node>::type node_type;
            typedef value_type &reference;
            typedef value_type *pointer;
            typedef ft::ptrdiff_t difference_type;
            typedef ft::size_t size_type;
            typedef ft::bidirectional_iterator_tag iterator_category;

        private:
            node_type *_ptr;
        public:
        
            /**
             * Default constructor for listIterator
             *
             * @throws None
             * @complexity O(1)
             */
            listIterator(void) { _ptr = NULL; }

            /**
             * Constructor with parameters for listIterator
             *
             * @param ptr Pointer to the node
             *
             * @throws None
             * @complexity O(1)
             */
            listIterator(node_type *const ptr) { _ptr = ptr; }

            /**
             * Destructor for listIterator
             *
             * @throws None
             * @complexity O(1)
             */
            ~listIterator(void) {}

            /**
             * Copy constructor for listIterator
             *
             * @param x Another listIterator to copy from
             *
             * @throws None
             * @complexity O(1)
             */
            template <bool B>
            listIterator(const listIterator<B> &x, typename ft::enable_if<!B>::type * = 0) { _ptr = x.getPtr(); }

            /**
             * Assignment operator for listIterator
             *
             * @param x Another listIterator to assign from
             * @return Reference to the assigned listIterator
             *
             * @throws None
             * @complexity O(1)
             */
            listIterator &operator=(const listIterator &x)
            {
                _ptr = x.getPtr();
                return (*this);
            }

            /**
             * Equality comparison operator for listIterator
             *
             * @param x Another listIterator to compare with
             * @return True if equal, false otherwise
             *
             * @throws None
             * @complexity O(1)
             */
            template <bool B>
            bool operator==(const listIterator<B> &x) const { return (_ptr == x.getPtr()); }

            /**
             * Inequality comparison operator for listIterator
             *
             * @param x Another listIterator to compare with
             * @return True if not equal, false otherwise
             *
             * @throws None
             * @complexity O(1)
             */
            template <bool B>
            bool operator!=(const listIterator<B> &x) const { return (_ptr != x.getPtr()); }

            /**
             * Pre-increment operator for listIterator
             *
             * @return Reference to the incremented listIterator
             *
             * @throws None
             * @complexity O(1)
             */
            listIterator &operator++(void)
            {
                _ptr = _ptr->next;
                return (*this);
            }

            /**
             * Pre-decrement operator for listIterator
             *
             * @return Reference to the decremented listIterator
             *
             * @throws None
             * @complexity O(1)
             */
            listIterator &operator--(void)
            {
                _ptr = _ptr->prev;
                return (*this);
            }

            /**
             * Post-increment operator for listIterator
             *
             * @return Copy of the listIterator before increment
             *
             * @throws None
             * @complexity O(1)
             */
            listIterator operator++(int)
            {
                listIterator<IsConst> x(*this);
                _ptr = _ptr->next;
                return (x);
            }

            /**
             * Post-decrement operator for listIterator
             *
             * @return Copy of the listIterator before decrement
             *
             * @throws None
             * @complexity O(1)
             */
            listIterator operator--(int)
            {
                listIterator<IsConst> x(*this);
                _ptr = _ptr->prev;
                return (x);
            }

            /**
             * Dereference operator for listIterator
             *
             * @return Reference to the value pointed by the iterator
             *
             * @throws None
             * @complexity O(1)
             */
            value_type &operator*(void) const { return (_ptr->data); }

            /**
             * Member access operator for listIterator
             *
             * @return Pointer to the value pointed by the iterator
             *
             * @throws None
             * @complexity O(1)
             */
            value_type *operator->(void) const { return (&_ptr->data); }

            /**
             * Get the pointer to the node
             *
             * @return Pointer to the node
             *
             * @throws None
             * @complexity O(1)
             */
            node_type *getPtr(void) const { return (_ptr); }


        }; // Iterator

        typedef T value_type;
        typedef typename Alloc::template rebind<node>::other allocator_type;
        typedef typename Alloc::reference reference;
        typedef typename Alloc::const_reference const_reference;
        typedef typename Alloc::pointer pointer;
        typedef typename Alloc::const_pointer const_pointer;
        typedef listIterator<false> iterator;
        typedef listIterator<true> const_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef typename listIterator<false>::difference_type difference_type;
        typedef typename listIterator<false>::size_type size_type;

        private:
            allocator_type _alloc;
            node *_end;
        public:

        /**
         * Default constructor for list
         *
         * @param alloc Allocator to use
         *
         * @throws None
         * @complexity O(1)
         */
        explicit list(const allocator_type &alloc = allocator_type())
        {
            _alloc = alloc;
            this->_new_end_node();
        }

        /**
         * Fill constructor for list
         *
         * @param n Number of elements
         * @param val Value to fill the list with
         * @param alloc Allocator to use
         *
         * @throws None
         * @complexity O(n)
         */
        explicit list(size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type())
        {
            _alloc = alloc;
            this->_new_end_node();

            while (n-- > 0)
                this->push_back(val);
        }

        /**
         * Range constructor for list
         *
         * @param first Iterator to the beginning of the range
         * @param last Iterator to the end of the range
         * @param alloc Allocator to use
         *
         * @throws None
         * @complexity O(n)
         */
        template <class InputIterator>
        list(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type(),
             typename ft::enable_if<!ft::is_same<InputIterator, int>::value>::type * = 0)
        {
            _alloc = alloc;
            this->_new_end_node();

            while (first != last)
            {
                this->push_back(*first);
                ++first;
            }
        }

        /**
         * Copy constructor for list
         *
         * @param x Another list to copy from
         *
         * @throws None
         * @complexity O(n)
         */
        list(const list &x)
        {
            this->_new_end_node();
            *this = x;
        }

        /**
         * Destructor for list
         *
         * @throws None
         * @complexity O(n)
         */
        ~list(void)
        {
            this->clear();
            _alloc.destroy(_end);
            _alloc.deallocate(_end, 1);
        }

        /**
         * Assignment operator for list
         *
         * @param x Another list to assign from
         * @return Reference to the assigned list
         *
         * @throws None
         * @complexity O(n)
         */
        list &operator=(const list &x)
        {
            if (this == &x)
                return (*this);

            this->clear();
            _alloc = x._alloc;

            for (const_iterator it = x.begin(); it != x.end(); it++)
                this->push_back(*it);
            return (*this);
        }

        //////////////////////////////
        // Iterators
        //////////////////////////////

        /**
         * Get iterator to the beginning of the list
         *
         * @return Iterator to the beginning
         *
         * @throws None
         * @complexity O(1)
         */
        iterator begin(void)
        {
            return (iterator(_end->next));
        }

        /**
         * Get constant iterator to the beginning of the list
         *
         * @return Constant iterator to the beginning
         *
         * @throws None
         * @complexity O(1)
         */
        const_iterator begin(void) const
        {
            return (const_iterator(_end->next));
        }

        /**
         * Get iterator to the end of the list
         *
         * @return Iterator to the end
         *
         * @throws None
         * @complexity O(1)
         */
        iterator end(void)
        {
            return (iterator(_end));
        }

        /**
         * Get constant iterator to the end of the list
         *
         * @return Constant iterator to the end
         *
         * @throws None
         * @complexity O(1)
         */
        const_iterator end(void) const
        {
            return (const_iterator(_end));
        }

        //////////////////////////////
        // Reverse iterators
        //////////////////////////////

        /**
         * Get reverse iterator to the beginning of the reversed list
         *
         * @return Reverse iterator to the beginning
         *
         * @throws None
         * @complexity O(1)
         */
        reverse_iterator rbegin(void)
        {
            return (reverse_iterator(_end));
        }

        /**
         * Get constant reverse iterator to the beginning of the reversed list
         *
         * @return Constant reverse iterator to the beginning
         *
         * @throws None
         * @complexity O(1)
         */
        const_reverse_iterator rbegin(void) const
        {
            return (const_reverse_iterator(_end));
        }

        /**
         * Get reverse iterator to the end of the reversed list
         *
         * @return Reverse iterator to the end
         *
         * @throws None
         * @complexity O(1)
         */
        reverse_iterator rend(void)
        {
            return (reverse_iterator(_end->next));
        }

        /**
         * Get constant reverse iterator to the end of the reversed list
         *
         * @return Constant reverse iterator to the end
         *
         * @throws None
         * @complexity O(1)
         */
        const_reverse_iterator rend(void) const
        {
            return (const_reverse_iterator(_end->next));
        }

        //////////////////////////////
        // Capacity
        //////////////////////////////

        /**
         * Check if the list is empty
         *
         * @return True if the list is empty, false otherwise
         *
         * @throws None
         * @complexity O(1)
         */
        bool empty(void) const
        {
            return (_end == _end->next);
        }

        /**
         * Get the size of the list
         *
         * @return Number of elements in the list
         *
         * @throws None
         * @complexity O(n)
         */
        size_type size(void) const
        {
            size_type n = 0;
            for (node *curr = _end->next; curr != _end; curr = curr->next)
                n++;
            return (n);
        }

        /**
         * Resize the list to contain n elements
         *
         * @param n New size of the list
         * @param val Value to initialize the new elements with
         *
         * @throws None
         * @complexity O(n)
         */
        void resize(size_type n, value_type val = value_type())
        {
            while (this->size() < n)
                this->push_back(val);
            while (this->size() > n)
                this->erase(this->end().getPtr()->prev);
        }

        /**
         * Get the maximum size of the list
         *
         * @return Maximum number of elements the list can hold
         *
         * @throws None
         * @complexity O(1)
         */
        size_type max_size(void) const
        {
            return (_alloc.max_size());
        }

        //////////////////////////////
        // Member access
        //////////////////////////////

        /**
         * Access the first element of the list
         *
         * @return Reference to the first element
         *
         * @throws None
         * @complexity O(1)
         */
        reference front(void)
        {
            return (_end->next->data);
        }

        /**
         * Access the first element of the list (const)
         *
         * @return Constant reference to the first element
         *
         * @throws None
         * @complexity O(1)
         */
        const_reference front(void) const
        {
            return (_end->next->data);
        }

        /**
         * Access the last element of the list
         *
         * @return Reference to the last element
         *
         * @throws None
         * @complexity O(1)
         */
        reference back(void)
        {
            return (_end->prev->data);
        }

        /**
         * Access the last element of the list (const)
         *
         * @return Constant reference to the last element
         *
         * @throws None
         * @complexity O(1)
         */
        const_reference back(void) const
        {
            return (_end->prev->data);
        }

        //////////////////////////////
        // Assignment modifiers
        //////////////////////////////

        /**
         * Assign a range of elements to the list
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
            *this = list(first, last);
        }

        /**
         * Assign n elements to the list
         *
         * @param n Number of elements
         * @param val Value to assign
         *
         * @throws None
         * @complexity O(n)
         */
        void assign(size_type n, const value_type &val)
        {
            *this = list(n, val);
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
         * @complexity O(1)
         */
        iterator insert(iterator position, const value_type &val)
        {
            node *new_node = _alloc.allocate(1);
            this->_construct(new_node, position.getPtr()->prev, position.getPtr(), val);
            position.getPtr()->prev = new_node;
            new_node->prev->next = new_node;
            return (new_node);
        }

        /**
         * Insert n elements at a given position
         *
         * @param position Iterator to the position
         * @param n Number of elements to insert
         * @param val Value to insert
         *
         * @throws None
         * @complexity O(n)
         */
        void insert(iterator position, size_type n, const value_type &val)
        {
            while (n-- > 0)
                position = this->insert(position, val);
        }

        /**
         * Insert a range of elements at a given position
         *
         * @param position Iterator to the position
         * @param first Iterator to the beginning of the range
         * @param last Iterator to the end of the range
         *
         * @throws None
         * @complexity O(n)
         */
        template <class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last,
                    typename ft::enable_if<!ft::is_same<InputIterator, int>::value>::type * = 0)
        {
            while (first != last)
            {
                this->insert(position, *first);
                ++first;
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
         * @complexity O(1)
         */
        iterator erase(iterator position)
        {
            node *position_node = position.getPtr();
            position = position.getPtr()->next;
            position_node->next->prev = position_node->prev;
            position_node->prev->next = position_node->next;
            _alloc.destroy(position_node);
            _alloc.deallocate(position_node, 1);
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
            for (iterator it = first++; it != last; it = first++)
                this->erase(it);
            return (last);
        }

        //////////////////////////////
        // Common modifiers
        //////////////////////////////

        /**
         * Add an element to the front of the list
         *
         * @param val Value to add
         *
         * @throws None
         * @complexity O(1)
         */
        void push_front(const value_type &val)
        {
            this->insert(_end->next, val);
        }

        /**
         * Remove the first element from the list
         *
         * @throws None
         * @complexity O(1)
         */
        void pop_front(void)
        {
            this->erase(_end->next);
        }

        /**
         * Add an element to the end of the list
         *
         * @param val Value to add
         *
         * @throws None
         * @complexity O(1)
         */
        void push_back(const value_type &val)
        {
            this->insert(_end, val);
        }

        /**
         * Remove the last element from the list
         *
         * @throws None
         * @complexity O(1)
         */
        void pop_back(void)
        {
            this->erase(_end->prev);
        }

        /**
         * Swap the contents of two lists
         *
         * @param x Another list to swap with
         *
         * @throws None
         * @complexity O(1)
         */
        void swap(list &x)
        {
            ft::swap(_alloc, x._alloc);
            ft::swap(_end, x._end);
        }

        /**
         * Clear the contents of the list
         *
         * @throws None
         * @complexity O(n)
         */
        void clear(void)
        {
            for (node *prev = _end->next, *next = _end->next->next; prev != _end; prev = next, next = next->next)
            {
                _alloc.destroy(prev);
                _alloc.deallocate(prev, 1);
            }
            _end->prev = _end;
            _end->next = _end;
        }

        //////////////////////////////
        // Splicing operations
        //////////////////////////////

        /**
         * Transfer all elements from x to the position before the given iterator
         *
         * @param position Iterator before which to transfer elements
         * @param x List to transfer elements from
         *
         * @throws None
         * @complexity O(1)
         */
        void splice(iterator position, list &x)
        {
            if (!x.empty())
            {
                x._end->next->prev = position.getPtr()->prev;
                x._end->prev->next = position.getPtr();
                position.getPtr()->prev->next = x._end->next;
                position.getPtr()->prev = x._end->prev;
                x._end->next = x._end;
                x._end->prev = x._end;
            }
        }

        /**
         * Transfer the element pointed to by i from x to the position before the given iterator
         *
         * @param position Iterator before which to transfer element
         * @param x List to transfer element from
         * @param i Iterator to the element to transfer
         *
         * @throws None
         * @complexity O(1)
         */
        void splice(iterator position, list &x, iterator i)
        {
            if (!x.empty())
            {
                i.getPtr()->next->prev = i.getPtr()->prev;
                i.getPtr()->prev->next = i.getPtr()->next;
                i.getPtr()->prev = position.getPtr()->prev;
                i.getPtr()->next = position.getPtr();
                position.getPtr()->prev->next = i.getPtr();
                position.getPtr()->prev = i.getPtr();
            }
        }

        /**
         * Transfer the range [first, last) from x to the position before the given iterator
         *
         * @param position Iterator before which to transfer elements
         * @param x List to transfer elements from
         * @param first Iterator to the beginning of the range
         * @param last Iterator to the end of the range
         *
         * @throws None
         * @complexity O(1)
         */
        void splice(iterator position, list &x, iterator first, iterator last)
        {
            if (!x.empty() && first != last)
            {
                last.getPtr()->prev->next = position.getPtr();
                first.getPtr()->prev->next = last.getPtr();
                position.getPtr()->prev->next = first.getPtr();
                node *a = first.getPtr()->prev;
                node *b = position.getPtr()->prev;
                node *c = last.getPtr()->prev;
                last.getPtr()->prev = a;
                first.getPtr()->prev = b;
                position.getPtr()->prev = c;
            }
        }

        //////////////////////////////
        // Removal operations
        //////////////////////////////

        /**
         * Remove all elements equal to val
         *
         * @param val Value to remove
         *
         * @throws None
         * @complexity O(n)
         */
        void remove(const value_type &val)
        {
            for (iterator it = this->begin(), next = ++this->begin(); it != this->end(); it = next++)
            {
                if (*it == val)
                    this->erase(it);
            }
        }

        /**
         * Remove all elements satisfying the predicate pred
         *
         * @param pred Predicate to satisfy
         *
         * @throws None
         * @complexity O(n)
         */
        template <class Predicate>
        void remove_if(Predicate pred)
        {
            for (iterator it = this->begin(), next = ++this->begin(); it != this->end(); it = next++)
            {
                if (pred(*it))
                    this->erase(it);
            }
        }

        //////////////////////////////
        // Uniquify operations
        //////////////////////////////

        /**
         * Remove consecutive duplicate elements
         *
         * @throws None
         * @complexity O(n)
         */
        void unique(void)
        {
            for (iterator it = ++this->begin(); it != this->end(); it++)
            {
                if (*it == *iterator(it.getPtr()->prev))
                {
                    this->erase(it);
                    it = this->begin();
                }
            }
        }

        /**
         * Remove consecutive duplicate elements satisfying the binary predicate
         *
         * @param binary_pred Binary predicate to satisfy
         *
         * @throws None
         * @complexity O(n)
         */
        template <class BinaryPredicate>
        void unique(BinaryPredicate binary_pred)
        {
            for (iterator it = ++this->begin(); it != this->end(); it++)
            {
                if (binary_pred(it.getPtr()->prev->data, *it))
                {
                    this->erase(it);
                    it = this->begin();
                }
            }
        }

        //////////////////////////////
        // Merging operations
        //////////////////////////////

        /**
         * Merge another list into this list
         *
         * @param x List to merge from
         *
         * @throws None
         * @complexity O(n log n)
         */
        void merge(list &x)
        {
            if (this == &x)
                return;
            for (iterator it = this->begin(); it != this->end();)
            {
                iterator it2 = x.begin();
                if (it2 != x.end() && *it2 < *it)
                    this->splice(it, x, it2);
                else
                    it++;
            }
            this->splice(this->end(), x, x.begin(), x.end());
        }

        /**
         * Merge another list into this list using a comparison function
         *
         * @param x List to merge from
         * @param comp Comparison function to use
         *
         * @throws None
         * @complexity O(n log n)
         */
        template <class Compare>
        void merge(list &x, Compare comp)
        {
            if (this == &x)
                return;
            for (iterator it = this->begin(); it != this->end();)
            {
                iterator it2 = x.begin();
                if (it2 != x.end() && comp(*it2, *it))
                    this->splice(it, x, it2);
                else
                    it++;
            }
            this->splice(this->end(), x, x.begin(), x.end());
        }

        //////////////////////////////
        // Sorting operations
        //////////////////////////////

        /**
         * Sort the list in ascending order
         *
         * @throws None
         * @complexity O(n log n)
         */
        void sort(void)
        {
            this->_quicksort(this->begin(), this->end());
        }

        /**
         * Sort the list using a comparison function
         *
         * @param comp Comparison function to use
         *
         * @throws None
         * @complexity O(n log n)
         */
        template <class Compare>
        void sort(Compare comp)
        {
            this->_quicksort(comp, this->begin(), this->end());
        }

        /**
         * Reverse the order of elements in the list
         *
         * @throws None
         * @complexity O(n)
         */
        void reverse(void)
        {
            for (iterator it = this->begin(); it != this->end(); it--)
                ft::swap(it.getPtr()->prev, it.getPtr()->next);
            ft::swap(_end->prev, _end->next);
        }

        //////////////////////////////
        // Allocator
        //////////////////////////////

        /**
         * Get the allocator used by the list
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
         * Create a new end node
         *
         * @throws None
         * @complexity O(1)
         */
        void _new_end_node(void)
        {
            _end = _alloc.allocate(1);
            this->_construct(_end, _end, _end);
        }

        /**
         * Construct a new node
         *
         * @param ptr Pointer to the node to construct
         * @param prev Pointer to the previous node
         * @param next Pointer to the next node
         * @param data Value to initialize the node with
         *
         * @throws None
         * @complexity O(1)
         */
        void _construct(node *ptr, node *prev, node *next, value_type data = value_type())
        {
            node tmp;
            tmp.data = data;
            tmp.prev = prev;
            tmp.next = next;
            _alloc.construct(ptr, tmp);
        }

        /**
         * Swap two nodes
         *
         * @param first Iterator to the first node
         * @param second Iterator to the second node
         *
         * @throws None
         * @complexity O(1)
         */
        void _swap_nodes(iterator &first, iterator &second)
        {
            node *first_prev = first.getPtr()->prev;
            node *first_next = first.getPtr()->next;
            node *second_prev = second.getPtr()->prev;
            node *second_next = second.getPtr()->next;
            ft::swap(first_prev->next, second_prev->next);
            ft::swap(first_next->prev, second_next->prev);
            ft::swap(first.getPtr()->prev, second.getPtr()->prev);
            ft::swap(first.getPtr()->next, second.getPtr()->next);
            ft::swap(first, second);
        }

        //////////////////////////////
        // Quicksort
        //////////////////////////////

        /**
         * Quicksort algorithm for sorting the list
         *
         * @param first Iterator to the beginning of the range
         * @param last Iterator to the end of the range
         *
         * @throws None
         * @complexity O(n log n)
         */
        void _quicksort(iterator first, iterator last)
        {
            if (first != last && iterator(first.getPtr()->next) != last)
            {
                iterator it = this->_partition(first, last);
                this->_quicksort(first, it++);
                this->_quicksort(it, last);
            }
        }

        /**
         * Partition function for quicksort
         *
         * @param first Iterator to the beginning of the range
         * @param last Iterator to the end of the range
         * @return Iterator to the partition point
         *
         * @throws None
         * @complexity O(n)
         */
        iterator _partition(iterator &first, iterator &last)
        {
            iterator prev = first;
            last--;
            for (iterator it = first; it != last; it++)
            {
                if (*it < *last)
                {
                    this->_swap_nodes(prev, it);
                    if (first == it)
                        first = prev;
                    prev++;
                }
            }
            this->_swap_nodes(prev, last);
            if (first == last)
                first = prev;
            last++;
            return (prev);
        }

        //////////////////////////////
        // Quicksort (with compare function)
        //////////////////////////////

        /**
         * Quicksort algorithm for sorting the list using a comparison function
         *
         * @param comp Comparison function to use
         * @param first Iterator to the beginning of the range
         * @param last Iterator to the end of the range
         *
         * @throws None
         * @complexity O(n log n)
         */
        template <class Compare>
        void _quicksort(Compare comp, iterator first, iterator last)
        {
            if (first != last && iterator(first.getPtr()->next) != last)
            {
                iterator it = this->_partition(comp, first, last);
                this->_quicksort(comp, first, it++);
                this->_quicksort(comp, it, last);
            }
        }

        /**
         * Partition function for quicksort with compare function
         *
         * @param comp Comparison function to use
         * @param first Iterator to the beginning of the range
         * @param last Iterator to the end of the range
         * @return Iterator to the partition point
         *
         * @throws None
         * @complexity O(n)
         */
        template <class Compare>
        iterator _partition(Compare comp, iterator &first, iterator &last)
        {
            iterator prev = first;
            last--;
            for (iterator it = first; it != last; it++)
            {
                if (comp(*it, *last))
                {
                    this->_swap_nodes(prev, it);
                    if (first == it)
                        first = prev;
                    prev++;
                }
            }
            this->_swap_nodes(prev, last);
            if (first == last)
                first = prev;
            last++;
            return (prev);
        }

    }; // List

    //////////////////////////////
    // Relational operators
    //////////////////////////////

    /**
     * Equality comparison operator for list
     *
     * @param lhs Left-hand side list
     * @param rhs Right-hand side list
     * @return True if equal, false otherwise
     *
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator==(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
    {
        return (ft::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
    }

    /**
     * Less than comparison operator for list
     *
     * @param lhs Left-hand side list
     * @param rhs Right-hand side list
     * @return True if lhs is less than rhs, false otherwise
     *
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator<(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
    {
        return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
    }

    /**
     * Inequality comparison operator for list
     *
     * @param lhs Left-hand side list
     * @param rhs Right-hand side list
     * @return True if not equal, false otherwise
     *
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator!=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
    {
        return (!(lhs == rhs));
    }

    /**
     * Less than or equal comparison operator for list
     *
     * @param lhs Left-hand side list
     * @param rhs Right-hand side list
     * @return True if lhs is less than or equal to rhs, false otherwise
     *
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator<=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
    {
        return (!(rhs < lhs));
    }

    /**
     * Greater than comparison operator for list
     *
     * @param lhs Left-hand side list
     * @param rhs Right-hand side list
     * @return True if lhs is greater than rhs, false otherwise
     *
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator>(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
    {
        return (rhs < lhs);
    }

    /**
     * Greater than or equal comparison operator for list
     *
     * @param lhs Left-hand side list
     * @param rhs Right-hand side list
     * @return True if lhs is greater than or equal to rhs, false otherwise
     *
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Alloc>
    bool operator>=(const list<T, Alloc> &lhs, const list<T, Alloc> &rhs)
    {
        return (!(lhs < rhs));
    }

    /**
     * Swap the contents of two lists
     *
     * @param x First list
     * @param y Second list
     *
     * @throws None
     * @complexity O(1)
     */
    template <class T, class Alloc>
    void swap(list<T, Alloc> &x, list<T, Alloc> &y)
    {
        x.swap(y);
    }

} // Namespace ft

#endif