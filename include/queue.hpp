#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "deque.hpp"
#include "priority_queue.hpp"

namespace ft
{

    template <class T, class Container = deque<T> >
    class queue
    {
    public:
        typedef T value_type;
        typedef Container container_type;
        typedef size_t size_type;

        /**
         * Constructor to initialize the queue with an optional container
         * 
         * @param ctnr Container object to initialize the queue
         * 
         * @throws None
         * @complexity O(1)
         */
        explicit queue(const container_type &ctnr = container_type())
        {
            c = ctnr;
        }

        /**
         * Returns the number of elements in the queue
         * 
         * @return size_type Number of elements in the queue
         * 
         * @throws None
         * @complexity O(1)
         */
        size_type size(void) const
        {
            return (c.size());
        }

        /**
         * Checks if the queue is empty
         * 
         * @return bool True if the queue is empty, false otherwise
         * 
         * @throws None
         * @complexity O(1)
         */
        bool empty(void) const
        {
            return (c.empty());
        }

        /**
         * Accesses the front element of the queue
         * 
         * @return value_type& Reference to the front element
         * 
         * @throws None
         * @complexity O(1)
         */
        value_type &front(void)
        {
            return (c.front());
        }

        /**
         * Accesses the front element of the queue (const version)
         * 
         * @return const value_type& Const reference to the front element
         * 
         * @throws None
         * @complexity O(1)
         */
        const value_type &front(void) const
        {
            return (c.front());
        }

        /**
         * Accesses the back element of the queue
         * 
         * @return value_type& Reference to the back element
         * 
         * @throws None
         * @complexity O(1)
         */
        value_type &back(void)
        {
            return (c.back());
        }

        /**
         * Accesses the back element of the queue (const version)
         * 
         * @return const value_type& Const reference to the back element
         * 
         * @throws None
         * @complexity O(1)
         */
        const value_type &back(void) const
        {
            return (c.back());
        }

        /**
         * Adds an element to the back of the queue
         * 
         * @param val The value to be added to the queue
         * 
         * @throws None
         * @complexity O(1)
         */
        void push(const value_type &val)
        {
            c.push_back(val);
        }

        /**
         * Removes the front element of the queue
         * 
         * @throws None
         * @complexity O(1)
         */
        void pop(void)
        {
            c.pop_front();
        }

    protected:
        container_type c;

        /**
         * Checks if two queues are equal
         * 
         * @param lhs Left-hand side queue
         * @param rhs Right-hand side queue
         * @return bool True if the queues are equal, false otherwise
         * 
         * @throws None
         * @complexity O(n)
         */
        friend bool operator==(const queue &lhs, const queue &rhs)
        {
            return (lhs.c == rhs.c);
        }

        /**
         * Checks if one queue is less than another
         * 
         * @param lhs Left-hand side queue
         * @param rhs Right-hand side queue
         * @return bool True if lhs is less than rhs, false otherwise
         * 
         * @throws None
         * @complexity O(n)
         */
        friend bool operator<(const queue &lhs, const queue &rhs)
        {
            return (lhs.c < rhs.c);
        }
    };

    /**
     * Checks if two queues are not equal
     * 
     * @param lhs Left-hand side queue
     * @param rhs Right-hand side queue
     * @return bool True if the queues are not equal, false otherwise
     * 
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Container>
    bool operator!=(const queue<T, Container> &lhs, const queue<T, Container> &rhs)
    {
        return (!(lhs == rhs));
    }

    /**
     * Checks if one queue is less than or equal to another
     * 
     * @param lhs Left-hand side queue
     * @param rhs Right-hand side queue
     * @return bool True if lhs is less than or equal to rhs, false otherwise
     * 
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Container>
    bool operator<=(const queue<T, Container> &lhs, const queue<T, Container> &rhs)
    {
        return (!(rhs < lhs));
    }

    /**
     * Checks if one queue is greater than another
     * 
     * @param lhs Left-hand side queue
     * @param rhs Right-hand side queue
     * @return bool True if lhs is greater than rhs, false otherwise
     * 
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Container>
    bool operator>(const queue<T, Container> &lhs, const queue<T, Container> &rhs)
    {
        return (rhs < lhs);
    }

    /**
     * Checks if one queue is greater than or equal to another
     * 
     * @param lhs Left-hand side queue
     * @param rhs Right-hand side queue
     * @return bool True if lhs is greater than or equal to rhs, false otherwise
     * 
     * @throws None
     * @complexity O(n)
     */
    template <class T, class Container>
    bool operator>=(const queue<T, Container> &lhs, const queue<T, Container> &rhs)
    {
        return (!(lhs < rhs));
    }

};

#endif
