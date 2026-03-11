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










## About The Project

This project is a from-scratch re-implementation of a subset of the C++98 Standard Template Library (STL). The primary goal is educational: to gain a deep understanding of the underlying mechanics of common data structures, iterators, and algorithms.

Every component, from containers to utility functions, is built from the ground up to mimic the behavior and interface of the standard library, providing insight into memory management, template metaprogramming, and API design.

## Features

This library includes custom implementations of the following STL components, all within the `ft` namespace.

#### Containers
*   **`ft::vector`**: A dynamic, contiguous array.
*   **`ft::list`**: A doubly-linked list.
*   **`ft::deque`**: A double-ended queue.
*   **`ft::map`**: A red-black tree-based associative container.
*   **`ft::set`**: A red-black tree-based associative container with unique keys.
*   **`ft::stack`**: A container adaptor providing LIFO (Last-In, First-Out) functionality.
*   **`ft::queue`**: A container adaptor providing FIFO (First-In, First-Out) functionality.
*   **`ft::priority_queue`**: A container adaptor that provides constant time lookup of the largest element.

#### Utilities & Algorithms
*   **Iterators**: Including `iterator`, `const_iterator`, `reverse_iterator`, and `const_reverse_iterator` for all containers.
*   **Type Traits**: A collection of helpers like `is_integral` and `enable_if` to facilitate template metaprogramming.
*   **Algorithms**: Implementations of `equal` and `lexicographical_compare`.
*   **`ft::pair`**: A utility class for storing a pair of objects.

## Getting Started

To get a local copy up and running, follow these simple steps.

### Prerequisites

You will need a C++ compiler (e.g., `g++` or `clang++`) and `make` installed on your system.

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
This will create an executable file that runs a series of tests on the custom containers.

## Usage

To use a container, simply include the corresponding header file. All components are located in the `ft` namespace to avoid conflicts with the standard `std` library.

Here is a basic example of using `ft::vector`:

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

## Testing

The repository includes multiple ways to test the implementation for correctness and performance.

### Functional Tests

The `tester/` directory contains external test suites for verifying correctness against the standard library.

*   `static_tester.tar`: A comprehensive test suite for functionality and edge cases.
*   `monkey_tester.tar`: A randomized tester to find more obscure bugs.

To use them, extract the archives and follow their respective instructions.

### Performance Tests

The `speedtests/` directory contains benchmarks that compare the performance of each `ft` container against its `std` counterpart.

You can run all speed tests using the provided shell script:
```sh
./speedtests/speedtests.sh
```
