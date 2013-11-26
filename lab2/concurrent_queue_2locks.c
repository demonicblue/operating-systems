#include "queue.h"

Queue *queue;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void initialize_queue(void){

	printf("Using two locks\n");

	//Initialize dummy node
	Node *dummy;
	dummy = malloc(sizeof(struct node));
	dummy->value=0;
	dummy->next=NULL;
	//Initialize queue
	queue = malloc(sizeof(struct queue));
	queue->head=dummy;
	queue->tail=dummy;
	//Initialize mutex variable
	mutex1 = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	mutex2 = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
}

void enqueue(int val){
	pthread_mutex_lock(&mutex1);
	Node *new_tail;
	new_tail = malloc(sizeof(struct node));
	new_tail->value=val;
	new_tail->next=NULL;

	queue->tail->next = new_tail;
	queue->tail = new_tail;
	pthread_mutex_unlock(&mutex1);
}

int dequeue(int *extractedValue){
	pthread_mutex_lock(&mutex2);
	if(queue->head->next == NULL){
		pthread_mutex_unlock(&mutex2);
		return 0;
	}
	else{
		*extractedValue = queue->head->next->value;
		Node *temp = queue->head;
		queue->head = queue->head->next;
		free(temp);
		pthread_mutex_unlock(&mutex2);
		return 1;
	}
}
