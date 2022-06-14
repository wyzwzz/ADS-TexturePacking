#include<iostream>
#include "pch.h"
using namespace std;
#define MAXHEIGHT 2000
#define MAXWIDTH 1000
bool box[MAXHEIGHT][MAXWIDTH] = { 0 };
//0 represent empty while 1 represent space occupied
int onlinePacking(int givenBoxWidth, bool can_rotate);
int loadBox(int startH, int startW, int boxH, int boxW);
void rotation(int* boxH, int* boxW);
/*
int main()
{
	int minheight;
	int givenBoxWidth;

	cout << "Please input the given width:" << endl;
	givenBoxWidth = 1000;
	cin >> givenBoxWidth;
	cout << "Please decide whether the box can be rotated!" << endl;
	cout << "key 0 : the box can't rotate!" << endl;
	cout << "key 1 : the box can rotate!" << endl;
	int rotate;
	cin >> rotate;
	cout << "Please input box's height and width:" << endl;
	minheight = onlinePacking(givenBoxWidth, rotate);
	cout << "The height is " << minheight << endl;
	return 0;
}*/
int onlinePacking(int givenBoxWidth, bool can_rotate)
{
	int boxwidth, boxheight;
	int restwidth, restheight;
	int packing_height = 0;
	int i, j;
	while (true)
	{		
		cin >> boxheight >> boxwidth;
		if (boxheight == 0 || boxwidth == 0) break;//end with input 0
		if (can_rotate && boxheight > boxwidth) rotation(&boxheight, &boxwidth);
		bool boxload = false;
		for (i = 0; i < MAXHEIGHT; i++)
		{
			restwidth = 0;
			for (j = 0; j < givenBoxWidth; j++)//once find a restwidth enough in i-th level,the box must can be loaded because the operation in function loadBox()!
			{
				if (box[i][j] == 0 && (restwidth < boxwidth || restwidth < boxheight && can_rotate))//restwidth is enougt big for boxwidth or boxheight if can rotate
				{
					restwidth++;
					if (can_rotate && restwidth >= boxheight && boxwidth + i <= packing_height)
					{
						rotation(&boxwidth, &boxheight);
						int tmp = loadBox(i, j - restwidth + 1, boxheight, boxwidth);
						if (tmp > packing_height) packing_height = tmp;//update result minheight
						boxload = true;
						break;
					}
					else if (restwidth >= boxwidth)
					{
						int tmp = loadBox(i, j - restwidth + 1, boxheight, boxwidth);
						if (tmp > packing_height) packing_height = tmp;
						boxload = true;
						break;
					}
				}
				else if (box[i][j] == 1)
				{
					restwidth = 0;
				}
			}
			if (boxload) break;
		}
	}
	return packing_height;
}

int loadBox(int startH, int startW, int boxH, int boxW)
{
	for (int i = 0; i < startH + boxH; i++)//the space under the box can't be use even if it was empty, so all these space would be occupied!
	{
		for (int j = startW; j < startW + boxW; j++)
		{
			box[i][j] = 1;
		}
	}
	printf("The box is loaded at box[%d][%d] to box[%d][%d]\n", startH, startW, startH + boxH - 1, startW + boxW - 1);
	return startH + boxH;
}
void rotation(int* boxH, int* boxW)
{
	int tmp = *boxH;
	*boxH = *boxW;
	*boxW = tmp;
}
