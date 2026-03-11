<header>
<h1 align="center">
  <a href="https://github.com/jdecorte-be/abstract-data"><img src=".assets/banner.png" alt="abstract-data" ></a>
  abstract-data
  <br>
</h1>

<p align="center">
  A from-scratch re-implementation of C++98 Standard Template Library (STL) containers, algorithms, and iterators for educational purposes.
</p>

<p align="center">
<a href="#">
    <img src="https://img.shields.io/badge/42-School%20Project-00B8BB?logo=42&logoColor=white&labelColor=000000"
         alt="42 School Project">
  </a>
<a href="#">
    <img src="https://img.shields.io/badge/Focus-STL%20Re--implementation-1D759B?labelColor=000000"
         alt="Focus STL Re-implementation">
  </a>
<a href="#">
    <img src="https://img.shields.io/badge/Standard-C%2B%2B98-00599C?logo=cplusplus&logoColor=white&labelColor=000000"
         alt="Standard C++98">
  </a>
<a href="#">
    <img src="https://img.shields.io/badge/Type-Header--only%20Library-blue?labelColor=000000"
         alt="Type Header-only Library">
  </a>
</p>

<p align="center">
<a href="#">
    <img src="https://img.shields.io/badge/Build-Makefile-grey?logo=gnu-make&logoColor=white&labelColor=000000"
         alt="Build Makefile">
  </a>
  <a href="https://github.com/jdecorte-be/abstract-data/stargazers">
    <img src="https://img.shields.io/github/stars/jdecorte-be/abstract-data?logo=star&logoColor=white&labelColor=000000&color=E6DB74"
         alt="abstract-data stars">
  </a>
  <a href="https://github.com/jdecorte-be/abstract-data/issues">
    <img src="https://img.shields.io/github/issues/jdecorte-be/abstract-data?logoColor=white&labelColor=000000&color=orange"
         alt="abstract-data issues">
  </a>
  <a href="https://github.com/jdecorte-be/abstract-data">
    <img src="https://img.shields.io/github/repo-size/jdecorte-be/abstract-data?logo=database&logoColor=white&labelColor=000000&color=AE81FF"
         alt="abstract-data repo size">
  </a>
  <a href="https://github.com/jdecorte-be/abstract-data">
    <img src="https://img.shields.io/github/languages/top/jdecorte-be/abstract-data?logo=code&logoColor=white&labelColor=000000&color=A6E22E"
         alt="abstract-data top language">
  </a>
  <a href="https://github.com/jdecorte-be/abstract-data/commits">
    <img src="https://img.shields.io/github/last-commit/jdecorte-be/abstract-data?logo=clock&logoColor=white&labelColor=000000&color=66D9EF"
         alt="abstract-data last commit">
  </a>
</p>
<p align="center">
  <a href="#associative-containers">Associative containers</a> •
  <a href="#container-adaptors">Container adaptors</a> •
  <a href="#iterators">Iterators</a> •
  <a href="#algorithms">Algorithms</a> •
  <a href="#functors">Functors</a> •
  <a href="#table-of-contents">Table of Contents</a> •
  <a href="#summary">Summary</a>
</p>
</header>










![Abstract Data Banner](./.assets/banner.png)

## About This Project

This project is a from-scratch re-implementation of a subset of the C++98 Standard Template Library (STL). Its primary goal is educational: to gain a deep understanding of the underlying mechanics of common data structures, iterators, and algorithms. Every component is built from the ground up to mimic the behavior and interface of the standard library, providing insight into memory management, template metaprogramming, and API design in C++98.

## Implemented Components

This library provides custom implementations of the following STL components, all within the `ft` namespace to prevent conflicts with the standard `std` library.

#### Containers
*   **`vector`**: A dynamic array that provides contiguous storage.
*   **`list`**: A doubly-linked list enabling efficient insertion and deletion.
*   **`deque`**: A double-ended queue supporting fast insertions and deletions at both ends.
*   **`map`**: An associative container that stores sorted key-value pairs, implemented as a red-black tree.
*   **`set`**: An associative container that stores a sorted set of unique keys, implemented as a red-black tree.
*   **`stack`**: A container adaptor providing LIFO (Last-In, First-Out) functionality.
*   **`queue`**: A container adaptor providing FIFO (First-In, First-Out) functionality.
*   **`priority_queue`**: A container adaptor providing constant-time lookup of the largest element.

#### Core Utilities
*   **Iterators**: Full support for `iterator`, `const_iterator`, `reverse_iterator`, and `const_reverse_iterator` for all containers.
*   **Type Traits**: A collection of helpers like `is_integral` and `enable_if` to facilitate SFINAE and template metaprogramming.
*   **Algorithms**: Implementations of `equal` and `lexicographical_compare`.
*   **`pair`**: A utility class for storing a pair of objects, commonly used in `ft::map`.

## Getting Started

Follow these steps to get a local copy up and running.

### Prerequisites

To build and run this project, you will need:
*   A C++ compiler that supports the C++98 standard (e.g., `g++` or `clang++`)
*   `make`

### Building

1.  Clone the repository:
    ```sh
    git clone https://github.com/jdecorte-be/abstract-data.git
    ```
2.  Navigate to the project directory:
    ```sh
    cd abstract-data
    ```
3.  Compile the project using the Makefile:
    ```sh
    make
    ```
This command builds the main executable, which runs a series of internal tests to validate the custom containers.

## Usage

To use a container in your own project, include the corresponding header file from the `include/` directory. All components are defined within the `ft` namespace.

For example, to use `ft::vector`:

```cpp
#include <iostream>
#include "include/vector.hpp"

int main() {
    ft::vector<int> my_vector;

    my_vector.push_back(10);
    my_vector.push_back(20);
    my_vector.push_back(30);

    std::cout << "My vector contains:";
    for (ft::vector<int>::iterator it = my_vector.begin(); it != my_vector.end(); ++it) {
        std::cout << ' ' << *it;
    }
    std::cout << std::endl;

    return 0;
}
```

To compile this code, you need to tell the compiler where to find the header files using the `-I` flag.

```sh
c++ -Wall -Wextra -Werror -std=c++98 -I./include your_program.cpp -o my_app
```

## Testing

The repository includes comprehensive tests to verify correctness and measure performance against the standard library.

### Functional Tests

The `tester/` directory contains third-party test suites for verifying functional correctness and handling of edge cases.

*   `static_tester.tar`: A comprehensive test suite for functionality.
*   `monkey_tester.tar`: A randomized tester designed to find obscure bugs.

To use them, extract the archives and follow the instructions in their respective README files.

### Performance Benchmarks

The `speedtests/` directory contains benchmarks that compare the performance of each `ft` container against its `std` counterpart.

To run all speed tests, execute the provided shell script:
```sh
./speedtests/speedtests.sh
```
