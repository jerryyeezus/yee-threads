// Test4
// Thread create chaining.
// The program should print out liek below when CHAIN_LEN is 20.
//
// worker1 starts
// worker2 starts
// ...
// worker20 starts
// worker20 ends
// worker19 ends
// ...
// worker 1 ends

#include <stdio.h>
#include "yeethread.h"

#define CHAIN_LEN 20

void* worker(void* arg)
{
	yeethread_t th;

	long id = (long) arg;
	printf("worker%ld starts\n", id);
	if (id < CHAIN_LEN) {
		yeethread_create(&th, worker, (void*) (id + 1));
		yeethread_join(th, NULL);
	}
	printf("worker%ld ends\n", id);
}

int main()
{
	yeethread_t th;

	yeethread_init(1000);
	yeethread_create(&th, worker, (void*) 1);
	yeethread_join(th, NULL);
	return 0;
}
