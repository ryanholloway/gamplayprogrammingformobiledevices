#pragma once
#include <iostream>
using namespace std;
void FindAllVariables(char variables[1000], char code[1000])
{
	int codeIndex = 0;
	int variableIndex = 0;
	int inFunction = false;
	while (code[codeIndex] != '\0')
	{
		if (code[codeIndex] == '{')
		{
			inFunction = true;
		}
		if (inFunction)
		{
			if ((code[codeIndex] == 'i' && code[codeIndex + 1] == 'n' && code[codeIndex + 2] == 't') || (code[codeIndex] == 'c' && code[codeIndex + 1] == 'h' && code[codeIndex + 2] == 'a' && code[codeIndex + 3] == 'r'))
			{
				codeIndex += (code[codeIndex] == 'i') ? 4 : 5; // skip distance based on whether int or char

				while (code[codeIndex] == ' ' || code[codeIndex] == '\t') //skipping whitespace
				{
					codeIndex++;
				}

				while (code[codeIndex] >= 'a' && code[codeIndex] <= 'z' || code[codeIndex] >= 'A' && code[codeIndex] <= 'Z' || code[codeIndex] >= '0' && code[codeIndex] <= '9' || code[codeIndex] == '_')
				{
					variables[variableIndex] = code[codeIndex];
					variableIndex++;
					codeIndex++;
				}
				variables[variableIndex] = ',';
				variableIndex++;

			}
		}
		codeIndex++;
	}
	//if the last char is a comma remove it
	if (variables[variableIndex - 1] == ',')
	{
		variables[variableIndex - 1] = '\0';
	}
}
void FindAllVariablesInScope(char variables[1000], char code[1000], int lineNumber)
{
	int codeIndex = 0;
	int variableIndex = 0;
	int inFunction = false;
	int countLines = 0;
	int notEnd = true;
	int ignore = false;
	int countBrack = 0;
	while (code[codeIndex] != '\0')
	{
		if (code[codeIndex] == '\n')
		{
			countLines++;
		}
		if (inFunction && code[codeIndex] == '{')
		{
			ignore = true;
			countBrack++;
		}
		else if (code[codeIndex] == '{')
		{
			inFunction = true;
		}

		//only check if you are in a function and if its above the line number
		if (inFunction && countLines <= lineNumber && !ignore)
		{
			if ((code[codeIndex] == 'i' && code[codeIndex + 1] == 'n' && code[codeIndex + 2] == 't') || (code[codeIndex] == 'c' && code[codeIndex + 1] == 'h' && code[codeIndex + 2] == 'a' && code[codeIndex + 3] == 'r'))
			{
				codeIndex += (code[codeIndex] == 'i') ? 4 : 5; // skip distance based on whether int or char

				while (code[codeIndex] == ' ' || code[codeIndex] == '\t') //skipping whitespace
				{
					codeIndex++;
				}

				while (code[codeIndex] >= 'a' && code[codeIndex] <= 'z' || code[codeIndex] >= 'A' && code[codeIndex] <= 'Z' || code[codeIndex] >= '0' && code[codeIndex] <= '9' || code[codeIndex] == '_')
				{
					variables[variableIndex] = code[codeIndex];
					variableIndex++;
					codeIndex++;
				}
				variables[variableIndex] = ',';
				variableIndex++;

			}
		}
		if (ignore && code[codeIndex] == '}')
		{
			if (countBrack > 0)
				countBrack--;

			if (countBrack == 0)
				ignore = false;
		}
		codeIndex++;
	}
	//if the last char is a comma remove it
	if (variables[variableIndex - 1] == ',')
	{
		variables[variableIndex - 1] = '\0';
	}
}