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
int ret_min;
int ret_max;
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
	}
	return EXIT_SUCCESS;
}

typedef struct {
	enum { MIN,MAX,AVG,STDEV }  ThreadType;

} CThreadArgs;

DWORD WINAPI ConsumerThreadFunc(LPVOID lpParam)
{
	CThreadArgs* argv = lpParam;
	argv->ThreadType = MIN;
	int con_i= 0;
	int (*funcToUse)(int) = getMin;
	if (argv->ThreadType == MIN) funcToUse = getMin;
	if (argv->ThreadType == MAX) funcToUse = getMax;
	if (argv->ThreadType == AVG) funcToUse = getAvg;
	if (argv->ThreadType == STDEV) funcToUse = getStDev;
	
	while (con_i < pro && con_i < nSize)
	{
		funcToUse(con_i);
		con_i++;
		if (con_i >= pro) Sleep(100);
	}

	return EXIT_SUCCESS;
}
int getMin(int index)
{
	static int currentIndex = 0;
	if (currentIndex == index) EXIT_SUCCESS;

	int i = 0;

	int *current = at(intQueue, currentIndex);
	int currentMin = *current;
	for (i = currentIndex; i < index; i++, current++)
		if (*current < currentMin)
			currentMin = *current;

	ret_max = currentMin;
	currentIndex = index;

	return EXIT_SUCCESS;
}
int getMax(int index)
{
	static int currentIndex = 0;
	if(currentIndex == index) EXIT_SUCCESS;

	int i = 0;

	int *current = at(intQueue, currentIndex);
	int currentMax = *current;
	for (i = currentIndex; i < index; i++, current++)
		if (*current > currentMax)
			currentMax = *current;

	ret_max = currentMax;
	currentIndex = index;

	return EXIT_SUCCESS;
}
int avgUsing;
int getAvg(int index)
{
	static int currentIndex = 0;
	static long long sum = 0;
	if (currentIndex == index) EXIT_SUCCESS;
	avgUsing = TRUE;

	int i = 0;
	int *current = at(intQueue, currentIndex);
	for (i = currentIndex; i < index; i++, current++)
			sum += *current;

	ret_avg = (double)sum / currentIndex;
	currentIndex = index;

	avgUsing = FALSE;
	return EXIT_SUCCESS;
}
int getStDev(int index)
{
	static int currentIndex = 0;
	static long double sum = 0;
	if (currentIndex == index || avgUsing == TRUE) EXIT_SUCCESS;
	double avg = ret_avg;

	int i = 0;
	int *current = at(intQueue, 0);
	for (i = 0; i < index; i++, current++)
		sum += pow((*current)-ret_avg,2);

	ret_avg = sum / currentIndex;
	currentIndex = index;

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
	scanf("%d %d",&n, &q);
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
				, NULL
				, argvs[i]
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
				, NULL
				, argvs[i]
				, NULL);
		}
	}
	WaitForMultipleObjects(THREADS, ThreadHandles, TRUE, INFINITE);
	for (i = 0; i < THREADS; i++)
	{
		CloseHandle(ThreadHandles[i]);
			free(argvs[i]);
	}
	printf("최소:%d 최대:%d 평균:%d 표준편차%d\n"
		,ret_min, ret_max, ret_avg, ret_std);
	return 0;
}