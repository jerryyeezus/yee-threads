// Test7
// gtthread_cancel. The program should terminate smoothly.

#include <stdio.h>
#include "gtthread.h"

void* worker2 ( void* arg )
{
    while ( 1 )
    {
        printf ( "worker2\n" );
    }
}

void* worker ( void* arg )
{
    gtthread_t th;

    gtthread_create ( &th, worker2, NULL );
    printf ( "foo\n" );
    gtthread_cancel ( th );
    printf ( "bar\n" );
    gtthread_join ( th, NULL );
    printf ( "baz\n" );

    while ( 1 );
}

int main()
{
    gtthread_t th;

    gtthread_init ( 1000 );
gtthread_join(444, NULL);
/*
    gtthread_create ( &th, worker, NULL );
    gtthread_cancel ( th );
    printf ( "try to join in main\n" );
    gtthread_join ( th, NULL );
    printf ( "succeed????\n" );
*/
    return 0;
}
