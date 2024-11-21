#pragma once
#include <iostream>
// dont use cout in any of the functions apart from PrintArray();
int ReplaceWithMinusOne(int array[4][6])
{
	int score = 0;
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (array[row][col] == array[row][col + 1] && array[row][col] == array[row][col + 2])
            {
                int clusterValue = array[row][col];
                score += clusterValue * 3;
                array[row][col] = -1;
                array[row][col + 1] = -1;
                array[row][col + 2] = -1;
            }
        }
    }
    for (int col = 0; col < 6; col++)
    {
        for (int row = 0; row < 2; row++) 
        {
            if (array[row][col] == array[row + 1][col] && array[row][col] == array[row + 2][col])
            {
                int clusterValue = array[row][col];
                score += clusterValue * 3;
                array[row][col] = -1;
                array[row + 1][col] = -1;
                array[row + 2][col] = -1;
            }
        }
    }
    return score;
}
void FallDownAndReplace(int array[4][6])
{
    for (int col = 0; col < 6; col++)
    {
        for (int row = 3; row >= 0; row--)
        {
            if (array[row][col] == -1)
            {

                for (int k = row; k > 0; k--)
                {
                    array[k][col] = array[k - 1][col];
                }
                array[0][col] = -2;
            }
        }
    }
}
void PrintArray(int array[4][6])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            std::cout << array[i][j]<<"   ";
        }
        std::cout << "\n";
    }
}