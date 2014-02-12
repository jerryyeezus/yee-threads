#ifndef __yeethread_H
#define __yeethread_H

typedef long yeethread_t;

/* Must be called before any of the below functions. Failure to do so may
 * result in undefined behavior. 'period' is the scheduling quantum (interval)
 * in microseconds (i.e., 1/1000000 sec.). */
void yeethread_init ( long period );
typedef struct yeethread_mutex
{
    yeethread_t owner;
    long lock;
} yeethread_mutex;


typedef yeethread_mutex yeethread_mutex_t;

/* see man pthread_create(3); the attr parameter is omitted, and this should
 * behave as if attr was NULL (i.e., default attributes) */
int  yeethread_create ( yeethread_t *thread,
                       void * ( *start_routine ) ( void * ),
                       void *arg );

/* see man pthread_join(3) */
int  yeethread_join ( yeethread_t thread, void **status );

/* yeethread_detach() does not need to be implemented; all threads should be
 * joinable */

/* see man pthread_exit(3) */
void yeethread_exit ( void *retval );

/* see man sched_yield(2) */
int yeethread_yield ( void );

/* see man pthread_equal(3) */
int  yeethread_equal ( yeethread_t t1, yeethread_t t2 );

/* see man pthread_cancel(3); but deferred cancelation does not need to be
 * implemented; all threads are canceled immediately */
int  yeethread_cancel ( yeethread_t thread );

/* see man pthread_self(3) */
yeethread_t yeethread_self ( void );


/* see man pthread_mutex(3); except init does not have the mutexattr parameter,
 * and should behave as if mutexattr is NULL (i.e., default attributes); also,
 * static initializers do not need to be implemented */
/*
int  yeethread_mutex_init ( yeethread_mutex_t *mutex );
int  yeethread_mutex_lock ( yeethread_mutex_t *mutex );
int  yeethread_mutex_unlock ( yeethread_mutex_t *mutex );
*/

int  yeethread_mutex_init ( yeethread_mutex *mutex );
int  yeethread_mutex_lock ( yeethread_mutex *mutex );
int  yeethread_mutex_unlock ( yeethread_mutex *mutex );

/* yeethread_mutex_destroy() and yeethread_mutex_trylock() do not need to be
 * implemented */

#endif
