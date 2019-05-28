#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void die(char * s) { perror(s); exit(1); }

int main(){
  unsigned char data[6] = {228, 186, 186, 229, 191, 151};

  int fd = open("hitoshi", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if(fd == -1)die("open");
  write(fd, data, 6);
  return 0;
}
