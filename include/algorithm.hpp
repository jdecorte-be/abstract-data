#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

namespace ft
{

    /**
     * Compares two ranges lexicographically
     * 
     * @param first1 Iterator to the beginning of the first range
     * @param last1 Iterator to the end of the first range
     * @param first2 Iterator to the beginning of the second range
     * @param last2 Iterator to the end of the second range
     * @return True if the first range is lexicographically less than the second range, otherwise false
     * 
     * @throws None
     * @complexity O(n), where n is the number of elements in the shorter range
     */
    template <class InputIterator1, class InputIterator2>
    bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
    {
        while (first1 != last1)
        {
            if (first2 == last2 || *first2 < *first1)
                return false;
            else if (*first1 < *first2)
                return true;
            ++first1;
            ++first2;
        }
        return (first2 != last2);
    }

    /**
     * Compares two ranges for equality
     * 
     * @param lit Iterator to the beginning of the first range
     * @param lend Iterator to the end of the first range
     * @param rit Iterator to the beginning of the second range
     * @param rend Iterator to the end of the second range
     * @return True if the ranges are equal, otherwise false
     * 
     * @throws None
     * @complexity O(n), where n is the number of elements in the range
     */
    template <class InputIterator1, class InputIterator2>
    bool equal(InputIterator1 lit, InputIterator1 lend, InputIterator2 rit, InputIterator2 rend)
    {
        while (lit != lend)
        {
            if (rit == rend || *rit < *lit || *lit < *rit)
                return (false);
            ++lit;
            ++rit;
        }

        return (rit == rend);
    }

    /**
     * Swaps the values pointed to by two iterators
     * 
     * @param a Iterator to the first value
     * @param b Iterator to the second value
     * 
     * @throws None
     * @complexity O(1)
     */
    template <class ForwardIterator1, class ForwardIterator2>
    void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
    {
        typename ft::iterator_traits<ForwardIterator1>::value_type temp = *a;
        *a = *b;
        *b = temp;
    }

    /**
     * Ensures the subtree rooted at the given root is a heap
     * 
     * @param first Iterator to the beginning of the range
     * @param last Iterator to the end of the range
     * @param root Iterator to the root of the subtree
     * @param comp Comparison function
     * 
     * @throws None
     * @complexity O(log n)
     */
    template <class RandomAccessIterator, class Compare>
    void heapify(RandomAccessIterator first, RandomAccessIterator last,
                 RandomAccessIterator root, Compare comp)
    {
        typedef typename ft::iterator_traits<RandomAccessIterator>::difference_type difference_type;

        difference_type size = last - first;
        difference_type largest = root - first;
        difference_type left = 2 * largest + 1;
        difference_type right = 2 * largest + 2;

        if (left < size && comp(*(first + largest), *(first + left)))
            largest = left;

        if (right < size && comp(*(first + largest), *(first + right)))
            largest = right;

        if (largest != root - first)
        {
            ft::iter_swap(first + largest, root);
            heapify(first, last, first + largest, comp);
        }
    }

    /**
     * Converts the range into a heap
     * 
     * @param first Iterator to the beginning of the range
     * @param last Iterator to the end of the range
     * @param comp Comparison function
     * 
     * @throws None
     * @complexity O(n log n)
     */
    template <class RandomAccessIterator, class Compare>
    void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
    {
        typedef typename ft::iterator_traits<RandomAccessIterator>::difference_type difference_type;

        difference_type size = last - first;
        for (difference_type i = size / 2 - 1; i >= 0; --i)
            heapify(first, last, first + i, comp);
    }

    /**
     * Converts the range into a heap using default comparison
     * 
     * @param first Iterator to the beginning of the range
     * @param last Iterator to the end of the range
     * 
     * @throws None
     * @complexity O(n log n)
     */
    template <class RandomAccessIterator>
    void make_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        typedef typename ft::iterator_traits<RandomAccessIterator>::value_type value_type;
        make_heap(first, last, ft::less<value_type>());
    }

    /**
     * Pushes the last element into its correct position in the heap
     * 
     * @param first Iterator to the beginning of the range
     * @param last Iterator to the end of the range
     * @param comp Comparison function
     * 
     * @throws None
     * @complexity O(log n)
     */
    template <class RandomAccessIterator, class Compare>
    void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
    {
        typedef typename ft::iterator_traits<RandomAccessIterator>::difference_type difference_type;
        typedef typename ft::iterator_traits<RandomAccessIterator>::value_type value_type;

        if (last - first > 1)
        {
            difference_type child = last - first - 1;
            difference_type parent = (child - 1) / 2;

            value_type value = *(first + child);

            while (child > 0 && comp(*(first + parent), value))
            {
                *(first + child) = *(first + parent);
                child = parent;
                parent = (child - 1) / 2;
            }

            *(first + child) = value;
        }
    }

    /**
     * Pushes the last element into its correct position in the heap using default comparison
     * 
     * @param first Iterator to the beginning of the range
     * @param last Iterator to the end of the range
     * 
     * @throws None
     * @complexity O(log n)
     */
    template <class RandomAccessIterator>
    void push_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        typedef typename ft::iterator_traits<RandomAccessIterator>::value_type value_type;
        push_heap(first, last, ft::less<value_type>());
    }

    /**
     * Moves the first element to its correct position and restores the heap
     * 
     * @param first Iterator to the beginning of the range
     * @param last Iterator to the end of the range
     * @param comp Comparison function
     * 
     * @throws None
     * @complexity O(log n)
     */
    template <class RandomAccessIterator, class Compare>
    void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
    {
        if (last - first > 1)
        {
            --last;
            ft::iter_swap(first, last);
            ft::heapify(first, last, first, comp);
        }
    }

    /**
     * Moves the first element to its correct position and restores the heap using default comparison
     * 
     * @param first Iterator to the beginning of the range
     * @param last Iterator to the end of the range
     * 
     * @throws None
     * @complexity O(log n)
     */
    template <class RandomAccessIterator>
    void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        typedef typename ft::iterator_traits<RandomAccessIterator>::value_type value_type;
        pop_heap(first, last, ft::less<value_type>());
    }

    template<class InputIterator, class OutputIterator>
    OutputIterator copy (InputIterator first, InputIterator last, OutputIterator result)
    {
        while (first!=last)
        {
            *result = *first;
            ++result; ++first;
        }
        return result;
    }


};

#endif
