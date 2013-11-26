typedef struct node{
	int value;
	struct node *next;
} Node;

typedef struct queue{
	Node *head;
	Node *tail;
} Queue;

void inititalize_queue(void);
void enqueue(int val);
int dequeue(int *extractedValue);