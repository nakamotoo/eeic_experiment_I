#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

// ./a.out A f n > hoge.raw

void die(char * s) { perror(s); exit(1); }

int main(int argc, char **argv){
  if(argc != 4){
    printf("argument is not enough!!!");
    return 0;
  }

  int a = atoi(argv[1]);
  int f = atoi(argv[2]);
  int n = atoi(argv[3]);

  short data[n];
  for(int t=0; t <=n; t++){
    short x = a * sin(2 * M_PI * f * t / 44100);
    data[t] = x;
  }

  for(int i = 0; i<=n; i++){
    write(1, data+i, 2);
  }
  return 0;
}
