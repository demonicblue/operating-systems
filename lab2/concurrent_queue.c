#include "concurrent_queue.h"

Queue *queue;
pthread_mutex_t mutex1;

void initialize_queue(void){

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
}

void enqueue(int val){
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

int dequeue(int *extractedValue){
	pthread_mutex_lock(&mutex1);
	if(queue->head->next == NULL){
		pthread_mutex_unlock(&mutex1);
		return 0;
	}
	else{
        //Extract the value
        *extractedValue = queue->head->next->value;
        //Get a temporary pointer to the old dummy node.
        Node *temp = queue->head;
        //Assign the new dummy node
        queue->head = queue->head->next;
        pthread_mutex_unlock(&mutex1);
        //Free the memory of the old dummy node.
        free(temp);
		return 1;
	}
}
