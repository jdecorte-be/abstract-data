#include <iostream>
#include <cstdlib>

#ifndef IS_STD
# define IS_STD 0
#endif

#if IS_STD == 1
	#include <stack>
	namespace ft = std;
#else
	#include "stack.hpp"
#endif

int	main( void )
{
	ft::stack<int>	testedStack;
	size_t			numberOfPushes = 1000000;

	for (size_t i = 0; i < numberOfPushes; i++)
		testedStack.push(i);
	while (!testedStack.empty())
	{
		testedStack.top();
		testedStack.pop();
	}
	return 0;
}
