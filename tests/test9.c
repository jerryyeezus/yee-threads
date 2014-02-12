// Test9
// Mutex and yeethread_yield. The program should print like below.
//
// thread 1
// thread 1
// thread 1
// thread 2
// thread 2
// thread 2

#include <stdio.h>
#include "yeethread.h"

yeethread_mutex_t g_mutex;

void* worker(void* arg)
{
	int i;

	yeethread_mutex_lock(&g_mutex);
	for(i=0; i < 3; i++)
	{
		printf("thread %ld\n", (long)arg);
		yeethread_yield();
	}
	yeethread_mutex_unlock(&g_mutex);
}

int main()
{
	yeethread_t th1, th2;

	yeethread_init(1000);

	yeethread_mutex_init(&g_mutex);

	yeethread_create(&th1, worker, (void*)1);
	yeethread_create(&th2, worker, (void*)2);

	yeethread_join(th1, NULL);
	yeethread_join(th2, NULL);
	return 0;
}
