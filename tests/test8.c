// Test8
// Mutex. Protect a global variable.

#include <stdio.h>
#include "yeethread.h"

#define NUM_THREADS 100

yeethread_mutex_t g_mutex;
int g_num = 0;

void* worker ( void* arg )
{
    yeethread_mutex_lock ( &g_mutex );
    ++g_num;
    yeethread_mutex_unlock ( &g_mutex );
}

int main()
{
    int i;
    yeethread_t threads[NUM_THREADS];

    yeethread_init ( 1000 );

    yeethread_mutex_init ( &g_mutex );

    for ( i = 0; i < NUM_THREADS; ++i )
    {
        yeethread_create ( &threads[i], worker, ( void* ) i );
    }

    for ( i = 0; i < NUM_THREADS; ++i )
    {
        yeethread_join ( threads[i], NULL );
    }

    if ( g_num != NUM_THREADS )
    {
        fprintf ( stderr,
                  "!ERROR! Wrong result! %d != %d\n",
                  g_num, NUM_THREADS );
    }
    printf ( "yay\n" );
    return 0;
}
