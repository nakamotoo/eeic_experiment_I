#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

//./a.out 10000 15 > doremi.raw
// play -b 16 -c 1 -e s -r 44100 doremi.raw

void die(char * s) { perror(s); exit(1); }

int main(int argc, char **argv){
  if(argc != 3){
    printf("argument is not enough!!!");
    return 0;
  }

  int a = atoi(argv[1]);
  // int f = atoi(argv[2]);
  int n = atoi(argv[2]);
  int f_ra = 440;

  double p[7] = {0.0833, 0.1666, 0.1666, 0.0833, 0.1666, 0.1666, 0.1666};

  // si
  double f_now = f_ra * pow(2, 0.1666);

  short data[13230];

  // printf("f_now = %f\n", pow(2, 0.1666));

  for(int i = 0; i < n; i++){
    int p_index = i % 7;
    f_now = f_now * pow(2, p[p_index]);
    // printf("f_now = %f\n", f_now);

    for(int t=0; t <13230; t++){
      short x =(short) a * sin(2 * M_PI * f_now * t / 44100);
      // printf("x = %f\n", (short)sin(2 * M_PI * f_now * t / 44100));
      // printf("x = %d\n", x);
      data[t] = x;
    }

    for(int j = 0; j<13230; j++){
      // printf("j = %d\n", data[j]);
      write(1, data+j, 2);
    }

  }

  return 0;
}
