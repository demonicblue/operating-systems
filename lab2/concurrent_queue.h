#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
typedef struct node{
	int value;
	struct node *next;
} Node;

typedef struct queue{
	Node *head;
	Node *tail;
} Queue;

void initialize_queue(void);
void enqueue(int val);
int dequeue(int *extractedValue);

