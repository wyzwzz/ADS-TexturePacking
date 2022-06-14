#include<iostream>
#include<stdlib.h>
#include<windows.h>
#include<time.h>
#include "pch.h"
using namespace std;
#define MAXHEIGHT 2000
#define MAXWIDTH 1000
#define ROWUSERATE 0.9
// rowuserate means if a row space use rate is more than this, this row would be treated as a new baseline
bool box[MAXHEIGHT][MAXWIDTH] = { 0 };
//0 represent empty while 1 represent space occupied
int onlinePacking(int givenBoxWidth, bool can_rotate);
int loadBox(int baseline, int startH, int startW, int boxH, int boxW);
void updateBaseline(int startH, int* baseline, int givenBoxWidth);
void rotation(int* boxH, int* boxW);
/*
int main()
{
	int minheight;
	int givenBoxWidth;
	double time = 0;
	double counts = 0;
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
	cout << "Please input the given width:" << endl;
	givenBoxWidth = 1000;
	//cin >> givenBoxWidth;
	cout << "Please decide whether the box can be rotated!" << endl;
	cout << "key 0 : the box can't rotate!" << endl;
	cout << "key 1 : the box can rotate!" << endl;
	int rotate;
	rotate = 1;
	//cin >> rotate;
	cout << "Please input box's height and width:" << endl;
	QueryPerformanceCounter(&nBeginTime);
	minheight = onlinePacking(givenBoxWidth, rotate);
	QueryPerformanceCounter(&nEndTime);
	time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
	cout << "The height is " << minheight<<endl;
	cout << "运行时间：" << time * 1000 << "ms" << endl;
	system("pause");
	return 0;
}*/
int onlinePacking(int givenBoxWidth, bool can_rotate)
{
	int boxwidth, boxheight;
	int restwidth, restheight;
	int packing_height = 0, baseline = 0;//baseline means that the rows index small than it should be treated as full use of space, this is for faster search but decrease space use efficency
	int i, j;
	int n = 1;
	srand((unsigned)time(NULL));
	while (true)
	{
		
		boxheight = rand() % 10 + 1;
		boxwidth = rand() % 10 + 1;
		n++;
		if (n == 50000) break;
		//cin >> boxheight >> boxwidth;
		if (boxheight == 0 || boxwidth == 0) break;//end with input 0
		if (can_rotate && boxheight > boxwidth) rotation(&boxheight, &boxwidth);
		bool boxload = false;
		for (i = baseline; i < MAXHEIGHT; i++)
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
						int tmp = loadBox(baseline, i, j - restwidth + 1, boxheight, boxwidth);
						if (tmp > packing_height) packing_height = tmp;//update result minheight
						boxload = true;
						break;
					}
					else if (restwidth >= boxwidth)
					{
						int tmp = loadBox(baseline, i, j - restwidth + 1, boxheight, boxwidth);
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
		updateBaseline(i, &baseline, givenBoxWidth);
	}
	return packing_height;
}
int loadBox(int baseline, int startH, int startW, int boxH, int boxW)
{
	for (int i = baseline; i < startH + boxH; i++)//the space under the box can't be use even if it was empty, so all these space would be occupied!
	{
		for (int j = startW; j < startW + boxW; j++)
		{
			box[i][j] = 1;
		}
	}
	//printf("The box is loaded at box[%d][%d] to box[%d][%d]\n", startH, startW, startH + boxH - 1, startW + boxW - 1);
	return startH + boxH;
}
void updateBaseline(int startH, int* baseline, int givenBoxWidth)
{
	int occup_num = 0;
	for (int i = 0; i < givenBoxWidth; i++)
	{
		if (box[startH][i] == 1)
			occup_num++;
	}
	double space_use_rate = occup_num * 1.0 / givenBoxWidth;
	if (space_use_rate > ROWUSERATE)
		*baseline = startH;
}
void rotation(int* boxH, int* boxW)
{
	int tmp = *boxH;
	*boxH = *boxW;
	*boxW = tmp;
}
