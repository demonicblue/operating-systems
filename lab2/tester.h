#include "queue.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>


int testMultiThreaded(int N);
int testMultiThreaded2(int N);
void *my_thread(void *ptr);
void *my_thread2(void *ptr);
int testSingleThread(int num);
int simpleTest();
