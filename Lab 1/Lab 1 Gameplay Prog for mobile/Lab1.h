#pragma once
#include <iostream>

bool isLeapYear(int year)
{
	if (year % 4 == 0)
	{
		if (year % 100 == 0)
		{
			
			if (year % 400 == 0)
			{
				return true;
			}
			return false;
		}
		return true;
	}
	return false;
}

int Reversed(int testNumber)
{
	int reversed = 0;
	while (testNumber > 0)
	{
		reversed = reversed * 10 + (testNumber % 10);
		testNumber /= 10;
	}
	return reversed;
}
bool isAPalindrome(int testNumber)
{
	return testNumber == Reversed(testNumber);
}
bool isAPrimeNumber(int numbertoTest)
{
	if (numbertoTest <= 1) 
		return false;

	for (int i = 2; i <= std::sqrt(numbertoTest); i++)
	{
		if (numbertoTest % i == 0)
			return false;
	}
	return true;

}
int input5CharsConvertToInt()
{
	int returnInt = 0;
	char inputChar;
	for (int i = 0; i < 5; i++)
	{
		std::cin >> inputChar;
		if (isdigit(inputChar))
		{
			returnInt = returnInt * 10 + (inputChar - '0');
		}
		else
		{
			std::cout << "Invalid input, not a digit!\n";
			return 0;
		}
	}
	return returnInt;
}
int convertBinarytoDecimal(int binaryNumber)
{
	int decimalNumber = 0, base = 1;
	while (binaryNumber)
	{
		int lastDigit = binaryNumber % 10;
		binaryNumber /= 10;
		decimalNumber += lastDigit * base;
		base *= 2;
	}
	return decimalNumber;
}
void drawRightAngledTriangle()
{
	int height;
	std::cout << "Enter the height of the triangle: ";
	std::cin >> height;
	for (int i = 1; i <= height; i++)
	{
		for (int j = 1; j <= i; j++)
		{
			std::cout << "*";
		}
		std::cout << std::endl;
	}
}
void drawIsocelesTriangle()
{
	int height;
	std::cout << "Enter the height of the triangle: ";
	std::cin >> height;
	for (int i = 1; i <= height; i++)
	{
		for (int j = i; j < height; j++)
		{
			std::cout << " ";
		}
		for (int j = 1; j <= (2 * i - 1); j++)
		{
			std::cout << "*";
		}
		std::cout << std::endl;
	}

}
void drawIsocelesTriangle2()
{
	int height;
	std::cout << "Enter the height of the triangle: ";
	std::cin >> height;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < height - i - 1; j++)
		{
			std::cout << " ";
		}
		for (int j = 0; j < 2 * i + 1; j++)
		{
			std::cout << "*";
		}
		std::cout << std::endl;
	}

}

int find(int size, int arr[], int toFind)
{
	for (int i = 0; i < size; i++)
	{
		if (arr[i] == toFind)
		{
			return i;
		}
	}
	return -1;
}
int find2ndLargest(int size, int arr[])
{
	if (size < 2) return -1;
	int largest = INT_MIN, secondLargest = INT_MIN;
	for (int i = 0; i < size; i++)
	{
		if (arr[i] > largest)
		{
			secondLargest = largest;
			largest = arr[i];
		}
		else if (arr[i] > secondLargest && arr[i] != largest)
		{
			secondLargest = arr[i];
		}
	}
	return secondLargest;
}
void copyArraytoArray(int size, int arr1[], int arr2[])
{
	for (int i = 0; i < size; i++)
	{
		arr2[i] = arr1[i];
	}

}
bool insertElement(int& size, int& count, int arr[], int elementToInsert, int insertIndex)
{
	if (count >= size || insertIndex < 0 || insertIndex > count) return false;
	for (int i = count; i > insertIndex; i--)
	{
		arr[i] = arr[i - 1];
	}
	arr[insertIndex] = elementToInsert;
	count++;
	return true;
}
bool deleteElement(int& size, int& count, int arr[], int deleteIndex)
{
	if (deleteIndex < 0 || deleteIndex >= count) 
		return false;
	for (int i = deleteIndex; i < count - 1; i++)
	{
		arr[i] = arr[i + 1];
	}
	count--;
	return true;
}
int frequencyCount(int size, int arr[], int value)
{
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (arr[i] == value) count++;
	}
	return count;
}
int countDuplicates(int size, int arr[])
{
	int duplicates = 0;
	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (arr[i] == arr[j])
			{
				duplicates++;
				break;
			}
		}
	}
	return duplicates;
}
void reverse(int size, int arr[])
{
	for (int i = 0; i < size / 2; i++)
	{
		std::swap(arr[i], arr[size - i - 1]);
	}
}
int rotateLeft(int size, int arr[])
{
	if (size <= 1) return -1;
	int firstElement = arr[0];
	for (int i = 0; i < size - 1; i++)
	{
		arr[i] = arr[i + 1];
	}
	arr[size - 1] = firstElement;
	return 0;
}
bool twoMovies(int flightLength, int movieLengths[], int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (movieLengths[i] + movieLengths[j] == flightLength)
			{
				return true;
			}
		}
	}
	return false;
}
int wordCounter(int size, char characters[])
{
	int words = 0;
	bool inWord = false;
	for (int i = 0; i < size; i++)
	{
		if (std::isspace(characters[i]))
		{
			if (inWord)
			{
				words++;
				inWord = false;
			}
		}
		else
		{
			inWord = true;
		}
	}
	if (inWord) words++;
	return words;
}