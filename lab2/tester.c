#include "queue.h"

#include <stdio.h>


int testSingleThread(int num)
{
	int returned, success = 1;

	initialize_queue();

	for(int i = 0; i<num; i++)
	{
		enqueue(i);
	}

	for(int i = 0; i<num; i++)
	{
		dequeue(&returned);
		if(returned != i)
		{
			success = 0;
		}
	}

	return success;
}

int simpleTest()
{
	int ret = 55;

	initialize_queue();

	enqueue(42);
	enqueue(43);

	dequeue(&ret);
	printf("Ret:%d\n", ret);

	dequeue(&ret);
	printf("Ret:%d\n", ret);

	return 0;

}

int main(void)
{
	/*printf("%s\n", "Running single thread test..");
	if(testSingleThread(100))
	{
		printf("%s\n", "Single thread: Success!");
	}
	else
	{
		printf("%s\n", "Single thread: Failed!");
	}*/
	simpleTest();
	
	return 0;
}