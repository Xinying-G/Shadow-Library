#define _GNU_SOURCE
#include <stdio.h> 
#include <malloc.h>
#include <sys/types.h>
#include <unistd.h>
#include <dlfcn.h>
#include <signal.h> 

/*
 * Run-time interposition of malloc and free based 
 * on the dynamic linker's (ld-linux.so) LD_PRELOAD mechanism
 * 
 * Example (Assume a.out calls malloc and free):
 *   linux> gcc -shared -fPIC shadow.c -o shadow.so -ldl
 *   tcsh> gcc -g -o watch watch.c -lpthread -lrt -lm
 *   tcsh>  ./watch ./a.out 
 *   
 *   tcsh> setenv LD_PRELOAD "./shadow.so"
 *   tcsh> ./a.out
 *   tcsh> unsetenv LD_PRELOAD
 */

void *malloc(size_t size)
{
    static int counter;
    counter = counter + size;
    if(counter > 4*1024*1024){
    	fprintf(stderr, "Heap Memory occupies more than 4MB\n");   
    	raise(9);
    }

    static void *(*mallocp)(size_t size);
    char *error;
    void *ptr;

     // get address of libc malloc 
    if (!mallocp) {
	mallocp = dlsym(RTLD_NEXT, "malloc");
	if ((error = dlerror()) != NULL) {
	    fputs(error, stderr);
	    return NULL;
	}
    }
    ptr = mallocp(size);
    return ptr;
}

pid_t fork(void)
{	
	fprintf(stderr, "Can not fork in child \n");
	//exit(9);
	kill(getpid(),9);
	//pipe
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg)
{
	fprintf(stderr, "Can not create thread in child\n");
	//kill(getpid(),9);
	raise(9);
}



FILE *fopen(const char *pathname, const char *mode)
{
	fprintf(stderr, "Can not open file in child\n");
	kill(getpid(),9);
}









