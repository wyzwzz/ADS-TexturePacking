#include "pch.h"
#include <iostream>
#include <algorithm>
#include<queue>
#include<vector>
#include<windows.h>
#include<time.h>
#define inf 999999
//MAX_HEIGHT MAX_WIDTH is used for online-packing
#define MAX_HEIGHT 10000
#define MAX_WIDTH 10000
// rowuserate means if a row space use rate is more than this, this row would be treated as a new baseline
#define ROW_USE_RATE 0.9
bool box[MAX_HEIGHT][MAX_WIDTH] = { 0 };
using namespace std;
// Struct used in Next-First Fit Decreasing Height Algorithm
typedef struct node* Box;
struct node {
	int h;
	int w;
	bool loaded;
};
// Class used to store information during every loop
class testinfo{
public:
	testinfo() { height = time = approximate_ratio = 0; }
	double height;
	double time;
	double approximate_ratio;
	void CalAvg(int n) {
		height /= n;
		time /= n;
		approximate_ratio /= n;
	}
};

/**********************************
 *
 * Program runing mode selection
 *
 **********************************/
void TestMode();
void InputMode();

/********************************** 
 *
 * Offline packing function
 *
 **********************************/
void offLinePacking(bool rotate);
void InputBoxesHandW(Box boxArr, int givenWidth, int* n, bool testmode);
int NextFirstFitDecreasingH(Box boxArr, int givenWidth, int n);
int FirstFitDecreasingH(Box boxArr, int givenWidth, int n);
int NextFitDecreasingH(Box boxArr, int givenWidth, int n);
int BestFitDecreasingH(Box boxArr, int givenWidth, int n);
bool cmp1(struct node n1, struct node n2);
int FindMaxHwithinHW(Box boxArr, int n, int limitWidth, int limitHeight = inf);
void AdjustHandW(Box boxArr, int n, int givenWidth);
double CalOptHeight(Box boxArr, int n, int givenWidth);

/**********************************
 *
 * Online packing function
 *
 **********************************/
void onLinePacking(bool rotate, bool faster, bool testmode, int n = inf);
int onLineFirstFitPacking(int givenWidth, bool rotate, bool faster, bool testmode, double* opt_height, int n = inf, Box boxArr = NULL);
int loadBox(int baseline, int startH, int startW, int boxH, int boxW, bool testmode);
void rotation(int *boxH, int *boxW);
void updateBaseline(int startH, int* baseline, int givenWidth);

