#Yee-Threads (a user-space threading library)
Jerry Yee

#Platform

Linux 12.04 64-bit

#Preemptive Scheduler

There is a global variable that stores the scheduling period called sched-period. On init(), it initializes a doubly linked list containing the gtthreads struct. The struct members for gtthreads is the context pointer, thread ID, routine, and retVal. 

A function called start-timer() is called whenever starting or restarting the SIGPROF timer. The handler called context-handler() is triggered whenever the timer expires or whenever there is a yield or thread return. The two possible input signals (SIGPROF or THREADDEAD) reflect these cases.

This function restarts the timer and updates the current-node global variable to point to the next thread's struct. Finally, it calls swap context on the previous context and the new context, respectively. 

#Compiliation

make clean
make
gcc <your_file> gtthreads.a

#Dining Philosophers

To run, use ./dining

Order the philosophers 1-5.

Make it so odd philosophers pick up left chopstick first and then right. For even philosophers, pick up right first then left. This ensures that at *worst*, there is at least one philosopher actually eating. When he finishes, he will release his sticks and let others have at it.

#Final Thoughts
^_^

