#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

void randomSleep();

int main(int argc, char* argv[])
{
    int input[4];
    int i;
    int result = 0;
    BOOL doSleep = TRUE;
    for(i=0;i<4;i++)
    {
        if(doSleep) randomSleep();
        scanf_s("%d", &input[i]);
    }
    result = input[0];
    for(i=1;i<4;i++)
    {
        result+=input[i];
    }
    result /= 4;
    
    printf("PID : %d, ", GetCurrentProcessId());
    for(i=0;i<4;i++)
        printf("num%d=%d, ", i+1, input[i]);
    printf("result= %d\n ", result);

    return 0;
}
void randomSleep()
{
    int waittime;

    srand(time(NULL));
    waittime = rand() % 4096;
    
    Sleep(waittime);
}
