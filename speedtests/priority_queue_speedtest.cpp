#include <iostream>
#include <cstdlib>

#ifndef IS_STD
# define IS_STD 0
#endif

#if IS_STD == 1
	#include <queue>
	namespace ft = std;
#else
	#include "priority_queue.hpp"
#endif

int	main( void )
{
	ft::priority_queue<int>	testedPQ;
	size_t					numberOfOps = 500000;

	// Push random elements
	for (size_t i = 0; i < numberOfOps; i++)
		testedPQ.push(rand());

	// Pop all elements (extracts in sorted order)
	while (!testedPQ.empty())
	{
		testedPQ.top();
		testedPQ.pop();
	}

	// Push again and pop interleaved
	for (size_t i = 0; i < numberOfOps; i++)
	{
		testedPQ.push(rand());
		if (i % 3 == 0 && !testedPQ.empty())
			testedPQ.pop();
	}

	while (!testedPQ.empty())
		testedPQ.pop();

	return 0;
}
