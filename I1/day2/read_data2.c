#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void die(char * s) { perror(s); exit(1); }

int main(int argc, char **argv){
  if(argc < 2){
    printf("please input filename");
  }

  // 16bit
  short data[10000];

  int fd = open(argv[1],  O_RDONLY);
  if(fd == -1)die("open");

  int i = 0;
  while(1){
    int n = read(fd, data, 2);
    if(n == -1) die("read");
    if(n == 0) break;
    
    printf("%d\t%d\n",i, data[0]);
    i+=1;
  }
  close(fd);
  return 0;
}
