/* invoking and communicating with a thread */ 
#include <stdio.h> 
#include <string.h> 
#include <pthread.h> 

void *threaded_routine (void * v) { 
    sleep(5); 
    fprintf(stderr, "oink!\n"); 
    return NULL;  
} 

main()
{ 
   pthread_t thread; 
   void *retptr; 
   if (pthread_create(&thread, NULL, threaded_routine, (void *)NULL)==0) { 
	pthread_join(thread,(void **)&retptr); 
   } else { 
	fprintf(stderr, "could not create thread!\n"); 
   } 
} 
