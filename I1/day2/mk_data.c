#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void die(char * s) { perror(s); exit(1); }

int main(int argc, char **argv){
  unsigned char data[256];
  for(int i=0; i <=255; i++){
    data[i] = (char)i;
  }

  int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if(fd == -1)die("open");
  write(fd, data, 256);
  return 0;
}
