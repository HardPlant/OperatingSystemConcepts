#include "queue.h"
#include <stdio.h>
void error(char* msg)
{
	fprintf(stderr, msg);
}
Queue* makeQueue(int size)
{
	Queue* toMake = (Queue*)malloc(sizeof(Queue));
	toMake->index = 0;
	toMake->size = size;
	return toMake;
}
int isEmpty(Queue* dest)
{
	if (dest->index <=0) return 1;
	else return 0;
}
int isFull(Queue* dest)
{
	if (dest->index > dest->size) return 1;
	else return 0;
}
int enqueue(Queue* dest, ElementType data)
{
	
	if (isFull(dest))
	{
		error("enqueue: 큐 가득 참\n");
		return 0;
	}
	else
	{
		dest->queue[dest->index++]= data;
	}
	return 1;
}
ElementType peek(Queue* dest)
{
	if (isEmpty(dest))
	{
		error("peek: 큐가 비어 있음");
		return -1;
	}
	return dest->queue[0];
}
ElementType dequeue(Queue* dest)
{
	ElementType result;
	int i;
	if (isEmpty(dest))
		error("dequeue: 큐가 비어 있음");
	else
	{
		result = dest->queue[0];
		for (i = 0; i < dest->index-1 ; i++) {
			dest->queue[i] = dest->queue[i + 1];
		}
		dest->index--;
		return result;
	}
}
void destroy(Queue* dest)
{
	free(dest);
}
void printQueue(Queue* dest)
{
	int i = 0;
	if (isEmpty(dest));
	ElementType* current = dest->queue;
	for(i=0;i<dest->index;i++)
	{
		printf("%d ", dest->queue[i]);
	}
	printf("\n");
}
ElementType* at(Queue* dest, int index)
{
	if (isEmpty(dest))
	{
		error("at: 빈 큐");
		return NULL;
	}
	if (index > dest->index + 1)
	{
		error("at: 큐 범위 초과");
		return NULL;
	}
	return &(dest->queue[index]);
}