#include <iostream>
#include <cstdlib>

#ifndef IS_STD
# define IS_STD 0
#endif

#if IS_STD == 1
	#include <list>
	namespace ft = std;
#else
	#include "list.hpp"
#endif

int	main( void )
{
	ft::list<int>	testedList;
	size_t			numberOfOps = 100000;

	// push_back
	for (size_t i = 0; i < numberOfOps; i++)
		testedList.push_back(rand());

	// push_front
	for (size_t i = 0; i < numberOfOps; i++)
		testedList.push_front(rand());

	// Iterate
	ft::list<int>::iterator it = testedList.begin();
	ft::list<int>::iterator ite = testedList.end();
	while (it != ite)
		it++;

	// Sort
	testedList.sort();

	// Unique (removes consecutive duplicates after sort)
	testedList.unique();

	// Reverse
	testedList.reverse();

	// Splice into another list
	ft::list<int> other;
	other.splice(other.begin(), testedList);

	// Clear
	other.clear();

	return 0;
}
