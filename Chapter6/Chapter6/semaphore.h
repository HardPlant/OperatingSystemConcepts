#ifndef  SEMAPHORE_H
#define SEMAPHORE_H
#include <windows.h>
int wait(LONG* sem);
int signal(LONG* sem);

#endif // ! SEMPAHORE_H
