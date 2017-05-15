#include "main.h"
int ThreadIsEmpty(Queue* queue) {
	wait(&in);
	wait(&out);
	int result = isEmpty(queue);
	signal(&out);
	signal(&in);
	return result;
}
int ThreadIsFull(Queue* queue) {
	wait(&in);
	wait(&out);
	int result = isFull(queue);
	signal(&out);
	signal(&in);
	return result;
}

int ThreadEnqueue(Queue* queue, int input, int pro) {
	int result;
	if (n < pro) return 0;
	while (ThreadIsFull(queue)) Sleep(1);
	wait(&in);
	result = enqueue(queue, input);
	signal(&in);
	return result;
}
int ThreadDequeue(Queue* queue, int* con_i, int turn) {
	int result;
	while (ThreadIsEmpty(queue)) {Sleep(1);}
	wait(&out);
	(*con_i)++;
	if (turn != ((THREADS / 2) - 1)) {
		InterlockedIncrement(&count);
		
		result = peek(queue);
	}
	else {
		count = 0;
		result = dequeue(queue);
	}
	signal(&out);
	if (result < 0) {
		printf("err!\n");
		return 1;
	}
	return result;
}
DWORD WINAPI ProducerThreadFunc(LPVOID lpParam)
{
	int randomint;
	while (1)
	{
		srand((unsigned int)time(NULL));
		randomint = pro;
		//randomint = rand() % 4096;
//		printf("try to Produce : ");
		if (ThreadEnqueue(intQueue, randomint, pro)){
			pro++;
//			printf("pro: %d\n", pro);	
		}
		if(n < pro)
		{
//			printf("Proc End\n");
			return EXIT_SUCCESS;
		}
	}
	Sleep(1);
}

DWORD WINAPI getMin_ConsumerThreadFunc(LPVOID lpParam)
{
	int con_i= 0;
	int popped;
	while (1)
	{
		while (con_i > pro) Sleep(1);
		while (count != 0) Sleep(1);
		popped = ThreadDequeue(intQueue, &con_i, 0);
		if (popped < ret_min)
		{
			ret_min = popped;
		}
		if (con_i >= n) return EXIT_SUCCESS;
		Sleep(1);
	}

	return EXIT_SUCCESS;
}
DWORD WINAPI getMax_ConsumerThreadFunc(LPVOID lpParam)
{
	int con_i = 0;
	int popped;
	while (1)
	{
		while (con_i > pro) Sleep(1);
		while (count != 1) Sleep(1);
		popped = ThreadDequeue(intQueue, &con_i, 1);

		if (popped > ret_max)
		{
			ret_max = popped;
		}
		if (con_i >= n) return EXIT_SUCCESS;
		Sleep(1);
	}
}
DWORD WINAPI getAvg_ConsumerThreadFunc(LPVOID lpParam)
{
	int con_i = 0;
	int popped;
	while (1)
	{
		while (con_i > pro) Sleep(1);
		while (count != 3) Sleep(1);
		popped = ThreadDequeue(intQueue, &con_i, 3);
//		printf("popped : %d, ",popped);
		delta = popped - ret_avg;
		ret_avg += delta / con_i;
		signal(&avgUsing);
		if (con_i >= n) return EXIT_SUCCESS;
		Sleep(1);
	}
}
DWORD WINAPI getStdev_ConsumerThreadFunc(LPVOID lpParam)
{
	int con_i = 0;
	int popped;
	while (1)
	{
		while (con_i > pro) Sleep(1);
		while (count != 2) Sleep(1);
		popped = ThreadDequeue(intQueue, &con_i,2);
		static double m2 = 0; // Welford's algorithm
		if (con_i == 1) continue;
		long double delta2 = popped - ret_avg;
		m2 += delta*delta2;
		ret_std = sqrt(m2/(con_i - 1));
		printf("con_i:%d\n", con_i);
		if (con_i >= n) return EXIT_SUCCESS;
		Sleep(1);
	}
}
int main(int argc, char* argv[])
{// main thread
	int i = 0;
	int q;
	HANDLE ThreadHandles[THREADS];

	printf("정수의 개수 n, 큐의 크기 q:");
	//scanf("%d %d",&n, &q);
	printf("\n");
	n = 100;
	q = 4;
	intQueue = makeQueue(q);
	for (i = 0; i < 4; i++)
	{
	ThreadHandles[i] = CreateThread(NULL
		, 0
		, (LPTHREAD_START_ROUTINE)ProducerThreadFunc
		, NULL
		, 0
		, NULL);
	}

	ThreadHandles[4] = CreateThread(NULL
		, 0
		, (LPTHREAD_START_ROUTINE)getMax_ConsumerThreadFunc
		, NULL
		, 0
		, NULL);
	ThreadHandles[5] = CreateThread(NULL
		, 0
		, (LPTHREAD_START_ROUTINE)getMin_ConsumerThreadFunc
		, NULL
		, 0
		, NULL);
	ThreadHandles[6] = CreateThread(NULL
		, 0
		, (LPTHREAD_START_ROUTINE)getAvg_ConsumerThreadFunc
		, NULL
		, 0
		, NULL);
	ThreadHandles[7] = CreateThread(NULL
		, 0
		, (LPTHREAD_START_ROUTINE)getStdev_ConsumerThreadFunc
		, NULL
		, 0
		, NULL);
	WaitForMultipleObjects(THREADS, ThreadHandles, TRUE, INFINITE);
	for (i = 0; i < THREADS; i++)
	{
		CloseHandle(ThreadHandles[i]);
	}
	printQueue(intQueue);
	printf("최소:%d 최대:%d 평균:%lf 표준편차:%lf\n"
		,ret_min, ret_max, ret_avg, ret_std);
	return 0;
}