//-------------------------------------------------------------------------------------------------
int main()
{
	cout << "Please select testing algorithm runing situation or just input with result!" << endl;
	cout << "Key [1] : Testing mode!" << endl;
	cout << "Key [2] : Input mode!" << endl;
	int mode;
	cin >> mode;
	if (mode == 1)
	{
		TestMode();
	}
	else if (mode == 2)
	{
		InputMode();
	}
	else
	{
		cout << "Invalid choice!" << endl;
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------
int maxboxW, maxboxH;
void TestMode()
{
	int n, givenWidth, minheight, testnum;
	double time, opt_height, approximate_ratio;
	bool onlinetest;
	testinfo tinfo[12];
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
	cout << "Please input the number of boxes: ";
	cin >> n;
	cout << "Please input the given width(<=10000): ";
	cin >> givenWidth;
	givenWidth = min(10000, givenWidth);
	cout << "Please input box's max height and max width for random generating: " << endl;
	cin >> maxboxH >> maxboxW;
	cout << "Please decide whether testing on-line for it runs so slow! 1 for yes while 0 for no!" << endl;
	cin >> onlinetest;
	cout << "Please input the number of test times:" << endl;
	cin >> testnum;
	//*****************************************************************
	for (int i = 0; i < testnum; i++)
	{
		Box boxArr = new struct node[n + 1];
		if (!boxArr)
		{
			cout << "The number of boxes is too big, Memory allocation failed!";
			return;
		}
		InputBoxesHandW(boxArr, givenWidth, &n, true);
		opt_height = CalOptHeight(boxArr, n, givenWidth);

		if (onlinetest)
		{
			/***********************************************
			          Online Packing Algorithm
			************************************************/
			// 0. ordinary online-packing without rotation 
			fill(box[0], box[0] + MAX_HEIGHT * MAX_WIDTH, 0);
			QueryPerformanceCounter(&nBeginTime);
			minheight = onLineFirstFitPacking(givenWidth, false, false, true, NULL, n, boxArr);
			QueryPerformanceCounter(&nEndTime);
			time = 1000 * (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
			approximate_ratio = minheight / opt_height;
			tinfo[0].height += minheight;
			tinfo[0].time += time;
			tinfo[0].approximate_ratio += approximate_ratio;

			// 1. faster online-packing without rotation
			fill(box[0], box[0] + MAX_HEIGHT * MAX_WIDTH, 0);
			QueryPerformanceCounter(&nBeginTime);
			minheight = onLineFirstFitPacking(givenWidth, false, true, true, NULL, n, boxArr);
			QueryPerformanceCounter(&nEndTime);
			time = 1000 * (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
			approximate_ratio = minheight / opt_height;
			approximate_ratio = minheight / opt_height;
			tinfo[1].height += minheight;
			tinfo[1].time += time;
			tinfo[1].approximate_ratio += approximate_ratio;

			// 2. ordinary online-packing with rotation 
			fill(box[0], box[0] + MAX_HEIGHT * MAX_WIDTH, 0);
			QueryPerformanceCounter(&nBeginTime);
			minheight = onLineFirstFitPacking(givenWidth, true, false, true, NULL, n, boxArr);
			QueryPerformanceCounter(&nEndTime);
			time = 1000 * (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
			approximate_ratio = minheight / opt_height;
			approximate_ratio = minheight / opt_height;
			tinfo[2].height += minheight;
			tinfo[2].time += time;
			tinfo[2].approximate_ratio += approximate_ratio;

			// 3. faster online-packing with rotation
			fill(box[0], box[0] + MAX_HEIGHT * MAX_WIDTH, 0);
			QueryPerformanceCounter(&nBeginTime);
			minheight = onLineFirstFitPacking(givenWidth, true, true, true, NULL, n, boxArr);
			QueryPerformanceCounter(&nEndTime);
			time = 1000 * (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
			approximate_ratio = minheight / opt_height;
			approximate_ratio = minheight / opt_height;
			tinfo[3].height += minheight;
			tinfo[3].time += time;
			tinfo[3].approximate_ratio += approximate_ratio;

		}

		/***************************************************
		             Offline Packing Algorithm
		 ***************************************************/

		// Without rotation
		// 4. Next-First Fit Decreasing Height Algorithm
		QueryPerformanceCounter(&nBeginTime);
		minheight = NextFirstFitDecreasingH(boxArr, givenWidth, n);
		QueryPerformanceCounter(&nEndTime);
		time = 1000 * (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		approximate_ratio = minheight / opt_height;
		approximate_ratio = minheight / opt_height;
		tinfo[4].height += minheight;
		tinfo[4].time += time;
		tinfo[4].approximate_ratio += approximate_ratio;

		// 5. First Fit Decreasing Height Algorithm
		QueryPerformanceCounter(&nBeginTime);
		minheight = FirstFitDecreasingH(boxArr, givenWidth, n);
		QueryPerformanceCounter(&nEndTime);
		time = 1000 * (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		approximate_ratio = minheight / opt_height;
		approximate_ratio = minheight / opt_height;
		tinfo[5].height += minheight;
		tinfo[5].time += time;
		tinfo[5].approximate_ratio += approximate_ratio;

		// 6. Next Fit Decreasing Height Algorithm
		QueryPerformanceCounter(&nBeginTime);
		minheight = NextFitDecreasingH(boxArr, givenWidth, n);
		QueryPerformanceCounter(&nEndTime);
		time = 1000 * (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		approximate_ratio = minheight / opt_height;
		approximate_ratio = minheight / opt_height;
		tinfo[6].height += minheight;
		tinfo[6].time += time;
		tinfo[6].approximate_ratio += approximate_ratio;

		// 7. Best Fit Decreasing Height Algorithm
		QueryPerformanceCounter(&nBeginTime);
		minheight = BestFitDecreasingH(boxArr, givenWidth, n);
		QueryPerformanceCounter(&nEndTime);
		time = 1000 * (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		approximate_ratio = minheight / opt_height;
		approximate_ratio = minheight / opt_height;
		tinfo[7].height += minheight;
		tinfo[7].time += time;
		tinfo[7].approximate_ratio += approximate_ratio;

		//**************************************************************************
		// With rotation
		AdjustHandW(boxArr, n, givenWidth);
		for (int i = 0; i < n; i++)
		{
			boxArr[i].loaded = false;
		}
		// 8. Next-First Fit Decreasing Fit Algorithm
		QueryPerformanceCounter(&nBeginTime);
		minheight = NextFirstFitDecreasingH(boxArr, givenWidth, n);
		QueryPerformanceCounter(&nEndTime);
		time = 1000 * (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		approximate_ratio = minheight / opt_height;
		approximate_ratio = minheight / opt_height;
		tinfo[8].height += minheight;
		tinfo[8].time += time;
		tinfo[8].approximate_ratio += approximate_ratio;

		// 9. First Fit Decreasing Fit Algorithm
		QueryPerformanceCounter(&nBeginTime);
		minheight = FirstFitDecreasingH(boxArr, givenWidth, n);
		QueryPerformanceCounter(&nEndTime);
		time = 1000 * (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		approximate_ratio = minheight / opt_height;
		approximate_ratio = minheight / opt_height;
		tinfo[9].height += minheight;
		tinfo[9].time += time;
		tinfo[9].approximate_ratio += approximate_ratio;

		// 10. Next Fit Decreasing Fit Algorithm
		QueryPerformanceCounter(&nBeginTime);
		minheight = NextFitDecreasingH(boxArr, givenWidth, n);
		QueryPerformanceCounter(&nEndTime);
		time = 1000 * (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		approximate_ratio = minheight / opt_height;
		approximate_ratio = minheight / opt_height;
		tinfo[10].height += minheight;
		tinfo[10].time += time;
		tinfo[10].approximate_ratio += approximate_ratio;

		// 11. Best Fit Decreasing Fit Algorithm
		QueryPerformanceCounter(&nBeginTime);
		minheight = BestFitDecreasingH(boxArr, givenWidth, n);
		QueryPerformanceCounter(&nEndTime);
		time = 1000 * (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		approximate_ratio = minheight / opt_height;
		approximate_ratio = minheight / opt_height;
		tinfo[11].height += minheight;
		tinfo[11].time += time;
		tinfo[11].approximate_ratio += approximate_ratio;

		delete[] boxArr;
	}
	for (int i = 0; i < 12; i++)
		tinfo[i].CalAvg(testnum);
	if (onlinetest)
	{
		cout << "[1] Testing result for on-line algorithm: " << endl;
		cout << "[1.1] Rotation denied: " << endl;
		printf("onLine-ordinary: %d  Average Testing Time: %.3fms Appoximate-ratio: %.2f\n", (int)(tinfo[0].height + 0.5), tinfo[0].time, tinfo[0].approximate_ratio);
		printf("onLine-faster: %d  Averate Testing Time: %.3fms Appoximate-ratio: %.2f\n", (int)(tinfo[1].height + 0.5), tinfo[1].time, tinfo[1].approximate_ratio);
		cout << "[1.2] Rotation permited: " << endl;
		printf("onLine-ordinary: %d  Average Testing Time: %.3fms Appoximate-ratio: %.2f\n", (int)(tinfo[2].height + 0.5), tinfo[2].time, tinfo[2].approximate_ratio);
		printf("onLine-faster: %d  Average Testing Time: %.3fms Appoximate-ratio: %.2f\n", (int)(tinfo[3].height + 0.5), tinfo[3].time, tinfo[3].approximate_ratio);
	}

	cout << "[2] Testing result for off-line algorithm: " << endl;
	cout << "[2.1] Rotation denied: " << endl;
	printf("N-FFDH: %d  Average Testing Time: %.3fms Appoximate-ratio: %.2f\n", (int)(tinfo[4].height + 0.5), tinfo[4].time, tinfo[4].approximate_ratio);
	printf("FFDH: %d  Average Testing Time: %.3fms Appoximate-ratio: %.2f\n", (int)(tinfo[5].height + 0.5), tinfo[5].time, tinfo[5].approximate_ratio);
	printf("NFDH: %d  Average Testing Time: %.3fms Appoximate-ratio: %.2f\n", (int)(tinfo[6].height + 0.5), tinfo[6].time, tinfo[6].approximate_ratio);
	printf("BFDH: %d  Average Testing Time: %.3fms Appoximate-ratio: %.2f\n", (int)(tinfo[7].height + 0.5), tinfo[7].time, tinfo[7].approximate_ratio);
	cout << "[2.2] Rotation permited: " << endl;
	printf("N-FFDH: %d  Average Testing Time: %.3fms Appoximate-ratio: %.2f\n", (int)(tinfo[8].height + 0.5), tinfo[8].time, tinfo[8].approximate_ratio);
	printf("FFDH: %d  Average Testing Time: %.3fms Appoximate-ratio: %.2f\n", (int)(tinfo[9].height + 0.5), tinfo[9].time, tinfo[9].approximate_ratio);
	printf("NFDH: %d  Average Testing Time: %.3fms Appoximate-ratio: %.2f\n", (int)(tinfo[10].height + 0.5), tinfo[10].time, tinfo[10].approximate_ratio);
	printf("BFDH: %d  Average Testing Time: %.3fms Appoximate-ratio: %.2f\n", (int)(tinfo[11].height + 0.5), tinfo[11].time, tinfo[11].approximate_ratio);
}

//-------------------------------------------------------------------------------------------------
void InputMode()
{
	cout << "Please select online or offline packing mode!" << endl;
	cout << "Key [1] : Off-Line Packing!" << endl;
	cout << "Key [2] : On-Line Packing!" << endl;
	int submode;
	cin >> submode;
	cout << "Please decide whether the box can be rotated!" << endl;
	cout << "Key 0 represent can't be rotated while Key 1 represetn can be rotated!" << endl;
	bool rotate;
	cin >> rotate;
	if (submode == 1)
	{
		offLinePacking(rotate);
	}
	else if (submode == 2)
	{
		bool faster;
		cout << "Input 0 for ordinary online algorithm while 1 for faster online algorithm!" << endl;
		cin >> faster;
		onLinePacking(rotate, faster, false);
	}
	return;
}

//-------------------------------------------------------------------------------------------------
void offLinePacking(bool rotate)
{
	int n, givenWidth, minheight = 0;
	double approximate_ratio, opt_height;
	cout << "Please input the number of boxes: ";
	cin >> n;
	cout << "Please input the given width: ";
	cin >> givenWidth;
	Box boxArr = new struct node[n + 1];
	if (!boxArr)
	{
		cout << "The number of boxes is too big, Memory allocation failed!";
		return;
	}
	InputBoxesHandW(boxArr, givenWidth, &n, false);
	if (!n)
	{
		cout << "No effective box information input!";
		return;
	}
	if (rotate) AdjustHandW(boxArr, n, givenWidth);

	opt_height = CalOptHeight(boxArr, n, givenWidth);

	minheight = NextFirstFitDecreasingH(boxArr, givenWidth, n);
	approximate_ratio = minheight / opt_height;
	printf("Next-First Fit Decreaing Height: %d  Appoximate-ratio: %.6f\n", minheight, approximate_ratio);

	minheight = FirstFitDecreasingH(boxArr, givenWidth, n);
	approximate_ratio = minheight / opt_height;
	printf("First Fit Decreasing Height: %d  Appoximate-ratio: %.6f\n", minheight, approximate_ratio);

	minheight = NextFitDecreasingH(boxArr, givenWidth, n);
	approximate_ratio = minheight / opt_height;
	printf("Next Fit Decreasing Height: %d  Appoximate-ratio: %.6f\n", minheight, approximate_ratio);

	minheight = BestFitDecreasingH(boxArr, givenWidth, n);
	approximate_ratio = minheight / opt_height;
	printf("Best Fit Decreasing Height: %d  Appoximate-ratio: %.6f\n", minheight, approximate_ratio);
	delete[] boxArr;
}

//-------------------------------------------------------------------------------------------------
void onLinePacking(bool rotate, bool faster, bool testmode, int n)
{
	int givenWidth, minheight = 0;
	double approximate_ratio, opt_height;
	cout << "Please input the given width: ";
	cin >> givenWidth;
	cout << "Please input box's height and width!(0 0 for ending)" << endl;
	minheight = onLineFirstFitPacking(givenWidth, rotate, faster, testmode, &opt_height);
	approximate_ratio = minheight / opt_height;
	printf("The fianl height of online-packing is %d, and the approximate ratio is %.6f", minheight, approximate_ratio);
}

/*****************************************************************************************************************************
 *
 * This algorithm is our own thougth.
 * This algorithm always finds the lowest place where box can be loaded.
 * This algorithm use large space and cost much time.
 * We design a faster mode, which depends on the threshold:ROW_USE_RATE(default 0.9)
 * The threshold is more small then the time cost is more less while the approximate ratio increasing 
 *
 *****************************************************************************************************************************/
int onLineFirstFitPacking(int givenWidth, bool rotate, bool faster, bool testmode, double* opt_height, int n, Box boxArr)
{
	int boxwidth, boxheight;
	int restwidth;
	int packing_height = 0, baseline = 0, total_area = 0;//baseline means that the space under it should be regarded as full use of
	int i, j, k = 0;
	while (true)
	{
		if (testmode)
		{
			boxheight = boxArr[k].h % maxboxH + 1;
			boxwidth = boxArr[k++].w % maxboxW + 1;
			if (k == n) break;
		}
		else
			cin >> boxheight >> boxwidth;
		if (boxheight == 0 || boxwidth == 0) break;//end with input 0
		total_area += boxheight * boxwidth;
		if (rotate && boxheight > boxwidth) rotation(&boxheight, &boxwidth);
		bool boxload = false;
		for (i = baseline; i < MAX_HEIGHT; i++)
		{
			restwidth = 0;
			for (j = 0; j < givenWidth; j++)//once find a restwidth enough in i-th level,the box must can be loaded because the operation in function loadBox()!
			{
				if (box[i][j] == 0 && (restwidth < boxwidth || restwidth < boxheight && rotate))//restwidth is enougt big for boxwidth or boxheight if can rotate
				{
					restwidth++;
					if (rotate && restwidth >= boxheight && boxwidth + i <= packing_height)
					{
						rotation(&boxwidth, &boxheight);
						int tmp = loadBox(baseline, i, j - restwidth + 1, boxheight, boxwidth, testmode);//load this box in certain place
						if (tmp > packing_height) packing_height = tmp;//update result minheight
						boxload = true;
						break;
					}
					else if (restwidth >= boxwidth)
					{
						int tmp = loadBox(baseline, i, j - restwidth + 1, boxheight, boxwidth, testmode);
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
		if (faster)
			updateBaseline(i, &baseline, givenWidth);//once load a box, call the function to update baseline
	}
	if (opt_height)
		*opt_height = total_area * 1.0 / givenWidth;
	return packing_height;
}

//-------------------------------------------------------------------------------------------------
int loadBox(int baseline, int startH, int startW, int boxH, int boxW, bool testmode)
{
	for (int i = baseline; i < startH + boxH; i++)//the space under the box can't be use even if it was empty, so all these space would be occupied, but we just need to start from baseline
	{
		for (int j = startW; j < startW + boxW; j++)
		{
			box[i][j] = 1;
		}
	}
	if (!testmode)
		printf("The box is loaded at box[%d][%d] to box[%d][%d]\n", startH, startW, startH + boxH - 1, startW + boxW - 1);
	return startH + boxH;
}

//-------------------------------------------------------------------------------------------------
void updateBaseline(int startH, int* baseline, int givenWidth)
{
	int occup_num = 0;
	for (int i = 0; i < givenWidth; i++)
	{
		if (box[startH][i] == 1)
			occup_num++;
	}
	double space_use_rate = occup_num * 1.0 / givenWidth;//calculate the line of last loading height
	if (space_use_rate > ROW_USE_RATE)                   //if it's space use rate more than a threshold ,then set it as baseline
		*baseline = startH;
}

//-------------------------------------------------------------------------------------------------
void rotation(int *boxH, int *boxW)
{
	int tmp = *boxH;
	*boxH = *boxW;
	*boxW = tmp;
}

/*************************************************************************
 *
 * Input the box's height and width for offline-algorithm
 *
 *************************************************************************/
void InputBoxesHandW(Box boxArr, int givenWidth, int* n, bool testmode)
{
	int effective_n = 0;
	if (!testmode)
		cout << "Please input box's height and width: " << endl;
	srand((unsigned)time(NULL));
	for (int i = 0; i < *n; i++)
	{
		int h, w;
		if (testmode)
		{
			h = rand() % maxboxH + 1;
			w = rand() % maxboxW + 1;
		}
		else
			cin >> h >> w;
		if (w > givenWidth)
			continue;
		boxArr[effective_n].h = h;
		boxArr[effective_n].w = w;
		boxArr[effective_n++].loaded = false;
	}
	*n = effective_n;
}

/****************************************************************************
 *
 * Suppose the opt solution use the full space and return the opt height
 *
 ****************************************************************************/
double CalOptHeight(Box boxArr, int n, int givenWidth)
{
	int total_area = 0;
	for (int i = 0; i < n; i++)
	{
		total_area += boxArr[i].h*boxArr[i].w;
	}
	return total_area * 1.0 / givenWidth;
}

/**************************************
 *
 * order by height desc, width desc
 *
 **************************************/
bool cmp1(struct node n1, struct node n2)
{
	if (n1.h > n2.h)
		return true;
	else if (n1.h == n2.h)
		return n1.w > n2.w;
	else
		return false;
}

/**********************************************************************
 *
 * Find the first box which is unloaded 
 * and it's height and width satisfy the limitWidth and limitHeight
 *
 **********************************************************************/
int FindMaxHwithinHW(Box boxArr, int n, int limitWidth, int limitHeight)
{
	for (int i = 0; i < n; i++)
		if (boxArr[i].loaded == false && boxArr[i].w <= limitWidth && boxArr[i].h <= limitHeight)
			return i;
	return -1;
}

/****************************************************
 *
 * Rotate the box if it's height is more than width
 *
 ****************************************************/
void AdjustHandW(Box boxArr, int n, int givenWidth)
{
	int w, h;
	for (int i = 0; i < n; i++)
	{
		w = boxArr[i].w;
		h = boxArr[i].h;
		if (max(w, h) <= givenWidth)
		{
			boxArr[i].w = max(w, h);
			boxArr[i].h = min(w, h);
		}
		else
		{
			boxArr[i].h = max(w, h);
			boxArr[i].w = min(w, h);
		}
	}
}

/***********************************************************************************************************************
 *
 * Our own algorithm.
 * This algorithm is level oriented.
 * Sort the box order by height desc, width desc.
 * For every level, find the unloaded box1 with max height, so the level height is this box's height.
 * Find the box2 which fits the level remain width with max height.
 * Then we find the box3 which it's width small than box2 and its height add box2' height no more than level height. 
 * We can find more fit box which will loaded on the box3 until the there is no box suitable
 * Also we can find more fit box next to box2 and repeat the operation.
 *
 ***********************************************************************************************************************/
int NextFirstFitDecreasingH(Box boxArr, int givenWidth, int n)
{
	if (!givenWidth || !n) return 0;
	sort(boxArr, boxArr + n, cmp1);
	int loadnum = 0, minheight = 0;
	while (loadnum < n)
	{
		int idx = FindMaxHwithinHW(boxArr, n, givenWidth);//Find the fisrt box for every level
		if (idx == -1) break;
		boxArr[idx].loaded = true;
		int levelheight = boxArr[idx].h;//Set the level height equal to the fist box' height
		int levelrestwidth = givenWidth - boxArr[idx].w;//Set the level remain width
		minheight += levelheight;
		loadnum++;
		while (levelrestwidth > 0)
		{
			idx = FindMaxHwithinHW(boxArr, n, levelrestwidth, levelheight);//Find the box next to
			if (idx == -1) break;
			boxArr[idx].loaded = true;
			int rowbasewidth = boxArr[idx].w;
			int rowrestheight = levelheight - boxArr[idx].h;
			levelrestwidth -= rowbasewidth;
			loadnum++;
			while (rowrestheight > 0)
			{
				idx = FindMaxHwithinHW(boxArr, n, rowbasewidth, rowrestheight);//Find the box above
				if (idx == -1) break;
				boxArr[idx].loaded = true;
				rowrestheight -= boxArr[idx].h;
				loadnum++;
			}
		}
	}
	return minheight;
}

/****************************************************************************************
 *
 * First-Fit Decreasing Height(FFDH) algorithm
 * FFDH packs the next item R(in non-incresing height) on the first level where R fits.
 * If no level can accommodate R, a new level is created.
 *
 ****************************************************************************************/
int FirstFitDecreasingH(Box boxArr, int givenWidth, int n)
{
	if (!givenWidth || !n) return 0;
	sort(boxArr, boxArr + n, cmp1);
	queue<struct node> boxQueue;
	for (int i = 0; i < n; i++)
		boxQueue.push(boxArr[i]);
	vector<int> levelwidth;
	int minheight = 0;
	while (!boxQueue.empty())
	{
		bool findlevel = false;
		for (int i = 0; i < levelwidth.size(); i++)
		{	//find the first level that can load the box
			if (levelwidth[i] + boxQueue.front().w <= givenWidth)
			{
				levelwidth[i] += boxQueue.front().w;
				boxQueue.pop();
				findlevel = true;
				break;
			}
		}
		if (!findlevel)
		{	//if no exist level is found to load, than create a new level
			levelwidth.push_back(boxQueue.front().w);
			minheight += boxQueue.front().h;
			boxQueue.pop();
		}
	}
	return minheight;
}

/*********************************************************************************************
 *
 * Next-Fit Decreasing Height(NFDH) algorithm
 * NFDH packs the next item R(in non-increasing height) on the current level if R fits.
 * Otherwise,the current level is "closed" and a new level is created.
 *
 *********************************************************************************************/
int NextFitDecreasingH(Box boxArr, int givenWidth, int n)
{
	if (!givenWidth || !n) return 0;
	sort(boxArr, boxArr + n, cmp1);
	queue<struct node> boxQueue;
	for (int i = 0; i < n; i++)
		boxQueue.push(boxArr[i]);
	int minheight = 0;
	while (!boxQueue.empty())
	{
		minheight += boxQueue.front().h;
		int levelwidth = boxQueue.front().w;
		boxQueue.pop();
		while (!boxQueue.empty())
		{	
			if (levelwidth + boxQueue.front().w <= givenWidth)
			{   //if the level remain width is not big enough for the next box, create a new level
				levelwidth += boxQueue.front().w;
				boxQueue.pop();
			}
			else
				break;
		}
	}
	return minheight;
}

/***********************************************************************************************
 *
 * Best-Fit Decreasing Height(BFDH) algorithm
 * BFDH packs the next item R(in non-increasing height) on the level,
 * among those that can accommodate R, for which the residual horizontal space is the minimum.
 * If no level can accommodate R, a new level is created.
 *
 ***********************************************************************************************/
int BestFitDecreasingH(Box boxArr, int givenWidth, int n)
{
	if (!givenWidth || !n) return 0;
	sort(boxArr, boxArr + n, cmp1);
	queue<struct node> boxQueue;
	for (int i = 0; i < n; i++)
		boxQueue.push(boxArr[i]);
	vector<int> levelwidth;
	int minheight = 0;
	while (!boxQueue.empty())
	{
		bool findlevel = false;
		int maxlevelusewidth = 0, idx = -1;
		for (int i = 0; i < levelwidth.size(); i++)
		{	//find the level that can load the box and the remain width is smallest
			if (levelwidth[i] + boxQueue.front().w <= givenWidth && levelwidth[i] + boxQueue.front().w > maxlevelusewidth)
			{           
				maxlevelusewidth = levelwidth[i] + boxQueue.front().w;
				idx = i;
				findlevel = true;
			}
		}
		if (findlevel)
		{
			levelwidth[idx] += boxQueue.front().w;
			boxQueue.pop();
		}
		else
		{	//if no exist level is found to load, than create a new level
			levelwidth.push_back(boxQueue.front().w);
			minheight += boxQueue.front().h;
			boxQueue.pop();
		}
	}
	return minheight;
}
