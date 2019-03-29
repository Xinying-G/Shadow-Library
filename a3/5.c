// open a file 
#include <stdio.h> 
#define BUFLEN 1000
main() { 
      FILE *fp = fopen("/etc/passwd", "r"); 
      sleep(1); 
      char buf[BUFLEN]; 
      while (! feof(fp)) { 
	fgets(buf, BUFLEN, fp); 
	printf("%s",buf); 
      } 
      fprintf(stderr, "oink!\n"); 
} 
 
