#include "Lab3.h"
#include <vector>
int main()
{
	//vector only used to automate test cases
	vector<char*> testCases;
	char code1[1000] = "int main() \n{ \n int vVar1 =10;\n int var_2 =10;\n if(vVar1<10) \n {\n int var3=20;\n} \n}\n";
	char code2[1000] = "int main() \n{ \n int a = 1;\n char b = 'c'; \n int x, y;\n int z=5;\n}\n";
	char code3[1000] = "int init() \n{ \n char first = 'A'; \n char second = 'B';\n int num = 100;\n}\n";
	char code4[1000] = "char test() \n{ \n int alpha = 9;\n int beta;\n if(alpha == 9)\n {\n char gamma = 'G';\n }\n}\n";
	char code5[1000] = "int calc() \n{ \n int a = 0;\n int result = 100;\n while(result > 0)\n {\n int temp = result;\n result--;\n }\n}\n";
	char code6[1000] = "void process() \n{ \n char flag = 'Y';\n int count = 10;\n if(flag == 'Y') \n {\n char status = 'N';\n }\n}\n";
	char code7[1000] = "void example() \n{ \n int counter = 0;\n if(counter == 0)\n {\n int step = 1;\n }\n char letter = 'Z';\n}\n";
	char code8[1000] = "int test() \n{ \n int var1;\n char var2;\n if(var1 == 1)\n {\n int innerVar = 20;\n }\n}\n";
	
	testCases.push_back(code1);
	testCases.push_back(code2);
	testCases.push_back(code3);
	testCases.push_back(code4);
	testCases.push_back(code5);
	testCases.push_back(code6);
	testCases.push_back(code7);
	testCases.push_back(code8);
	
	//chose testcase
	int num = 0;
	cout << "Chose test case\n1. All Variables\n2. All Variables In Scope\n";
	while (num < 1 || num>2)
	{
		cin >> num;
	}
	
	int whichTest = 1;
	for (auto test: testCases)
	{
		
		char variables[1000] = "";
		cout << "Test: " << whichTest << endl;
		switch (num)
		{
		case 1:
			FindAllVariables(variables, test);
			break;
		case 2:
			FindAllVariablesInScope(variables, test, 5);
			break;
		default:
			break;
		}
		int codeIndex = 0;
		while (variables[codeIndex] != '\0')
		{
			cout << variables[codeIndex];
			codeIndex++;
		}
		codeIndex--;
		whichTest++;
		cout << "\n";
	}
	
	
	
	

}



