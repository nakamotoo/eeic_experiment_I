#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

/*
./a.out 5 < dq1.raw >dq1_5.raw
 play -t raw -b 16 -c 1 -e s -r 8820 dq1_5.raw

 or

 ./a.out 20 < dq1.raw >dq1_20.raw
  play -t raw -b 16 -c 1 -e s -r 2205 dq1_20.raw

*/

void die(char * s) { perror(s); exit(1); }

int main(int argc, char **argv){

  if(argc != 2){
    printf("syntax should be ./a.out n");
  }

  int gap = atoi(argv[1]);
  short data[1000000];

  int i = 0;
  while(1){
      int n = read(0, data+i, 2);
      i++;
      if(n == -1) die("read");
      if(n == 0) break;
  }

  for(int j = 0; j <= i; j+=gap){
    write(1, data+j, 2);
  }
  return 0;
}
