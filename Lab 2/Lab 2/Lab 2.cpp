#include "Header.h"

int main()
{
	//example of calling the functions

	int array[4][6] = { {0,0,3,1,3,4},
					   {0,0,2,3,4,3},
					   {0,0,1,3,3,2},
					   {0,0,1,1,1,1} };
	PrintArray(array);
	std::cout << "\n";
	
	
	int score = ReplaceWithMinusOne(array);

	PrintArray(array);
	std::cout << "\n";

	FallDownAndReplace(array);
	FallDownAndReplace(array);
	
	
	PrintArray(array);
	std::cout << "\n";
	std::cout << score;

}