#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "queue.h"
#define THREADS 8
//**inter-threads globals
int nSize;
int pro;
int count;
int in;
int out;
int ret_min = INT_MAX;
int ret_max = INT_MIN;
double ret_avg;
double ret_std;
Queue* intQueue;
//*******//
int getMin(int);
int getMax(int);
int getAvg(int);
int getStDev(int);

typedef struct {
	int i;
} PThreadArgs;
DWORD WINAPI ProducerThreadFunc(LPVOID lpParam)
{
	PThreadArgs* argv = lpParam;
	int randomint;
	while (pro < nSize)
	{
		srand((unsigned int)time(NULL));
		randomint = rand() % 4096;
		if(enqueue(intQueue, randomint))
			pro++;
		printf("pro: %d\n", pro);
	}
	return EXIT_SUCCESS;
}

typedef struct {
	enum { MIN,MAX,AVG,STDEV }  ThreadType;

} CThreadArgs;

DWORD WINAPI ConsumerThreadFunc(LPVOID lpParam)
{
	CThreadArgs* argv = lpParam;
	int con_i= 0;

	int (*funcToUse)(int) = getMin;
	if (argv->ThreadType == MIN) funcToUse = getMin;
	if (argv->ThreadType == MAX) funcToUse = getMax;
	if (argv->ThreadType == AVG) funcToUse = getAvg;
	if (argv->ThreadType == STDEV) funcToUse = getStDev;
	while (con_i < pro && con_i < nSize)
	{
		int popped = dequeue(intQueue);
		funcToUse(popped);
		con_i++;
		if (con_i >= pro) Sleep(100);
	}

	return EXIT_SUCCESS;
}
int getMin(int popped)
{
	if (popped < ret_min)
		ret_min = popped;

	printf("min: %d\n", ret_min);
	return EXIT_SUCCESS;
}
int getMax(int popped)
{
	if (popped > ret_max)
		ret_max = popped;
	
	return EXIT_SUCCESS;
}
int avgUsing;
int getAvg(int popped)
{
	static int currentIndex = 0;
	static long long sum = 0;
	sum += popped;
	currentIndex++;
	ret_avg = (double)sum / currentIndex;

	printf("avg: %lf\n", ret_avg);
	return EXIT_SUCCESS;
}
int getStDev(int popped)
{
	return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{// main thread
	int i = 0;
	int n, q;
	HANDLE ThreadHandles[THREADS];
	LPVOID argvs[8];
	PThreadArgs* currentPArgs;
	CThreadArgs* currentCArgs;

	printf("정수의 개수 n, 큐의 크기 q:");
	//scanf("%d %d",&n, &q);
	n = 100000;
	q = 20000;
	nSize = n;
	intQueue = makeQueue(q);

	for (i = 0; i < THREADS; i++)
	{
		if (i % 2 != 0)
		{
			argvs[i] = malloc(sizeof(PThreadArgs));
			currentPArgs = argvs[i];
			currentPArgs->i = 0;

			ThreadHandles[i] = CreateThread(NULL
				, 0
				, (LPTHREAD_START_ROUTINE)ProducerThreadFunc
				, argvs[i]
				, 0
				, NULL);
		}
		else
		{
			argvs[i] = malloc(sizeof(CThreadArgs));
			currentCArgs = argvs[i];
			currentCArgs->ThreadType = i/2; // 0 2 4 6 -> 0 1 2 3
			ThreadHandles[i] = CreateThread(NULL
				, 0
				, (LPTHREAD_START_ROUTINE)ConsumerThreadFunc
				, argvs[i]
				, 0
				, NULL);
		}
	}
	WaitForMultipleObjects(THREADS, ThreadHandles, TRUE, INFINITE);
	for (i = 0; i < THREADS; i++)
	{
		CloseHandle(ThreadHandles[i]);
			free(argvs[i]);
	}
	printf("최소:%d 최대:%d 평균:%lf 표준편차:%lf\n"
		,ret_min, ret_max, ret_avg, ret_std);
	return 0;
}