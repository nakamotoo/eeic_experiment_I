#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// ./a.out 192.168.100.106 50000

void die(char *s){perror(s); exit(1);}

int main(int argc, char **argv){
  if(argc != 3){
    printf("syntax: ./a.out ip_address port_num");
  }

  // printf("%s\n", argv[1]);
  // printf("%s", argv[2]);

  unsigned char data[256];

  int port_num = atoi(argv[2]);

  // ソケットを作る→ファイルをopen的な
  int s = socket(PF_INET, SOCK_STREAM, 0);
  if(s == -1)die("socket");

  // connect → ipアドレスとポート番号の接続
  struct sockaddr_in addr;
  addr.sin_family = AF_INET; // IPv4のアc
  inet_aton(argv[1], &addr.sin_addr); //inet_aton()は32bitに変換
  addr.sin_port = htons(port_num);
  int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr));
  if(ret == -1)die("connect");

  while(1){
      int n = recv(s, data, 1, 0);
      if(n == -1)die("recv");
      // server側からctrl + c で中断された場合、break
      if(n == 0) break;
      printf("%c", data[0]);
  }
  close(s);
  return 0;
}
