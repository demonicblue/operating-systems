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

	printf("Total time: %.3f ms\n", (end-start)/100.0);

	return 0;
}

int testMultiThreaded2(int N)
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
		iret[i] = pthread_create( &threads[i], NULL, my_thread2, (void*) &num );
	}

	for (int i = 0; i < N; ++i)
	{
		pthread_join( threads[i], NULL );
	}

	gettimeofday(&now, NULL);
	end = now.tv_usec;

	iret[1]++; // To get rid of compiler warnings

	printf("Total time: %.3f ms\n", (end-start)/100.0);

	return 0;
}

void *my_thread(void *ptr)
{
	int num = *(int*)ptr;
	int ret = 0;
	int tot = 2*num;

	for(int i = 0; i< tot; i++)
	{
		if( (rand() % 2 + 1) > 1 ) {
			enqueue(i);
		} else {
			dequeue(&ret);
		}
	}
	/*
	for (int i = 0; i < num; ++i)
	{
		enqueue(i);
	}

	for (int i = 0; i < num; ++i)
	{
		dequeue(&ret);
		//printf("%d\n", ret);
	}*/
	return 0;
}

void *my_thread2(void *ptr)
{
	int num = *(int*)ptr;
	int ret = 0;
	int tot = 2*num;

	for(int i = 0; i< tot; i++)
	{
		if( (rand() % 2 + 1) > 1 ) {
			enqueue2(i);
		} else {
			dequeue2(&ret);
		}
	}

	/*
	for (int i = 0; i < num; ++i)
	{
		enqueue2(i);
	}

	for (int i = 0; i < num; ++i)
	{
		dequeue2(&ret);
		//printf("%d\n", ret);
	}*/
	return 0;
}


int main(int argc, char **argv)
{
	int N = 2;

	srand (time(NULL));

	if(argv[1])
	{
		N = atoi(argv[1]);
	}
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
	//printf("%d\n", N);
	printf("Using one lock\n");
	initialize_queue();

	for (int i = 0; i < 100; ++i)
	{
		enqueue(42);
	}

	testMultiThreaded(N);
	printf("Using two locks\n");

	initialize_queue2();

	for (int i = 0; i < 100; ++i)
	{
		enqueue2(42);
	}

	testMultiThreaded2(N);

	return 0;
}
