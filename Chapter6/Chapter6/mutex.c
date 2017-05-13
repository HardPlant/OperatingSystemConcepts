#include "mutex.h"

void acquire(BOOL i) {
	while (!i);
	i = FALSE;
}
void release(BOOL i) {
	i = TRUE;
}