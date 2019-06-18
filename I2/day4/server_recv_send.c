#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#define MAXLINE 10000
#include <pthread.h>


// ./server_recv_send 50000 | play -t raw -b 16 -c 1 -e s -r 44100 -

void die(char *s){perror(s); exit(1);}

void* exe_recv(void *tmp){
  int* s = (int *)tmp;
  char data;
  while(1){
    int n = recv(*s, &data, 1, 0);
    if(n == -1)die("recv");
    if(n == 0) break;
    printf("%c", data);
  }
  return 0;
}

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

int main(int argc, char **argv){
  if(argc != 2){
    printf("syntax: ./a.out port_num");
  }

  int port_num = atoi(argv[1]);

  // ソケットを作る→ファイルをopen的な
  int ss = socket(PF_INET, SOCK_STREAM, 0);
  if(ss == -1)die("socket");

  // bind → ソケットにポート番号を割り当てる
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port_num);
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(ss, (struct sockaddr *)&addr, sizeof(addr));

  // listen → 接続受付開始
  listen(ss, 10);

  // accept → クライアントからの接続を待つ
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(struct sockaddr_in);
  int s = accept(ss, (struct sockaddr *)&client_addr, &len);
  close(ss);

  pthread_t pthread;

  pthread_create(&pthread, NULL, &exe_recv, &s);
  exe_send(s);
  pthread_join(pthread, NULL);
  close(s);
  return 0;

  close(s);
  return 0;
}
