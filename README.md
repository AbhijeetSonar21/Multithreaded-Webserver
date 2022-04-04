# Multithreaded-Webserver

In code I’ve successfully implemented Multi-Threaded web server with synchronization.
First of all I created a buffer with size of Max request plus one, to store all requests.
Then created a thread model. In which I initialized pthread_t myThreads with size of Number of
threads plus one. Then in for loop I created a threads. In which first thread has to be a producer
so in for loop I created first thread as producer and called ‘listener’ function in it. And rest all are
consumer threads in which I called ‘consumer’ function. Later in different for loop I joined then
all.

In listener function, in while loop after ‘accept’ call I stored file descriptor in buffer. Which is
basically a producer model. And it is also a critical section. So for handling deadlocks and
mutual exclusion I created two semaphores and one mutex lock. So in listener function whenever
we are putting file descriptor which is variable name ‘s’ in buffer so to handle this critical section
I first added semaphore “`sem_wait(&sem_empty)`” which is initialized in main function with
Max request which tells that whether buffer is full or not. If its full then producer should not
write or add more request in buffer. And then I added pthread_mutex_lock for handling mutual
exclusion of variable ‘n’. So that whenever we are putting file descriptor in buffer and
incrementing ‘n’ value it should not cause race condition. After successfully adding request in
buffer, I unlocked mutex lock and added “`sem_post(&sem_full)`” which is initialized as 0 in
main function. So basically, it tells that it added a request in buffer. So consumer can access it.
In consumer function, I created a for loop for handling all requests from buffer. For example, if
there are 4 threads and 100 requests then after completion of first four requests threads should
complete next requests and so on. Later I added a semaphore “`sem_wait(&sem_full)`” which will
check whether there are elements in buffer or not and if buffer is empty then do not enter in
critical section and then there is one mutex lock for avoiding race condition. So in consumer
function it takes requests from buffer and then after removing mutex lock and adding
“`sem_post(&sem_empty)`” which will tell that request has been collected and now buffer is
available so that producer can add request in it, I passed file descriptor/request to process
function and it executes that request.

So in this manner I handled deadlocks and mutual exclusion in program. And throughout the
program after testing multiple test cases program is never going in deadlock situation and mutual
exclusion is perfectly achieved. When I passed 100 requests with 4 threads or similar cases then
there’s not a single situation where there is race condition or deadlock taking place at any point
of time and buffer and its index is completely protected from race condition and any sort of
deadlock.
