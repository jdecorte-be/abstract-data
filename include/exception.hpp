#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <iostream>
#include <exception>
#include <string>

namespace ft
{
    class exception : public std::exception
    {
    public:
        exception() throw() {}
        exception(const exception &) throw() {}
        virtual ~exception() throw() {}

        exception &operator=(const exception &) throw() { return *this; }
        virtual const char *what() const throw() { return "ft::exception"; }
    };

    class out_of_range : public ft::exception
    {
    public:
        explicit out_of_range(const std::string &what_arg) : s(what_arg) {}
        virtual ~out_of_range() throw() {}
        const char *what() const throw() { return s.c_str(); }

    private:
        std::string s;
    };

    class length_error : public ft::exception
    {
    public:
        explicit length_error(const std::string &what_arg) : s(what_arg) {}
        virtual ~length_error() throw() {}
        const char *what() const throw() { return s.c_str(); }

    private:
        std::string s;
    };
}

#endif
