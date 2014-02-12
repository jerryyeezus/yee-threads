#Yee-Threads (a user-space threading library)
Author: Jerry Yee

#Platform

Linux 12.04 64-bit

#API

Yeethreads provides the following API for thread and mutex operations:

* void yeethread_init(long period);
* int  yeethread_create(yeethread_t *thread,
                     void *(*start_routine)(void *),
                     void *arg);
* int  yeethread_join(yeethread_t thread, void **status);
* void yeethread_exit(void *retval);
* int  yeethread_yield(void);
* int  yeethread_equal(yeethread_t t1, yeethread_t t2);
* int  yeethread_cancel(yeethread_t thread);
* yeethread_t yeethread_self(void);
* int  yeethread_mutex_init(yeethread_mutex_t *mutex);
* int  yeethread_mutex_lock(yeethread_mutex_t *mutex);
* int  yeethread_mutex_unlock(yeethread_mutex_t *mutex);

######Behavior of function yeethread_* is synononmous with pthread_* with the exception of yeethread_init, which does not exist for pthreads. yeethread_init() takes a long integer as input which will be used as the round-robin scheduling period.

#Compiliation

To compile the yeethreads.a library file, simply type

```
  make
```

#Usage

Simply #include "yeethreads.h" in your file.
  
```
  gcc <your_file> yeethreads.a
```

#Dining Philosophers Example

To run, use 

```
./dining
```


