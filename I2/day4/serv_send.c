#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void die(char *s){perror(s); exit(1);}

int main(int argc, char **argv){
  if(argc != 2){
    printf("syntax: ./a.out port_num");
  }

  int port_num = atoi(argv[1]);

  // bind → ソケットにポート番号を割り当てる
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port_num);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(ss, (struct sockaddr *)&addr, sizeof(addr));

  // listen → 接続受付開始
  listen(ss, 10);
}
