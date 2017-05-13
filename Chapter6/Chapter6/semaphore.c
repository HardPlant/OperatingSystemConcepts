#include "semaphore.h"

int wait(int* smp_S) {
	while (smp_S <= 0);
	(*smp_S)--;
}
int signal(int* smp_S) {
	(*smp_S)++;
}
