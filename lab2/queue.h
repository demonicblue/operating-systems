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

void initialize_queue2(void);
void enqueue2(int val);
int dequeue2(int *extractedValue);
