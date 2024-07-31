#ifndef STACK_HPP
#define STACK_HPP

#include "utility.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"
#include "exception.hpp"
#include "type_traits.hpp"
#include "vector.hpp"

namespace ft
{

    template <class T, class Container = ft::vector<T>>
    class stack
    {
    public:
        typedef Container container_type;
        typedef typename Container::value_type value_type;
        typedef typename Container::size_type size_type;
        typedef typename Container::reference reference;
        typedef typename Container::const_reference const_reference;

        /**
         * Constructs a stack container adaptor with the given underlying container.
         * 
         * @param ctnr The underlying container to be used (default is an empty container)
         * 
         * @complexity O(1)
         */
        explicit stack(const container_type &ctnr = container_type()) : c(ctnr) {}

        /**
         * Checks whether the stack is empty.
         * 
         * @return True if the stack is empty, false otherwise
         * 
         * @complexity O(1)
         */
        bool empty() const { return c.empty(); }

        /**
         * Returns the number of elements in the stack.
         * 
         * @return The number of elements in the stack
         * 
         * @complexity O(1)
         */
        size_type size() const { return c.size(); }

        /**
         * Returns a reference to the top element in the stack.
         * 
         * @return Reference to the top element in the stack
         * 
         * @complexity O(1)
         */
        reference top() { return c.back(); }

        /**
         * Returns a const reference to the top element in the stack.
         * 
         * @return Const reference to the top element in the stack
         * 
         * @complexity O(1)
         */
        const_reference top() const { return c.back(); }

        /**
         * Pushes an element onto the top of the stack.
         * 
         * @param val The value of the element to be pushed
         * 
         * @complexity O(1)
         */
        void push(const value_type &val) { c.push_back(val); }

        /**
         * Removes the top element from the stack.
         * 
         * @complexity O(1)
         */
        void pop() { c.pop_back(); }

    protected:
        container_type c;

        /**
         * Checks if two stacks are equal.
         * 
         * @param lhs Left-hand side stack
         * @param rhs Right-hand side stack
         * @return True if the stacks are equal, false otherwise
         * 
         * @complexity O(n)
         */
        friend bool operator==(const stack &lhs, const stack &rhs)
        {
            return lhs.c == rhs.c;
        }

        /**
         * Checks if two stacks are not equal.
         * 
         * @param lhs Left-hand side stack
         * @param rhs Right-hand side stack
         * @return True if the stacks are not equal, false otherwise
         * 
         * @complexity O(n)
         */
        friend bool operator!=(const stack &lhs, const stack &rhs)
        {
            return !(lhs == rhs);
        }

        /**
         * Checks if one stack is less than another.
         * 
         * @param lhs Left-hand side stack
         * @param rhs Right-hand side stack
         * @return True if the lhs stack is less than the rhs stack, false otherwise
         * 
         * @complexity O(n)
         */
        friend bool operator<(const stack &lhs, const stack &rhs)
        {
            return lhs.c < rhs.c;
        }

        /**
         * Checks if one stack is greater than another.
         * 
         * @param lhs Left-hand side stack
         * @param rhs Right-hand side stack
         * @return True if the lhs stack is greater than the rhs stack, false otherwise
         * 
         * @complexity O(n)
         */
        friend bool operator>(const stack &lhs, const stack &rhs)
        {
            return rhs < lhs;
        }

        /**
         * Checks if one stack is less than or equal to another.
         * 
         * @param lhs Left-hand side stack
         * @param rhs Right-hand side stack
         * @return True if the lhs stack is less than or equal to the rhs stack, false otherwise
         * 
         * @complexity O(n)
         */
        friend bool operator<=(const stack &lhs, const stack &rhs)
        {
            return !(lhs > rhs);
        }

        /**
         * Checks if one stack is greater than or equal to another.
         * 
         * @param lhs Left-hand side stack
         * @param rhs Right-hand side stack
         * @return True if the lhs stack is greater than or equal to the rhs stack, false otherwise
         * 
         * @complexity O(n)
         */
        friend bool operator>=(const stack &lhs, const stack &rhs)
        {
            return !(lhs < rhs);
        }
    };

} // namespace ft

#endif