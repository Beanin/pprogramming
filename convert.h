#pragma once
#include <vector>
using std::vector;

inline void FieldToBuf(int* buf, vector<vector<int>> &Field)
{
	int height = Field.size();
	int width = Field[0].size();
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			buf[i * width + j] = Field[i][j]; 
		} 
	}	
}

inline void BufToField(int* buf, vector<vector<int>> &Field)
{
	int height = Field.size();
	int width = Field[0].size();
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			Field[i][j] = buf[i * width + j]; 
		} 
	}	
}
