#include <iostream>
#include <cstdlib>

#ifndef IS_STD
# define IS_STD 0
#endif

#if IS_STD == 1
	#include <deque>
	namespace ft = std;
#else
	#include "deque.hpp"
#endif

int	main( void )
{
	ft::deque<int>	testedDeque;
	size_t			numberOfOps = 100000;

	// push_back
	for (size_t i = 0; i < numberOfOps; i++)
		testedDeque.push_back(i);

	// push_front
	for (size_t i = 0; i < numberOfOps; i++)
		testedDeque.push_front(i);

	// Random access
	for (size_t i = 0; i < testedDeque.size(); i++)
		testedDeque[i];

	// Insert in the middle
	for (size_t i = 0; i < 1000; i++)
	{
		ft::deque<int>::iterator mid = testedDeque.begin();
		for (size_t j = 0; j < testedDeque.size() / 2 && j < 500; j++)
			mid++;
		testedDeque.insert(mid, 42);
	}

	// pop_back + pop_front alternating
	while (!testedDeque.empty())
	{
		testedDeque.pop_back();
		if (!testedDeque.empty())
			testedDeque.pop_front();
	}

	return 0;
}
