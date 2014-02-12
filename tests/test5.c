// Test5
// yeethread_equal.

#include <stdio.h>
#include "yeethread.h"

void* worker(void* arg)
{
	int i;
	for(i = 0; i < 99999999; ++i);
}

int main()
{
	yeethread_t th1, th2;

	yeethread_init(1000);

	yeethread_create(&th1, worker, (void*)1);
	yeethread_create(&th2, worker, (void*)1);

	if(yeethread_equal(th1, th2)) {
		fprintf(stderr, 
				"!ERROR! They are different! %p, %p\n",
				th1, th2);
	}

	yeethread_join(th1, NULL);
	yeethread_join(th2, NULL);
	return 0;
}
