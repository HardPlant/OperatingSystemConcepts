#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define CORES 4
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
	printf("쓰레드 실행 : %d, %d\n", args->base, args->to);
	
	EnterCriticalSection(&primeTotalCritical);
	g_allPrimes += result;
	LeaveCriticalSection(&primeTotalCritical);

	return 0;
}

int useThread(int min, int max)
{
	//const int CORES = 4;
	int length = max - min;
	int sizePerCores = length / CORES;
	int i = 0;
	int result = 0;
	getNumberOfPrimeArgs* args[CORES];
	InitializeCriticalSection(&primeTotalCritical);

	for (i = 0; i < CORES; i++)
	{
		args[i] = (getNumberOfPrimeArgs*)malloc(sizeof(getNumberOfPrimeArgs));
	}
	HANDLE ThreadHandles[CORES];
	int current = min;
	for (i = 0; i < CORES; i++)
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
	WaitForMultipleObjects(CORES, ThreadHandles, TRUE, INFINITE);
	for (i = 0; i < CORES; i++)
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
	printf("걸린 시간:%lf, 소수의 갯수 : %d\n", elapsedtime, result);
	
	return elapsedtime;
}
int main(int argc, char* argv[])
{
	double elapsedtime = 0;
	int min = 0, max = 0;
	int result = 0;
	int tries = 0;
	const int maxtry = 50;
	scanf_s("%d", &min);
	scanf_s("%d", &max);
	for(tries = 0; tries < maxtry; tries++)
		elapsedtime += testmain(min, max);
	
	printf("걸린 시간 평균:%lf\n", elapsedtime/ maxtry);

	return 0;
}

