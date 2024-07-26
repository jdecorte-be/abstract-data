#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

namespace ft
{

    // lexico compare
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

    template <class InputIterator1, class InputIterator2>
    bool equal(InputIterator1 lit, InputIterator1 lend, InputIterator2 rit, InputIterator2 rend)
    {
        while (lit != lend)
        {
            if (rit == rend || *rit < *lit || *lit < *rit)
                return (false);
            ++lit, ++rit;
        }

        return (rit == rend);
    }

    template <class ForwardIterator1, class ForwardIterator2>
    void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
    {
        typename ft::iterator_traits<ForwardIterator1>::value_type temp = *a;
        *a = *b;
        *b = temp;
    }

    // make_heap
    template <class RandomAccessIterator, class Compare>
    void heapify(RandomAccessIterator first, RandomAccessIterator last,
                 RandomAccessIterator root, Compare comp)
    {
        typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;

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

    template <class RandomAccessIterator, class Compare>
    void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
    {
        typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;

        difference_type size = last - first;
        for (difference_type i = size / 2 - 1; i >= 0; --i)
        {
            heapify(first, last, first + i, comp);
        }
    }

    // Overload for default comparison
    template <class RandomAccessIterator>
    void make_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
        make_heap(first, last, std::less<value_type>());
    }

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

    template <class RandomAccessIterator>
    void push_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
        push_heap(first, last, std::less<value_type>());
    }

    // pop_heap
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

    // Overload for default comparison
    template <class RandomAccessIterator>
    void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
    {
        typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
        pop_heap(first, last, std::less<value_type>());
    }

};

#endif