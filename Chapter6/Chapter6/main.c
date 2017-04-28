#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"
#define THREADS 8
//**inter-threads globals
int pro;
int count;
int in;
int out;

Queue* intQueue;
//*******//

DWORD WINAPI ProducerThreadFunc(LPVOID lpParam)
{
	srand((unsigned int)time(NULL));
	int randomint = rand() % 4096;

}
DWORD WINAPI ConsumerThreadFunc(LPVOID lpParam)
{
	int con_i= 0;

}
int main(int argc, char* argv[])
{// main thread
	int i = 0;
	int n, q;
	HANDLE ThreadHandles[THREADS];

	printf("정수의 개수 n, 큐의 크기 q:");
	scanf("%d %d",&n, &q);
	
	intQueue = makeQueue(q);

	for (i = 0; i < THREADS; i++)
	{
		if (i % 2 == 0)
		{
			ThreadHandles[i]= CreateThread(NULL
				, 0
				, (LPTHREAD_START_ROUTINE)ProducerThreadFunc
				, NULL
				, 0
				, NULL);
		}
		else
		{
			ThreadHandles[i] = CreateThread(NULL
				, 0
				, (LPTHREAD_START_ROUTINE)ConsumerThreadFunc
				, NULL
				, 0
				, NULL);
		}
	}
	return 0;
}