#ifndef FT_PRIORITY_QUEUE_HPP
#define FT_PRIORITY_QUEUE_HPP

#include "vector.hpp"
#include "algorithm.hpp"

namespace ft
{

    template <
        class T,
        class Container = ft::vector<T>,
        class Compare = ft::less<typename Container::value_type> >
    class priority_queue
    {
    public:
        typedef T value_type;
        typedef Container container_type;
        typedef typename Container::size_type size_type;
        typedef Compare value_compare;

    protected:
        Container c;
        Compare comp;

    public:
        // Constructors
        explicit priority_queue(const Compare &compare = Compare(),
                                const Container &cont = Container())
            : c(cont), comp(compare)
        {
            ft::make_heap(c.begin(), c.end(), comp);
        }

        template <class InputIterator>
        priority_queue(InputIterator first, InputIterator last,
                       const Compare &compare = Compare(),
                       const Container &cont = Container())
            : c(cont), comp(compare)
        {
            c.insert(c.end(), first, last);
            ft::make_heap(c.begin(), c.end(), comp);
        }

        // Capacity
        bool empty() const { return c.empty(); }
        size_type size() const { return c.size(); }

        // Element access
        const value_type &top() const { return c.front(); }

        // Modifiers
        void push(const value_type &val)
        {
            c.push_back(val);
            ft::push_heap(c.begin(), c.end(), comp);
        }

        void pop()
        {
            ft::pop_heap(c.begin(), c.end(), comp);
            c.pop_back();
        }

        // Non-member function overloads
        friend bool operator==(const priority_queue &lhs, const priority_queue &rhs)
        {
            return lhs.c == rhs.c;
        }

        friend bool operator!=(const priority_queue &lhs, const priority_queue &rhs)
        {
            return !(lhs == rhs);
        }

        friend bool operator<(const priority_queue &lhs, const priority_queue &rhs)
        {
            return lhs.c < rhs.c;
        }

        friend bool operator<=(const priority_queue &lhs, const priority_queue &rhs)
        {
            return !(rhs < lhs);
        }

        friend bool operator>(const priority_queue &lhs, const priority_queue &rhs)
        {
            return rhs < lhs;
        }

        friend bool operator>=(const priority_queue &lhs, const priority_queue &rhs)
        {
            return !(lhs < rhs);
        }
    };

} // namespace ft

#endif // FT_PRIORITY_QUEUE_HPP