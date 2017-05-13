#pragma once
#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE 20000
typedef int ElementType;
typedef struct {
	int index;
	int queue[MAXSIZE];
	int size;
}Queue;

Queue* makeQueue(int size);
int isFull(Queue* dest);
int isEmpty(Queue* dest);
int enqueue(Queue* dest, ElementType data);
ElementType peek(Queue* dest);
void destroy(Queue* dest);
ElementType dequeue(Queue* dest);
void printQueue(Queue* dest);
ElementType* at(Queue* dest, int index);