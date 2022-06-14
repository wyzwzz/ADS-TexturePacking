#include <iostream>
#include <algorithm>
#include<queue>
#include<vector>
#include<windows.h>
#include <time.h>  
#define inf 999999
using namespace std;
typedef struct node* Box;
struct node {
	int h;
	int w;
	bool loaded;
};
void InputBoxesHandW(Box boxArr, int givenWidth, int* n);
int LeftDownLoad(Box boxArr, int givenWidth, int n);
int FirstFitDecreasingH(Box boxArr, int givenWidth, int n);
int NextFitDecreasingH(Box boxArr, int givenWidth, int n);
int BestFitDecreasingH(Box boxArr, int givenWidth, int n);
bool cmp1(struct node n1, struct node n2);
int FindMaxHwithinHW(Box boxArr, int n, int limitWidth, int limitHeight = inf);
void AdjuseHandW(Box boxArr, int n, int givenWidth);
/*
int main()
{
	int n, givenwidth, minheight = 0;
	double time_store[4];
	double ave_time[4];
	double time = 0;
	double counts = 0;
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	QueryPerformanceFrequency(&nFreq);
	cout << "Please input the number of boxes: ";
	cin >> n;
	//cout << "Please input the given width: ";
	//cin >> givenwidth;
	givenwidth = 1000;
	for (int i = 0; i < 4; i++) {
		time_store[i] = 0;
		ave_time[i] = 0;
	}
	Box boxArr = new struct node[n + 1];
	if (!boxArr)
	{
		cout << "The number of boxes is too big,Memory allocation failed!";
		return 0;
	}
	InputBoxesHandW(boxArr, givenwidth, &n);
	if (!n)
	{
		cout << "No effective box information input!";
		return 0;
	}
	/*text 100 and get average time*//*
	for (int i = 0; i < 100; i++) {
		QueryPerformanceCounter(&nBeginTime);
		minheight = LeftDownLoad(boxArr, givenwidth, n);
		//cout << "LeftDownLoad:" << minheight << endl;
		QueryPerformanceCounter(&nEndTime);
		time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		time_store[0] += time;
		//cout << "运行时间：" << time * 1000 << "ms" << endl;

		QueryPerformanceCounter(&nBeginTime);
		minheight = FirstFitDecreasingH(boxArr, givenwidth, n);
		//cout << "FFDH:" << minheight << endl;
		QueryPerformanceCounter(&nEndTime);
		time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		time_store[1] += time;
		//cout << "运行时间：" << time * 1000 << "ms" << endl;

		QueryPerformanceCounter(&nBeginTime);
		minheight = NextFitDecreasingH(boxArr, givenwidth, n);
		//cout << "NFDH:" << minheight << endl;
		QueryPerformanceCounter(&nEndTime);
		time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		//cout << "运行时间：" << time * 1000 << "ms" << endl;
		time_store[2] += time;

		QueryPerformanceCounter(&nBeginTime);
		minheight = BestFitDecreasingH(boxArr, givenwidth, n);
		//cout << "BFDH:" << minheight << endl;
		QueryPerformanceCounter(&nEndTime);
		time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		//cout << "运行时间：" << time * 1000 << "ms" << endl;
		time_store[3] += time;
	}
	for (int i = 0; i < 4; i++) {
		//cout << "show the total running time" << endl;
		//cout << time_store[i] << endl;
		cout << "show the average running time" << endl;
		ave_time[i] = time_store[i] / 100;
		cout << ave_time[i]*1000 << "ms" << endl;
	}
	system("pause");
}
*/
void InputBoxesHandW(Box boxArr, int givenWidth, int* n)
{
	int effective_n = 0;
	//cout << "Please input box's height and width: " << endl;
	srand((unsigned)time(NULL));
	for (int i = 0; i < *n; i++)
	{
		int h, w;
		h = rand() % 10 + 1;
		w = rand() % 10 + 1;
		
		//cin >> h >> w;
		if (w > givenWidth)
			continue;
		boxArr[effective_n].h = h;
		boxArr[effective_n].w = w;
		boxArr[effective_n++].loaded = false;
	}
	*n = effective_n;
}

