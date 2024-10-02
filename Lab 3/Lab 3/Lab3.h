#pragma once
#include <iostream>
using namespace std;
void FindAllVariables(char variables[1000], char code[1000])
{
	int codeIndex = 0;
	int variableIndex = 0;
	bool inFunction = false;
	while (code[codeIndex]!='\0')
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
				variables[variableIndex] = ',' ;
				variableIndex++;

			}
		}
		codeIndex++;
	}
	if (variables[variableIndex - 1] == ',')
	{
		variables[variableIndex - 1] = '\0';
	}
}
void FindAllVariablesInScope(char variables[1000], char code[1000], int lineNumber)
{
	
	int codeIndex = 0;
	int variableIndex = 0;
	bool inFunction = false;
	int countLines = 0;
	bool notEnd = true;
	while (code[codeIndex] != '\0')
	{
		if (code[codeIndex] == '\n'&&countLines<lineNumber)
		{
			countLines++;
		}
		if (countLines == lineNumber&&notEnd)
		{
			if (code[codeIndex] == '}')
			{
				notEnd = false;
			}
			if ((code[codeIndex] == 'i' && code[codeIndex + 1] == 'n' && code[codeIndex + 2] == 't') ||
				(code[codeIndex] == 'c' && code[codeIndex + 1] == 'h' && code[codeIndex + 2] == 'a' && code[codeIndex + 3] == 'r'))
			{
				codeIndex += (code[codeIndex] == 'i') ? 4 : 5; // skip distance based on whether int or char

				while (code[codeIndex] == ' ' || code[codeIndex] == '\t') //skipping whitespace
				{
					codeIndex++;
				}

				while (code[codeIndex] >= 'a' && code[codeIndex] <= 'z' || code[codeIndex] >= 'A' && code[codeIndex] <= 'Z' ||
					code[codeIndex] >= '0' && code[codeIndex] <= '9' || code[codeIndex] == '_')
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
	if (variables[variableIndex - 1] == ',')
	{
		variables[variableIndex - 1] = '\0';
	}
}