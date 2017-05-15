#include "semaphore.h"
int test_and_set() {

}
int wait(LONG* sem) {
	while (sem <= 0) Sleep(10);
	InterlockedDecrement(sem);
	return 0;
}
int signal(LONG* sem) {
	InterlockedIncrement(sem);
	return 0;
}
