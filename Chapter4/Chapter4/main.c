#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define NUM_THREAD 4
typedef struct {
	int base;
	int to;
} getNumberOfPrimeArgs;

int getNumberOfPrime(int base, int to)
{
	int index,filterNum;
	int length = to-base + 1;
	int current = base;
	BOOL* primes;
	int filterMax = 0;
	int primeNumbers = 0;
	if (base > to || length < 0) return 0;
	primes = malloc(length * sizeof(BOOL));
	for (index = 0; index < length; index++)
	{
		primes[index] = TRUE;
	}
	
	if (base == 1)
		primes[base - base] = FALSE;

	for (index = base; index <= to; index++)
	{
		filterMax = (int)(sqrt((double)index));
		for (filterNum = 2; filterNum <= filterMax; filterNum++)
		{
			if (index % filterNum == 0)
			{
				primes[index - base] = FALSE;
				break;
			}
		}
	}
	for (index = 0; index < length; index++)
	{
		if (primes[index] == TRUE)
		{
			primeNumbers++;
		}
	}
	free(primes);
	return primeNumbers;
}

int g_allPrimes = 0;
CRITICAL_SECTION primeTotalCritical;
DWORD WINAPI getNumberOfPrimeThreadFunc(LPVOID lpParam)
{
	getNumberOfPrimeArgs* args = lpParam;
	int result = getNumberOfPrime(args->base, args->to);
	
	EnterCriticalSection(&primeTotalCritical);
	g_allPrimes += result;
	LeaveCriticalSection(&primeTotalCritical);

	return 0;
}

int useThread(int min, int max)
{
	int length = max - min;
	int sizePerCores = length / NUM_THREAD;
	int i = 0;
	int result = 0;
	getNumberOfPrimeArgs* args[NUM_THREAD];
	InitializeCriticalSection(&primeTotalCritical);

	for (i = 0; i < NUM_THREAD; i++)
	{
		args[i] = (getNumberOfPrimeArgs*)malloc(sizeof(getNumberOfPrimeArgs));
	}
	HANDLE ThreadHandles[NUM_THREAD];
	int current = min;
	for (i = 0; i < NUM_THREAD; i++)
	{
		args[i]->base = current+(i*sizePerCores);
		args[i]->to = current+((i+1)*sizePerCores);
		ThreadHandles[i] = CreateThread(
			NULL,
			0,
			(LPTHREAD_START_ROUTINE)getNumberOfPrimeThreadFunc,
			args[i],
			0,
			NULL);
		current++;
	}
	WaitForMultipleObjects(NUM_THREAD, ThreadHandles, TRUE, INFINITE);
	for (i = 0; i < NUM_THREAD; i++)
	{
		CloseHandle(ThreadHandles[i]);
		free(args[i]);
	}
	DeleteCriticalSection(&primeTotalCritical);
	return g_allPrimes;
}
double testmain(int min, int max)
{
	clock_t beforetime = 0;
	clock_t currenttime = 0;
	double elapsedtime = 0;
	int result = 0;
	int i = 0;
	const BOOL threadAllowed = TRUE;
	g_allPrimes = 0;
	beforetime = clock();
	if (threadAllowed)
	{
		result = useThread(min, max);
	}
	else
		result = getNumberOfPrime(min, max);
	currenttime = clock();
	elapsedtime = (double)(currenttime - beforetime) / CLOCKS_PER_SEC;
	
	return elapsedtime;
}
int main(int argc, char* argv[])
{
	double elapsedtime = 0;
	int min = 0, max = 0;
	int result = 0;
	int tries = 0;
	const int maxtry = 52;
	scanf_s("%d", &min);
	scanf_s("%d", &max);
	for(tries = 0; tries < maxtry; tries++)
		elapsedtime += testmain(min, max);
	
	printf("걸린 시간 평균:%lf, 소수의 개수:%d\n", elapsedtime/ maxtry, g_allPrimes);

	return 0;
}

