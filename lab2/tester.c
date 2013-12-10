#ifdef LOCKS
#include "concurrent_queue_2locks.c"
#else
#include "concurrent_queue.c"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>

#define X 10000

int testMultiThreaded(int N);
void *my_thread(void *ptr);

/*
* Sets up and spawns N threads, then wait for them to finnish and time it.
*/
int testMultiThreaded(int N)
{
	struct timeval now; /* Helper struct for getting current time */
	pthread_t threads[N];
	int num = X/N; /* Divide operations per thread */
	int start, end;

	gettimeofday(&now, NULL);
	start = now.tv_usec;

	for (int i = 0; i < N; ++i)
	{
		/* Create N threads */
		pthread_create( &threads[i], NULL, my_thread, (void*) &num );
	}

	for (int i = 0; i < N; ++i)
	{
		/* Wait for N threads to finnish */
		pthread_join( threads[i], NULL );
	}

	gettimeofday(&now, NULL);
	end = now.tv_usec;

	printf("Total time: %.3f ms\n", (end-start)/100.0);

	return 0;
}

/*
* Pthreads function. Randomly enques and deques specified number of elements.
*/
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
	return 0;
}

/*
* The program takes one parameter which decides how many threads
* will be spawned. If no parameter is provided; it defaults to 2.
*
* Example (to spawn 4 threads):
*   ./tester1lock 4
*/
int main(int argc, char **argv)
{
	int N = 2;

	srand (time(NULL));

	/* Interprett number of threads to use */
	if(argv[1])
	{
		N = atoi(argv[1]);
	}

	initialize_queue();

	/* Enqueu some elements as a starting point */
	for (int i = 0; i < 100; ++i)
	{
		enqueue(42);
	}

	testMultiThreaded(N);
	
	return 0;
}
