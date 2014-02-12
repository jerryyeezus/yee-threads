#include <stdio.h>
#include <stdlib.h>
#include "yeethread.h"

#define MAX_DURATION 5
#define NUM_CHOPSTICKS 5
#define NUM_PHILOSOPHERS 5
#define PERIOD 500L

typedef enum state
{
    THINKING, EATING, HUNGRY
} STATE;

yeethread_mutex_t chopsticks[NUM_CHOPSTICKS];
yeethread_t philosophers[NUM_PHILOSOPHERS];

void *philosophize ( void *arg )
{
    yeethread_mutex_t *first, *second;
    STATE state = THINKING;
    int which = * ( int * ) arg;
    int switched = 1;
    int turns = 0;
    int duration = 0;
    int first_index = 0;
    int second_index = 0;


    if ( ( which & 1 ) == 1 ) 						/* Odd philosopher */
    {
        first_index = which == 1 ?
                      NUM_CHOPSTICKS - 1 : which-2;
        second_index = which - 1;
    }
    else										/* Even philosopher */
    {
        first_index = which - 1;
        second_index = which - 2;
    }

    first = &chopsticks[first_index];
    second = &chopsticks[second_index];

    while ( 1 )
    {
        switch ( state )
        {
        case THINKING:
            if ( switched )
            {
                printf ( "Philosopher %d is thinking.\n", which );
                switched = 0;
                duration = rand() * MAX_DURATION;
                turns = 0;
            }
            else if ( turns > duration )
            {
                state = HUNGRY;
                switched = 1;
            }
            break;
        case EATING:
            if ( switched )
            {
                printf ( "Philosopher %d is eating.\n", which );
                switched = 0;
                duration = rand() * MAX_DURATION;
                turns = 0;
            }
            else if ( turns > duration )
            {
                printf ( "Philosopher %d releasing stick %d\n", which, second_index );
                yeethread_mutex_unlock ( second );
                printf ( "Philosopher %d releasing stick %d\n", which, first_index );
                yeethread_mutex_unlock ( first );
                state = THINKING;
                switched = 1;
            }
            break;

        case HUNGRY:
            printf ( "Philosopher %d is hungry.\n", which );
            if ( switched )
            {
                switched = 0;
                printf ( "Philosopher %d trying to pick up stick %d.\n", which, first_index );
                yeethread_mutex_lock ( first );
                printf ( "Philosopher %d now has stick %d.\n", which, first_index );
                printf ( "Philosopher %d trying to pick up stick %d.\n", which, second_index );
                yeethread_mutex_lock ( second );
                printf ( "Philosopher %d now has stick %d.\n", which, second_index );
                state = EATING;
                switched = 1;
            }
            break;
        }

        turns++;
    }
    return NULL;
}

int main ( int argc, const char *argv[] )
{
    int i;
    int args[NUM_PHILOSOPHERS];

    for ( i = 0; i < NUM_PHILOSOPHERS; i++ )
    {
        args[i] = i + 1;
    }

    yeethread_init ( PERIOD );

    for ( i = 0; i < NUM_CHOPSTICKS; i++ )
    {
        yeethread_mutex_init ( &chopsticks[i] );
    }

    for ( i = 0; i < NUM_PHILOSOPHERS; i++ )
    {
        yeethread_create ( &philosophers[i], philosophize, ( void* ) &args[i] );

    }
    while ( 1 );

    return 0;
}
