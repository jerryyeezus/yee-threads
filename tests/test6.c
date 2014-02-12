// Test6
// yeethread_self. The program should print out liek below.
//
// same or diff
// diff    same

#include <stdio.h>
#include "yeethread.h"

yeethread_t g_th1, g_th2;

void* worker ( void* arg )
{
    yeethread_t tid = yeethread_self();

    if ( yeethread_equal ( g_th2, tid ) )
        printf ( "same\n" );
    else
        printf ( "diff\n" );
}

int main()
{
    int rc;

    yeethread_init ( 1000 );

    rc = yeethread_create ( &g_th1, worker, NULL );
    rc = yeethread_create ( &g_th2, worker, NULL );

    yeethread_join ( g_th1, NULL );
    yeethread_join ( g_th2, NULL );
    return 0;
}
