#include "tester.h"

#define X 10000


int testMultiThreaded(int N)
{
	struct timeval now;
	pthread_t threads[N];
	int iret[N];
	int num = X/N;
	int start, end;

	gettimeofday(&now, NULL);
	start = now.tv_usec;
	

	for (int i = 0; i < N; ++i)
	{
		iret[i] = pthread_create( &threads[i], NULL, my_thread, (void*) &num );
	}

	for (int i = 0; i < N; ++i)
	{
		pthread_join( threads[i], NULL );
	}

	gettimeofday(&now, NULL);
	end = now.tv_usec;

	iret[1]++; // To get rid of compiler warnings

	printf("Total time: %d\n", end-start);

	return 0;
}

void *my_thread(void *ptr)
{
	int num = *(int*)ptr;
	int ret = 0;

	for (int i = 0; i < num; ++i)
	{
		enqueue(i);
	}

	for (int i = 0; i < num; ++i)
	{
		dequeue(&ret);
		//printf("%d\n", ret);
	}
	return 0;
}

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
	//simpleTest();
	initialize_queue();

	for (int i = 0; i < 100; ++i)
	{
		enqueue(42);
	}

	testMultiThreaded(2);
	
	return 0;
}