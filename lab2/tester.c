#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <assert.h>
#ifdef LOCKS
#include "concurrent_queue_2locks.c"
#else
#include "concurrent_queue.c"
#endif

#define MILL	1000000L
#define INIT_ELEMENTS 100


void *worker(void *start);
void *multipleEnqueues(void *args);
void *multipleDequeues(void *args);

typedef struct threadvals {
    int no_operations;
    int thread_id;
    void *extraArg;
} threadvals;


void spawner(int n, int x)
{
    pthread_t threads[n];
    
    int t = x / n;
    int i;
    
     printf("\t************************** Randomized operations ************************** \n\n");
    for(i = 0; i < INIT_ELEMENTS; i++)
       enqueue(i);
    
     printf("\tCreating thread: %d with %d random operations each\n", n,t);
    for(i = 0; i < n; i++)
    {
       
        pthread_create(&threads[i], NULL, worker, &t);
    }
    
    for(i = 0; i < n; i++)
        pthread_join(threads[i], NULL);
    
    while(dequeue(&i) != 1) //clear all memory, avoid leakages
        ;
   
     printf("\n\t************************** Completed Successfully ************************** \n\n");
}
void MPSC(int n, int x) //Multiple-Producer:Single-Consumer
{
    pthread_t enqueuers[n];
    threadvals vals[n];
    
    int no_operations = x / n;
    int i;
    int dequeue_val, index;
    int dequeued_vals[n];
    
    printf("\n\t*******************Multiple Producers, Single Consumer ******************* \n\n");
    printf("\tCreating consumers: %d with %d enqueue operations \n", n,no_operations);
    for(i = 0; i < n; i++)
    {
        
        vals[i].thread_id = i;
        vals[i].no_operations = no_operations;
        pthread_create(&enqueuers[i], NULL, multipleEnqueues, &vals[i]);
        
        dequeued_vals[i] = -1;
    }
    
    for(i = 0; i < n; i++)
        pthread_join(enqueuers[i], NULL);
    
    //Dequeue and verify FIFO sequential ordering as per each thread

    while (dequeue(&dequeue_val) != 1) {
        index = dequeue_val/no_operations;
#ifdef DEBUG
        printf("Dequeued %d index %d\n", dequeue_val, index);
#endif
        assert(dequeued_vals[index] < dequeue_val);
        dequeued_vals[index] = dequeue_val;
    }
  printf("\n\t************************** Completed Successfully ************************** \n\n");

    
}

void SPMC(int n, int x) //Single-Producer:Multiple-Consumer
{
    pthread_t dequeuers[n];
    threadvals vals[n];
    int results[x];
    
    int no_operations = x / n;
    int i,j;
    int dequeued_vals[n][no_operations];
    
    printf("\n\t*******************Single Producers, Multiple Consumer ******************* \n\n");
    printf("\tCreating producers: %d with %d enqueue operations \n", 1,x);
    
    for(i = 0; i < x ; i++){
        enqueue(i);
        results[i] = 0;
    }
    
    
    printf("\tCreating consumers: %d with %d dequeue operations each\n", n,no_operations);
    for(i = 0; i < n ; i++)
    {
        
        vals[i].thread_id = i;
        vals[i].no_operations = no_operations;
        vals[i].extraArg = dequeued_vals[i];
        pthread_create(&dequeuers[i], NULL, multipleDequeues, &vals[i]);
    }
  
    
    for(i = 0; i < n; i++)
        pthread_join(dequeuers[i], NULL);
   

    for(i = 0; i < x/n ; i++){
        for (j=0; j< n; j++) {
          
            
            assert(results[dequeued_vals[j][i]] == 0);
            results[dequeued_vals[j][i]] = dequeued_vals[j][i] + 1;
        }
        
    }
    
    // for(i = 0; i < x ; i++)
     //    printf("%d \n", results[i]);
    
    //Dequeue and verify FIFO sequential ordering as per each thread
       printf("\n\t************************** Completed Successfully ************************** \n\n");
    
    
}


int main(int argc, char* argv[]) {
  int  no_threads, no_operations;
  struct timeval start;
  struct timeval end;
  long diff;
    
    if(argc <= 2)
    {
        printf("Usage: %s <#threads> <#operations>\n", argv[0]);
        return 1;
    }
    
   no_threads = atoi(argv[1]);

   no_operations = atoi(argv[2]);
    
    assert( no_threads > 0 );
    assert( no_operations > 0 );
  
  initialize_queue();
    
 
     printf("\n\n");
    
    /**** TEST with random operations ***/
    gettimeofday(&start, NULL);
        spawner(no_threads, no_operations);
    gettimeofday(&end, NULL);


    diff = MILL * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    printf("\tElapsed %ld microseconds!\n", diff);
    
    
     /**** TEST FIFO ordering for each concurrent object ***/
    //gettimeofday(&start, NULL);
    MPSC(no_threads, no_operations);
    printf("\n\n");
    SPMC(no_threads, no_operations);
   // gettimeofday(&end, NULL);
    
   // diff = MILL * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
   // printf("\tElapsed %ld microseconds!\n", diff);

    printf("\n\n");
  exit(0);
  
}

void *worker(void *start) {
  int  threadlocal_oper, i;
  int val;
    srand(time(NULL));
  threadlocal_oper = *(int *) start;

  
  for (i=0; i<threadlocal_oper; i++) {
    if (rand()%2 == 0) {
        enqueue(i);
#ifdef DEBUG
        printf("Enqueued %d %d\n", i, (int)pthread_self());
#endif
    } else {
      if (dequeue(&val) == 0) {
#ifdef DEBUG
        printf("Dequeued %d %d\n", val, (int)pthread_self());
#endif
      }
    }
  }
    return NULL;
}

void *multipleEnqueues(void *args) {
    threadvals * thread_local = (threadvals *)args;
    int  threadlocal_oper, i;
    int  threadlocal_start;
    srand(time(NULL));
    threadlocal_oper = thread_local->no_operations;
    threadlocal_start = thread_local->thread_id * threadlocal_oper;
    
    
    for (i=0; i<threadlocal_oper; i++) {

        enqueue(threadlocal_start + i);
#ifdef DEBUG
        printf("Enqueued %4d %4d %d\n", threadlocal_start + i, thread_local->thread_id , (int)pthread_self());
#endif
        
    }
    return NULL;
}

void *multipleDequeues(void *args) {
    threadvals * thread_local = (threadvals *)args;
    int  threadlocal_oper, i;
    int val, *resultsArray;
    srand(time(NULL));
    threadlocal_oper = thread_local->no_operations;
    resultsArray = thread_local->extraArg;
    
    
    for (i=0; i<threadlocal_oper; i++) {
       
        if (dequeue(&val) == 0) {
            resultsArray[i] = val;
            if(i!=0)
                assert(resultsArray[i] > resultsArray[i-1]);
#ifdef DEBUG
            printf("Dequeued %4d %4d %d\n", resultsArray[i], i , (int)pthread_self());
#endif

        }else{
            printf("empty");
        }
        
    }
    return NULL;
}

