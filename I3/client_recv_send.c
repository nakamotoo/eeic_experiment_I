#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Compile and link with -pthread.

//./client_recv_send 192.168.100.110 50001 | play -t raw -b 16 -c 1 -e s -r 44100 -
void die(char *s){perror(s); exit(1);}

void exe_send(int s){
  FILE *fp;
  int ch;
  fp = popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r");
  if(fp == NULL) die("popen");
  while((ch = fgetc(fp)) != EOF){
    int n = send(s, &ch, 1, 0);
    if(n == -1)die("send");
  }
}

void* exe_recv(void *tmp){
  int* s = (int *)tmp;
  char data;
  // fp = popen("play -t raw -b 16 -c 1 -e s -r 44100 -", "w");
  // if(fp == NULL) die("popen");
  // recv
  while(1){
    int n = recv(*s, &data, 1, 0);
    if(n == -1)die("recv");
    if(n == 0) break;
    printf("%c", data);
  }
  return 0;
}

int main(int argc, char **argv){
  if(argc != 3){
    printf("syntax: ./a.out ip_address port_num");
  }

  int port_num = atoi(argv[2]);

  int s = socket(PF_INET, SOCK_STREAM, 0);
  if(s == -1)die("socket");
  // connect → ipアドレスとポート番号の接続
  struct sockaddr_in addr;
  addr.sin_family = AF_INET; // IPv4のアc
  inet_aton(argv[1], &addr.sin_addr); //inet_aton()は32bitに変換
  addr.sin_port = htons(port_num);
  int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr));
  if(ret == -1)die("connect");

  pthread_t pthread;

  pthread_create(&pthread, NULL, &exe_recv, &s);
  exe_send(s);
  pthread_join(pthread, NULL);
  close(s);
  return 0;
}
