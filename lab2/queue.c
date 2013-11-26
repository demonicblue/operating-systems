#include <queue.h>

void initialize_queue(void){
	//Initialize dummy node
	Node dummy;
	dummy.value=0;
	dummy.next=NULL;
	//Initialize queue
	queue.head=dummy;
	queue.tail=dummy;

}

void enqueue(int val){
	//TODO Need locks
	Node new_tail;
	new_tail.value=val;
	new_tail.next=NULL;

	queue.tail->next = new_tail;
	queue.tail=new_tail;
}

int dequeue(int *extractedValue){
	//TODO need locks

	if(queue.head->next == NULL){
		return 0;
	}
	else{
		*extractedValue = queue.head->next.value;
		queue.head = queue.head->next;
		return 1
	}
}