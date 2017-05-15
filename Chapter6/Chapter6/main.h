#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "queue.h"
#include "semaphore.h"
#include "mutex.h"
#define THREADS 8
//**inter-threads globals
int n;
int pro;
LONG count;
LONG in;
LONG out = 0;
LONG avgUsing = 0;
int ret_min = INT_MAX;
int ret_max = INT_MIN;
long double ret_avg;
long double ret_std;
long double delta;
Queue* intQueue;
//*******//
int initSemaphores();
int getMin(int);
int getMax(int);
int getAvg(int);
int getStDev(int);

