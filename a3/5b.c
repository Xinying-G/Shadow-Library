// open a file 
#include <stdio.h> 
#define BUFLEN 1000
main() { 
    FILE *fp = fopen("/etc/passwd", "r"); 
    char buf[BUFLEN]; 
    fgets(buf, BUFLEN, fp); 
    fprintf(stderr, "oink! %s",buf); 
    fclose(fp); 
} 
 