bool cmp1(struct node n1, struct node n2)
{
	if (n1.h > n2.h)
		return true;
	else if (n1.h == n2.h)
		return n1.w > n2.w;
	else
		return false;
}

int FindMaxHwithinHW(Box boxArr, int n, int limitWidth, int limitHeight)
{
	for (int i = 0; i < n; i++)
		if (boxArr[i].loaded == false && boxArr[i].w <= limitWidth && boxArr[i].h <= limitHeight)
			return i;
	return -1;
}
void AdjuseHandW(Box boxArr, int n, int givenWidth)
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


int LeftDownLoad(Box boxArr, int givenWidth, int n)
{
	if (!givenWidth || !n) return 0;
	//AdjuseHandW(boxArr, n, givenWidth);
	sort(boxArr, boxArr + n, cmp1);
	int loadnum = 0, minheight = 0;
	while (loadnum < n)
	{
		int idx = FindMaxHwithinHW(boxArr, n, givenWidth);
		if (idx == -1) break;
		boxArr[idx].loaded = true;
		int levelheight = boxArr[idx].h;
		int levelrestwidth = givenWidth - boxArr[idx].w;
		minheight += levelheight;
		loadnum++;
		while (levelrestwidth > 0)
		{
			idx = FindMaxHwithinHW(boxArr, n, levelrestwidth, levelheight);
			if (idx == -1) break;
			boxArr[idx].loaded = true;
			int rowbasewidth = boxArr[idx].w;
			int rowrestheight = levelheight - boxArr[idx].h;
			levelrestwidth -= rowbasewidth;
			loadnum++;
			while (rowrestheight > 0)
			{
				idx = FindMaxHwithinHW(boxArr, n, rowbasewidth, rowrestheight);
				if (idx == -1) break;
				boxArr[idx].loaded = true;
				rowrestheight -= boxArr[idx].h;
				loadnum++;
			}
		}
	}
	return minheight;
}
/*
 *First-Fit Decreasing Height(FFDH) algorithm
 *FFDH packs the next item R(in non-incresing height) on the first level where R fits.
 *If no level can accommodate R, a new level is created.
 */
int FirstFitDecreasingH(Box boxArr, int givenWidth, int n)
{
	if (!givenWidth || !n) return 0;
	//AdjuseHandW(boxArr, n, givenWidth);
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
		{
			if (levelwidth[i] + boxQueue.front().w <= givenWidth)
			{
				levelwidth[i] += boxQueue.front().w;
				boxQueue.pop();
				findlevel = true;
				break;
			}
		}
		if (!findlevel)
		{
			levelwidth.push_back(boxQueue.front().w);
			minheight += boxQueue.front().h;
			boxQueue.pop();
		}
	}
	return minheight;
}
/*
 *Next-Fit Decreasing Height(NFDH) algorithm
 *NFDH packs the next item R(in non-increasing height) on the current level if R fits.
 *Otherwise,the current level is "closed" and a new level is created.
 */
int NextFitDecreasingH(Box boxArr, int givenWidth, int n)
{
	if (!givenWidth || !n) return 0;
	//AdjuseHandW(boxArr, n, givenWidth);
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
			{
				levelwidth += boxQueue.front().w;
				boxQueue.pop();
			}
			else
				break;
		}
	}
	return minheight;
}
/*
 *Best-Fit Decreasing Height(BFDH) algorithm
 *BFDH packs the next item R(in non-increasing height) on the level,
 *among those that can accommodate R, for which the residual horizontal space is the minimum.
 *If no level can accommodate R, a new level is created.
 */
int BestFitDecreasingH(Box boxArr, int givenWidth, int n)
{
	if (!givenWidth || !n) return 0;
	//AdjuseHandW(boxArr, n, givenWidth);
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
		{
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
		{
			levelwidth.push_back(boxQueue.front().w);
			minheight += boxQueue.front().h;
			boxQueue.pop();
		}
	}
	return minheight;
}