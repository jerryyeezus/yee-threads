// Test3
// Main exit. Program must terminate smoothly.

#include <stdio.h>
#include "yeethread.h"

void* worker(void* arg)
{
	for(;;);
}

int main()
{
	yeethread_t th1;

	yeethread_init(1000);

	yeethread_create(&th1, worker, NULL);

	printf("main exit\n");

	yeethread_exit(NULL);
	return 0;
}
