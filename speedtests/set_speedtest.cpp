#include <iostream>
#include <cstdlib>

#ifndef IS_STD
# define IS_STD 0
#endif

#if IS_STD == 1
	#include <set>
	namespace ft = std;
#else
	#include "set.hpp"
#endif

int	main( void )
{
	ft::set<int>			testedSet;
	ft::set<int>::iterator	it, ite;
	size_t					numberOfInserts = 50000;

	// Insert elements
	for (size_t i = 0; i < numberOfInserts; i++)
		testedSet.insert(rand());

	// Iterate through all elements
	it = testedSet.begin();
	ite = testedSet.end();
	while (it != ite)
		it++;

	// Find each element
	for (size_t i = 0; i < numberOfInserts; i++)
		testedSet.find(rand());

	// Erase all elements one by one
	while (!testedSet.empty())
		testedSet.erase(testedSet.begin());

	return 0;
}
