// Test7
// yeethread_cancel. The program should terminate smoothly.

#include <stdio.h>
#include "yeethread.h"

void* worker2 ( void* arg )
{
    while ( 1 )
    {
        printf ( "worker2\n" );
    }
}

void* worker ( void* arg )
{
    yeethread_t th;

    yeethread_create ( &th, worker2, NULL );
    printf ( "foo\n" );
    yeethread_cancel ( th );
    printf ( "bar\n" );
    yeethread_join ( th, NULL );
    printf ( "baz\n" );

    while ( 1 );
}

int main()
{
    yeethread_t th;

    yeethread_init ( 1000 );
yeethread_join(444, NULL);
/*
    yeethread_create ( &th, worker, NULL );
    yeethread_cancel ( th );
    printf ( "try to join in main\n" );
    yeethread_join ( th, NULL );
    printf ( "succeed????\n" );
*/
    return 0;
}
