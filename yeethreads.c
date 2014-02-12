#include <ucontext.h>
#include <asm/errno.h>
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include "yeethread.h"
#include "list.h"

#define ST_SIZE 8192
#define SIG_FINISHED 42
#define THREAD_FINISHED 3
#define THREAD_YIELD 4
#define yeethread_CANCELLED -1
#define MUTEX_NO_OWNER -1

typedef struct yeethread
{
    yeethread_t tid;
    ucontext_t ctx;
    void ( *start_routine ) ();
    void *retVal;
    int state;
} yeethread;

node *current_node;
long sched_period;
list *threads;
list *deads;
yeethread_t tid;

void context_handler ( int sig );
void start_timer();

void yeethread_exit ( void *retVal )
{
    sigset_t blockSet;
    yeethread *current_thread, *removed_thread;
    node *new_ptr;

    sigprocmask ( SIG_BLOCK, &blockSet, NULL );
    current_thread = ( yeethread * ) current_node->data;
    current_thread->retVal = retVal;

    new_ptr = current_node->next;
    removed_thread = ( yeethread * ) list_remove ( threads, current_node );
    if ( threads->size == 0 )
    {
        exit ( 1 );
    }
    current_node = new_ptr;
    list_add ( deads, removed_thread );
    sigprocmask ( SIG_UNBLOCK, &blockSet, NULL );

    context_handler ( SIG_FINISHED );
}

/* TODO retval*/
void _create ( void * ( *start_routine ) ( void* ), void *arg )
{
    sigset_t blockSet;
    node *new_ptr;
    yeethread *removed_thread;

    /* Execute start routine */
    yeethread *current_thread = ( yeethread * ) current_node->data;
    void *ret = start_routine ( arg );

    /* Routine done executing now... */

    /* Remove from threads and add to deads. Then, increment current_node. */
    sigprocmask ( SIG_BLOCK, &blockSet, NULL );

    current_thread->retVal = ret;
    new_ptr = current_node->next;
    removed_thread = ( yeethread * ) list_remove ( threads, current_node );
    if ( threads->size == 0 )
    {
        exit ( 1 );
    }
    current_node = new_ptr;
    list_add ( deads, removed_thread );

    sigprocmask ( SIG_UNBLOCK, &blockSet, NULL );

    context_handler ( SIG_FINISHED );
}

node *get_node ( list *it, yeethread_t thread )
{
    int i = 0;
    int size = it->size;
    node *node_ptr = it->head;
    yeethread *thread_ptr;

    while ( i++ < size )
    {
        thread_ptr = ( yeethread * ) node_ptr->data;
        if ( thread_ptr->tid == thread )
        {
            return node_ptr;
        }

        node_ptr = node_ptr->next;
    }

    return NULL;
}

yeethread *get_thread ( list *it, yeethread_t thread )
{
    int i = 0;
    int size = it->size;
    node *node_ptr = it->head;
    yeethread *thread_ptr;

    while ( i++ < size )
    {
        thread_ptr = ( yeethread * ) node_ptr->data;
        if ( thread_ptr->tid == thread )
        {
            return thread_ptr;
        }

        node_ptr = node_ptr->next;
    }

    return NULL;
}

int yeethread_yield()
{
    context_handler ( SIGPROF );
    return 0;
}

int yeethread_join ( yeethread_t thread, void **status )
{
    yeethread *target;
    if ( get_thread ( deads, thread ) == NULL && get_thread ( threads, thread ) == NULL )
    {
        return EINVAL;
    }

    if ( get_thread ( deads, thread ) != NULL )
    {
        if ( status )
        {
            target = get_thread ( deads, thread );
            *status = target->retVal;
        }
        return 0;
    }

    while ( get_thread ( deads, thread ) == NULL )
    {
        yeethread_yield();
    }

    if ( status )
    {
        target = get_thread ( deads, thread );
        *status = target->retVal;
    }

    return 0;
}

