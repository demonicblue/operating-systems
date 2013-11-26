#include "queue.h"

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>


int testMultiThreaded(int N, int X);
void *my_thread(void *ptr);
int testSingleThread(int num);
int simpleTest();
