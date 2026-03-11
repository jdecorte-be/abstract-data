#include <iostream>
#include <cstdlib>

#ifndef IS_STD
# define IS_STD 0
#endif

#if IS_STD == 1
	#include <queue>
	namespace ft = std;
#else
	#include "queue.hpp"
#endif

int	main( void )
{
	ft::queue<int>	testedQueue;
	size_t			numberOfOps = 1000000;

	// Push elements
	for (size_t i = 0; i < numberOfOps; i++)
		testedQueue.push(i);

	// Access front and back
	while (!testedQueue.empty())
	{
		testedQueue.front();
		testedQueue.back();
		testedQueue.pop();
	}

	return 0;
}