void start_timer()
{
    struct itimerval it;
    struct sigaction act, oact;

    act.sa_handler = context_handler;
    sigemptyset ( &act.sa_mask );
    act.sa_flags = 0;
    sigaction ( SIGPROF, &act, &oact );

    it.it_interval.tv_sec = 0;
    it.it_interval.tv_usec = sched_period;

    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = sched_period;
    setitimer ( ITIMER_PROF, &it, NULL );
}

void context_handler ( int sig )
{
    sigset_t blockSet;
    yeethread *prev_thread, *current_thread;

    sigprocmask ( SIG_BLOCK, &blockSet, NULL );
    if ( sig == SIGPROF )
    {
        prev_thread = ( yeethread * ) current_node->data;
        current_node = current_node->next;
        current_thread = ( yeethread * ) current_node->data;
        sigprocmask ( SIG_UNBLOCK, &blockSet, NULL );
        start_timer();
        swapcontext ( &prev_thread->ctx, &current_thread->ctx );
    }
    else if ( sig == SIG_FINISHED )
    {
        current_thread = ( yeethread * ) current_node->data;
        sigprocmask ( SIG_UNBLOCK, &blockSet, NULL );
        start_timer();
        setcontext ( &current_thread->ctx );
    }
}

void yeethread_init ( long period )
{
    ucontext_t newCtx;
    yeethread *newThread;
    sched_period = period;

    threads = list_create();
    deads = list_create();
    getcontext ( &newCtx );

    newThread = malloc ( sizeof ( yeethread ) );
    newThread->tid = 0;
    newThread->ctx = newCtx;
    newThread->state = 1;
    list_add ( threads, newThread );
    current_node = threads->head;
}

int yeethread_create ( yeethread_t *thread, void * ( *start_routine ) ( void* ), void *arg )
{
    ucontext_t newCtx;
    yeethread *newThread;
    sigset_t blockSet;
    getcontext ( &newCtx );
    sigprocmask ( SIG_BLOCK, &blockSet, NULL );
    newCtx.uc_stack.ss_sp = malloc ( ST_SIZE );
    newCtx.uc_stack.ss_size = ST_SIZE;

    tid++;
    *thread = tid;

    makecontext ( &newCtx, ( void ( * ) ( void ) ) _create, 2, start_routine, arg );

    newThread = malloc ( sizeof ( yeethread ) );
    newThread->tid = tid;
    newThread->ctx = newCtx;
    newThread->state = 1;

    if ( threads->size < 2 )
    {
        /*current_node = threads->tail; */
        start_timer();
    }

    list_add ( threads, newThread );
    sigprocmask ( SIG_UNBLOCK, &blockSet, NULL );

    return 0;
}

yeethread_t yeethread_self ( void )
{
    yeethread *current_thread = ( yeethread * ) current_node->data;
    return current_thread->tid;
}

int  yeethread_cancel ( yeethread_t thread )
{
    node *removed_node = get_node ( threads, thread );
    yeethread *removed_thread = ( yeethread * ) list_remove ( threads, removed_node );
    list_add ( deads, removed_thread );

    return 0;
}

int  yeethread_equal ( yeethread_t t1, yeethread_t t2 )
{
    return t1 == t2;
}

int  yeethread_mutex_init ( yeethread_mutex *mutex )
{
    if ( mutex->lock == 1 ) return -1;
    mutex->lock = 0;
    mutex->owner = -1;
    return 0;
}


int yeethread_mutex_lock ( yeethread_mutex *mutex )
{
    yeethread *current_thread = ( yeethread * ) current_node;
    long curr_tid = current_thread->tid;

    while ( mutex->lock != 0 )
        yeethread_yield();

    mutex->owner = curr_tid;
    mutex->lock = 1;
    return 0;
}

int yeethread_mutex_unlock ( yeethread_mutex *mutex )
{
    yeethread *current_thread = ( yeethread * ) current_node;

    if ( mutex->lock == 1 && mutex->owner == current_thread->tid )
    {
        mutex->owner = MUTEX_NO_OWNER;
        mutex->lock = 0;
        return 0;
    }

    return -1;
}
