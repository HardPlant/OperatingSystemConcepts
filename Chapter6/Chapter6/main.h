#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "queue.h"
#include "semaphore.h"
#define THREADS 8
//**inter-threads globals
int n;
int pro;
LONG count = 0;
LONG in = 0;
LONG out = 0;
int ret_min = INT_MAX;
int ret_max = INT_MIN;
long double ret_avg;
long double ret_std;
long double delta;
Queue* intQueue;
//*******//
int getMin(int*, int*);
int getMax(int*, int*);
int getAvg(int*, int*);
int getStDev(int*, int*);

