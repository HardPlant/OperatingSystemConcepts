#include "main.h"
int ThreadIsEmpty(Queue* queue) {

}
int ThreadIsFull(Queue* queue) {

}

int ThreadEnqueue(Queue* queue, int input) {
	while (isFull(queue)) Sleep(1);
	wait(&in);
	enqueue(queue, input);
	signal(&out);
	//if (isFull(queue)) signal(&out);
	return 1;
}
int ThreadDequeue(Queue* queue, int* con_i) {
	while (isEmpty(queue)) Sleep(1);
	wait(&out);
	printf("queue->%d, count: %d\n", queue->index, count);
	int result;
	
	if (count > 0) {
		count--;
		result = peek(queue);
	}
	else {
		count = THREADS / 2;
		result = dequeue(queue);
	}
	
	(*con_i)++;
	signal(&in);
	//if (isEmpty(queue)) signal(&in);
	if (result < 0) return 1;
	return result;
}
DWORD WINAPI ProducerThreadFunc(LPVOID lpParam)
{
	int randomint;
	while (pro < n)
	{
		srand((unsigned int)time(NULL));
		randomint = pro;
		//randomint = rand() % 4096;
		if (ThreadEnqueue(intQueue, randomint))
		{
			pro++;
			printf("pro: %d\n", pro);
		}
		Sleep(1);
	}
	return EXIT_SUCCESS;
}

DWORD WINAPI getMin_ConsumerThreadFunc(LPVOID lpParam)
{
	int con_i= 0;
	int popped;
	while (con_i >= pro) Sleep(1);
	while (con_i < pro && con_i < n)
	{
		popped = ThreadDequeue(intQueue, &con_i);
		if (popped < ret_min)
		{
			ret_min = popped;

		}
		printf("con_i:%d, min: %d\n", con_i,ret_min);
		Sleep(1);
	}

	return EXIT_SUCCESS;
}
DWORD WINAPI getMax_ConsumerThreadFunc(LPVOID lpParam)
{
	int con_i = 0;
	int popped;
	while (con_i >= pro) Sleep(1);
	while (con_i < pro && con_i < n)
	{
		popped = ThreadDequeue(intQueue, &con_i);

		if (popped > ret_max)
		{
			ret_max = popped;
		}
		printf("con_i:%d, max: %d\n", con_i, ret_max);
		Sleep(1);
	}

	return EXIT_SUCCESS;
}
int avgUsing = 0;
DWORD WINAPI getAvg_ConsumerThreadFunc(LPVOID lpParam)
{
	int con_i = 0;
	int popped;
	while (con_i >= pro) Sleep(1);
	while (con_i < pro && con_i < n)
	{
		popped = ThreadDequeue(intQueue, &con_i);

		static int currentIndex = 0;
		static long double sum = 0;
		sum += popped;
		currentIndex++;
		ret_avg = sum / currentIndex;
		signal(&avgUsing);
		printf("con_i:%d, avg: %lf\n", con_i, ret_avg);
		Sleep(1);
	}

	return EXIT_SUCCESS;
}
DWORD WINAPI getStdev_ConsumerThreadFunc(LPVOID lpParam)
{
	int con_i = 0;
	int popped;
	while (con_i >= pro) Sleep(1);
	while (con_i < pro && con_i < n)
	{
		popped = ThreadDequeue(intQueue, &con_i);
		wait(&avgUsing);

		static long double mean = 0;
		static long double m2 = 0;
		static long double delta = 0;
		static long double delta2 = 0;
		static long double total;
		delta = popped - mean; // Welford's algorithm
		mean += delta / con_i;
		delta2 = popped - mean;
		m2 = +delta*delta2;
		if (con_i == 1) continue;
		total = m2 / (con_i - 1);

		ret_std = total;
		printf("con_i:%d, stdev: %lf\n", con_i, ret_std);
		Sleep(1);
	}

	return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{// main thread
	int i = 0;
	int q;
	HANDLE ThreadHandles[THREADS];

	printf("정수의 개수 n, 큐의 크기 q:");
	//scanf("%d %d",&n, &q);
	n = 100;
	q = 30;
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
	printf("최소:%d 최대:%d 평균:%lf 표준편차:%lf\n"
		,ret_min, ret_max, ret_avg, ret_std);
	return 0;
}