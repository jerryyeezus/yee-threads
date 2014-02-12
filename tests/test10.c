// Test10
// Producer and consumer.

#include <stdio.h>
#include <time.h>
#include "yeethread.h"

#define LOOP 100

yeethread_t g_th1, g_th2;
yeethread_mutex_t g_mutex;
int g_num = 0;

void* producer ( void* arg )
{
    int i, j;

    for ( i=0; i < LOOP; i++ )
    {
        yeethread_mutex_lock ( &g_mutex );
        ++g_num;
        yeethread_mutex_unlock ( &g_mutex );

        for ( j=0; j < rand() % 999999; ++j );
    }
}

void* consumer ( void* arg )
{
    int i, j;

    for ( i=0; i < LOOP; i++ )
    {
        yeethread_mutex_lock ( &g_mutex );
        --g_num;
        yeethread_mutex_unlock ( &g_mutex );

        for ( j=0; j < rand() % 999999; ++j );
    }
}

int main()
{
    yeethread_t th1, th2;

    srand ( time ( NULL ) );

    yeethread_init ( 1000 );

    yeethread_mutex_init ( &g_mutex );

    yeethread_create ( &th1, producer, NULL );
    yeethread_create ( &th2, consumer, NULL );

    yeethread_join ( th1, NULL );
    yeethread_join ( th2, NULL );

    if ( g_num != 0 )
    {
        fprintf ( stderr, "!ERROR! Wrong result! %d != 0\n", g_num );
    }
    printf ( "yay\n" );
    return 0;
}
