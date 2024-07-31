#ifndef FT_PRIORITY_QUEUE_HPP
#define FT_PRIORITY_QUEUE_HPP

#include "vector.hpp"
#include "algorithm.hpp"

namespace ft
{
    template <class T, class Container = ft::vector<T>, class Compare = ft::less<typename Container::value_type> >
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
        /**
         * @brief Constructs an empty priority queue.
         * 
         * @param compare The comparator to use.
         * @param cont The container to use.
         * 
         * @complexity O(n log n), where n is the number of elements in the container.
         */
        explicit priority_queue(const Compare &compare = Compare(),
                                const Container &cont = Container())
            : c(cont), comp(compare)
        {
            ft::make_heap(c.begin(), c.end(), comp);
        }

        /**
         * @brief Constructs a priority queue with elements from the range [first, last).
         * 
         * @tparam InputIterator Iterator type for the range.
         * @param first Iterator to the beginning of the range.
         * @param last Iterator to the end of the range.
         * @param compare The comparator to use.
         * @param cont The container to use.
         * 
         * @complexity O(n log n), where n is the number of elements in the range.
         */
        template <class InputIterator>
        priority_queue(InputIterator first, InputIterator last,
                       const Compare &compare = Compare(),
                       const Container &cont = Container())
            : c(cont), comp(compare)
        {
            c.insert(c.end(), first, last);
            ft::make_heap(c.begin(), c.end(), comp);
        }

        /**
         * @brief Checks if the priority queue is empty.
         * 
         * @return true if the priority queue is empty, false otherwise.
         * 
         * @complexity O(1)
         */
        bool empty() const { return c.empty(); }

        /**
         * @brief Returns the number of elements in the priority queue.
         * 
         * @return The number of elements.
         * 
         * @complexity O(1)
         */
        size_type size() const { return c.size(); }

        /**
         * @brief Returns a const reference to the top element in the priority queue.
         * 
         * @return A const reference to the top element.
         * 
         * @complexity O(1)
         */
        const value_type &top() const { return c.front(); }

        /**
         * @brief Inserts a new element into the priority queue.
         * 
         * @param val The value of the element to insert.
         * 
         * @complexity O(log n), where n is the number of elements in the priority queue.
         */
        void push(const value_type &val)
        {
            c.push_back(val);
            ft::push_heap(c.begin(), c.end(), comp);
        }

        /**
         * @brief Removes the top element from the priority queue.
         * 
         * @complexity O(log n), where n is the number of elements in the priority queue.
         */
        void pop()
        {
            ft::pop_heap(c.begin(), c.end(), comp);
            c.pop_back();
        }

        /**
         * @brief Checks if two priority queues are equal.
         * 
         * @param lhs The left-hand side priority queue.
         * @param rhs The right-hand side priority queue.
         * @return true if the two priority queues are equal, false otherwise.
         * 
         * @complexity O(n), where n is the number of elements in the priority queue.
         */
        friend bool operator==(const priority_queue &lhs, const priority_queue &rhs)
        {
            return lhs.c == rhs.c;
        }

        /**
         * @brief Checks if two priority queues are not equal.
         * 
         * @param lhs The left-hand side priority queue.
         * @param rhs The right-hand side priority queue.
         * @return true if the two priority queues are not equal, false otherwise.
         * 
         * @complexity O(n), where n is the number of elements in the priority queue.
         */
        friend bool operator!=(const priority_queue &lhs, const priority_queue &rhs)
        {
            return !(lhs == rhs);
        }

        /**
         * @brief Checks if one priority queue is less than another.
         * 
         * @param lhs The left-hand side priority queue.
         * @param rhs The right-hand side priority queue.
         * @return true if lhs is less than rhs, false otherwise.
         * 
         * @complexity O(n), where n is the number of elements in the priority queue.
         */
        friend bool operator<(const priority_queue &lhs, const priority_queue &rhs)
        {
            return lhs.c < rhs.c;
        }

        /**
         * @brief Checks if one priority queue is less than or equal to another.
         * 
         * @param lhs The left-hand side priority queue.
         * @param rhs The right-hand side priority queue.
         * @return true if lhs is less than or equal to rhs, false otherwise.
         * 
         * @complexity O(n), where n is the number of elements in the priority queue.
         */
        friend bool operator<=(const priority_queue &lhs, const priority_queue &rhs)
        {
            return !(rhs < lhs);
        }

        /**
         * @brief Checks if one priority queue is greater than another.
         * 
         * @param lhs The left-hand side priority queue.
         * @param rhs The right-hand side priority queue.
         * @return true if lhs is greater than rhs, false otherwise.
         * 
         * @complexity O(n), where n is the number of elements in the priority queue.
         */
        friend bool operator>(const priority_queue &lhs, const priority_queue &rhs)
        {
            return rhs < lhs;
        }

        /**
         * @brief Checks if one priority queue is greater than or equal to another.
         * 
         * @param lhs The left-hand side priority queue.
         * @param rhs The right-hand side priority queue.
         * @return true if lhs is greater than or equal to rhs, false otherwise.
         * 
         * @complexity O(n), where n is the number of elements in the priority queue.
         */
        friend bool operator>=(const priority_queue &lhs, const priority_queue &rhs)
        {
            return !(lhs < rhs);
        }
    };

} // namespace ft

#endif