#include "queue.h"

Queue *queue;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;


void initialize_queue2(void){
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

void enqueue2(int val){
	//Create the new node and allocate memory for it.
	Node *new_tail;
	new_tail = malloc(sizeof(struct node));
	//Accessing critical section of adding the node to the queue.
	pthread_mutex_lock(&mutex1);
	new_tail->value=val;
	new_tail->next=NULL;

	//Make the old tail point to the new node.
	queue->tail->next = new_tail;
	//Set the tail pointing to the new tail.
	queue->tail = new_tail;
	pthread_mutex_unlock(&mutex1);
}

int dequeue2(int *extractedValue){
	pthread_mutex_lock(&mutex2);
	if(queue->head->next == NULL){
		pthread_mutex_unlock(&mutex2);
		return 0;
	}
	else{
		*extractedValue = queue->head->next->value;
		Node *temp = queue->head;
		queue->head = queue->head->next;
		pthread_mutex_unlock(&mutex2);
		free(temp);
		return 1;
	}
}
