/*
* How to compile and run program: 
* linux> gcc -shared -fPIC shadow.c -o shadow.so -ldl
* linux> gcc -g -o watch watch.c -lpthread -lrt
* tcsh>  ./watch ./a.out 
* for set limit size of stack memory(1.out), I used setrlimit. 
* for the rest(2.out, 3.out, 4.out,5.out), I used shadow library. 
* I didn't do the limit of global variables(6.out).  
*/



/* trapping bus errors */ 
#define _GNU_SOURCE
#include <stdio.h> 
#include <stdlib.h> 
#include <signal.h> 
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <time.h> 
#include <sys/resource.h>
#include <sys/wait.h>
#include <string.h>  
#include "/comp/111/a/a3/proc.c"

#define FALSE 0
#define TRUE  1

int done = FALSE;
struct rlimit rl;
pid_t pidc; 
char inbuf[256];
int p[2];
struct timespec start, finish;
struct rusage buf1,buf2; 

void reaper(int sig) { 

    clock_gettime(CLOCK_REALTIME, &finish);
    getrusage(RUSAGE_SELF, &buf2);
    
    done=TRUE;
    int status;
    pid_t pid2 = waitpid(pidc,&status,0);


    close(p[1]);
    int count = 0;
    FILE *read = fdopen(p[0],"r");
    int a;
    while(fgets(inbuf,25,read)!= NULL && inbuf[0] != '\n'){
        count++;
    }

    fclose(read); 
    fprintf(stderr, "stdout count: %d\n", count);

    if(WIFEXITED(status)){
      if(WEXITSTATUS(status) == 11)
        fprintf(stderr, "Can not use stack memory more than 4MB\n");
      fprintf(stderr, "Child %d terminated narmally with status: %d\n",pid2,WEXITSTATUS(status));
    }
    else if(WIFSIGNALED(status)){
            if(WTERMSIG(status) == 11)
        fprintf(stderr, "Stack Memory occupies more than 4 MB\n");
      fprintf(stderr, "Child %d terminated abnarmally with status: %d\n",pid2,WTERMSIG(status));
    }
} 

main(int argc, char *argv[])
{ 
    int status; 
    signal(SIGCHLD,reaper);

    if(pipe(p) < 0)
      fprintf(stderr, "Pipe failed\n");

    if ((pidc=fork())) {    // parent

    } else {            // child

        close(1);
        dup(p[1]);
        close(p[1]);
        close(p[0]);

        rl.rlim_cur = 4*1024*1024;
        rl.rlim_max = 4*1024*1024;
        if(setrlimit (RLIMIT_STACK, &rl) == -1)
            fprintf(stderr,"RLIMIT_STACK failed\n");
        else{
            getrlimit (RLIMIT_STACK, &rl);  
        }
        char *env[] = {"LD_PRELOAD=./shadow.so", NULL };
        char *args[] = {argv[1],NULL};
        execvpe(argv[1], args, env);
    } 
    
    clock_gettime(CLOCK_REALTIME, &start);
    getrusage(RUSAGE_SELF, &buf1);
    while (!done) {} 


    long seconds = finish.tv_sec - start.tv_sec; 
    long ns = finish.tv_nsec - start.tv_nsec; 
    
    if (start.tv_nsec > finish.tv_nsec) { // clock underflow 
        --seconds; 
        ns += 1000000000; 
    } 
    fprintf(stderr,"wallclock seconds: %e\n", (double)seconds + (double)ns/(double)1000000000); 
    double cpu_time = 
    ((double) buf2.ru_utime.tv_sec 
    + (double) buf2.ru_utime.tv_usec / (double) 1000000) - ((double) buf1.ru_utime.tv_sec 
    + (double) buf1.ru_utime.tv_usec / (double) 1000000) +      ((double) buf2.ru_stime.tv_sec 
    + (double) buf2.ru_stime.tv_usec / (double) 1000000)  - ((double) buf1.ru_stime.tv_sec 
    + (double) buf1.ru_stime.tv_usec / (double) 1000000);

    fprintf(stderr,"total runtime seconds: %e\n", cpu_time);
} 







