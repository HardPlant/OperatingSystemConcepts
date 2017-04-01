#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

DWORD ExecuteProcess(LPWSTR PATH);
DWORD CreateProcessAndGetPid(
	LPWSTR path);
void randomSleep();

int main(int argc, char* argv[])
{
	LPWSTR ProcAPath = L"./ProcessA.exe";
	LPWSTR ProcBPath = L"./ProcessB.exe";
	int input[4];
	int i;
	int sum = 0;
	int ProcAPid = 0;
	int ProcBPid = 0;
	BOOL doSleep = FALSE;
	ProcAPid = ExecuteProcess(ProcAPath);
	printf("PID : %d\n", ProcAPid);
	ProcBPid = ExecuteProcess(ProcBPath);
	printf("PID : %d\n", ProcBPid);

	for(i=0;i<4;i++)
	{
		if(doSleep) randomSleep();
		scanf_s("%d", &input[i]);
		sum += input[i];
	}

	printf("PID : %d, ", GetCurrentProcessId());
	for(i=0;i<4;i++)
		printf("num%d=%d, ", i+1, input[i]);
	printf("result=%d", sum);
	printf("\n");

	return 0;
}

DWORD ExecuteProcess(LPWSTR PATH)
{
	DWORD pid = CreateProcessAndGetPid(PATH);
	return pid;
}

DWORD CreateProcessAndGetPid(
	LPWSTR path)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	WCHAR procName[80];
	wcsncpy_s(procName, 80, path,80); // strcpyÀÇ widechar¹öÀü
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	
	if (!CreateProcess(NULL,
		procName,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi))
	{
		fprintf(stderr, "Create Process Failed");
		return -1;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return pi.dwProcessId;
}

void randomSleep()
{
    int waittime;

    srand((unsigned int)time(NULL));
    waittime = rand() % 4096;
    
    Sleep(waittime);
}
