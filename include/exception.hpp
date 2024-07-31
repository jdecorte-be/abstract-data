#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <string> // std::string

namespace ft
{
    class exception : public std::exception // !
    {
    public:
        /**
         * Default constructor for exception
         *
         * @throws None
         * @complexity O(1)
         */
        exception() throw() {}

        /**
         * Copy constructor for exception
         *
         * @param other Another exception object to copy from
         *
         * @throws None
         * @complexity O(1)
         */
        exception(const exception &) throw() {}

        /**
         * Destructor for exception
         *
         * @throws None
         * @complexity O(1)
         */
        virtual ~exception() throw() {}

        /**
         * Assignment operator for exception
         *
         * @param other Another exception object to assign from
         * @return Reference to the assigned exception object
         *
         * @throws None
         * @complexity O(1)
         */
        exception &operator=(const exception &) throw() { return *this; }

        /**
         * Get the exception message
         *
         * @return Constant character pointer to the exception message
         *
         * @throws None
         * @complexity O(1)
         */
        virtual const char *what() const throw() { return "ft::exception"; }
    };

    class out_of_range : public ft::exception
    {
    public:
        /**
         * Constructor for out_of_range with message
         *
         * @param what_arg String message describing the exception
         *
         * @throws None
         * @complexity O(1)
         */
        explicit out_of_range(const std::string &what_arg) : s(what_arg) {}

        /**
         * Destructor for out_of_range
         *
         * @throws None
         * @complexity O(1)
         */
        virtual ~out_of_range() throw() {}

        /**
         * Get the exception message
         *
         * @return Constant character pointer to the exception message
         *
         * @throws None
         * @complexity O(1)
         */
        const char *what() const throw() { return s.c_str(); }

    private:
        std::string s;
    };

    class length_error : public ft::exception
    {
    public:
        /**
         * Constructor for length_error with message
         *
         * @param what_arg String message describing the exception
         *
         * @throws None
         * @complexity O(1)
         */
        explicit length_error(const std::string &what_arg) : s(what_arg) {}

        /**
         * Destructor for length_error
         *
         * @throws None
         * @complexity O(1)
         */
        virtual ~length_error() throw() {}

        /**
         * Get the exception message
         *
         * @return Constant character pointer to the exception message
         *
         * @throws None
         * @complexity O(1)
         */
        const char *what() const throw() { return s.c_str(); }

    private:
        std::string s;
    };
}

#endif