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
int ThreadEnqueue(Queue* queue, int input) {
	int result = 0;
	if (n < pro) return 0;
	if (ThreadIsFull(queue)) return 0;
	wait(&in);
	if (result = enqueue(queue, input))
	{
		pro++;
	}
	signal(&in);
	return result;
}
int ThreadDequeue(Queue* queue, int* con_i, int turn) {
	int result;
	if (ThreadIsEmpty(queue)) return 0;
	wait(&out);
	if (turn != 3) {
		InterlockedIncrement(&count);
		
		result = peek(queue);
	}
	else {
		count = 0;
		result = dequeue(queue);
	}
	signal(&out);
	if (result < 0) {
		return 0;
	}
	(*con_i)++;
	return result;
}
DWORD WINAPI ProducerThreadFunc(LPVOID lpParam)
{
	int randomint;
	srand((unsigned int)time(NULL));
	while (1)
	{
		randomint = pro;
		randomint = rand();
		ThreadEnqueue(intQueue, randomint);
		
		if(n < pro)
			return EXIT_SUCCESS;
		Sleep(1);
	}
}

struct ConsumeArgs {
	int index;
};
DWORD WINAPI ConsumerThreadFunc(LPVOID lpParam)
{
	struct ConsumeArgs* args = (struct ConsumeArgs*)lpParam;
	const int turn = args->index;
	int con_i= 0;
	int popped;
	int (*func)(int*, int*) = getMin;
	switch (turn) {
	case 0: func = getMin;
		break;
	case 1: func = getMax;
		break;
	case 2: func = getStDev;
		break;
	case 3: func = getAvg;
		break;
	}
	while (1)
	{
		while (con_i > pro) Sleep(1);
		if (count == turn) {
			popped = ThreadDequeue(intQueue, &con_i, turn);
			if(popped > 0) func(&popped, &con_i);
		}
		if (con_i >= n) return EXIT_SUCCESS;
		Sleep(1);
	}
}
int getMin(int* popped, int* con_i) {
	if (*popped < ret_min)
	{
		ret_min = *popped;
	}
	return 1;
}
int getMax(int* popped, int* con_i) {
	if (*popped > ret_max)
	{
		ret_max = *popped;
	}
	return 1;
}
int getAvg(int* popped, int* con_i) {
	delta = *popped - ret_avg;
	ret_avg += delta / *con_i;
	
	return 1;
}
int getStDev(int *popped, int* con_i) {
	static double m2 = 0; // Welford's algorithm
	if (*con_i == 1) return 0;
	long double delta2 = *popped - ret_avg;
	m2 += delta*delta2;
	ret_std = sqrt(m2 / (*con_i - 1));

	return 1;
}
int main(int argc, char* argv[])
{// main thread
	int i = 0;
	int q;
	HANDLE ThreadHandles[THREADS];

	printf("정수의 개수 n, 큐의 크기 q:");
	scanf("%d %d",&n, &q);
	printf("\n");
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
	struct ConsumeArgs arg[4];
	for (i = 4; i < 8; i++)
	{
		arg[i - 4].index = i - 4;
		ThreadHandles[i] = CreateThread(NULL
			, 0
			, (LPTHREAD_START_ROUTINE)ConsumerThreadFunc
			, &arg[i - 4]
			, 0
			, NULL);
	}
	WaitForMultipleObjects(THREADS, ThreadHandles, TRUE, INFINITE);
	for (i = 0; i < THREADS; i++)
	{
		CloseHandle(ThreadHandles[i]);
	}
	printf("최소:%d 최대:%d 평균:%lf 표준편차:%lf\n"
		,ret_min, ret_max, ret_avg, ret_std);
	return 0;
}