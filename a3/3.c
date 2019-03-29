// a very friendly and control-C sensitive fork botch.
#include <stdio.h> 
main() { 
    if (fork()) { 
        sleep(5); 
        fprintf(stderr, "oink!\n"); 
    } 
} 
