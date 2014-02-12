// Test7
// yeethread_cancel. The program should terminate smoothly.

#include <stdio.h>
#include "yeethread.h"

void* worker2 ( void* arg )
{
    while ( 1 );
}

void* worker ( void* arg )
{
    yeethread_t th;

    yeethread_create ( &th, worker2, NULL );
    yeethread_cancel ( th );
    yeethread_join ( th, NULL );

    while ( 1 );
}

int main()
{
    yeethread_t th;

    yeethread_init ( 1000 );
    yeethread_create ( &th, worker, NULL );
    yeethread_cancel ( th );
    yeethread_join ( th, NULL );
    return 0;
}
