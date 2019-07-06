#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <complex.h>
#include <math.h>
#include <string.h>

#include <ncurses.h>
//gcc -Wall -o serv serv.c -lm -pthread -lncurses
//./serv 50000 | play -t raw -b 16 -c 1 -e s -r 44100 -


typedef short sample_t;

void die(char *s){
    perror(s);
    exit(1);
}

//fd から 必ず n バイト読み, bufへ書く.n バイト未満でEOFに達したら, 残りは0で埋める.fd から読み出されたバイト数を返す
ssize_t read_n(int fd, ssize_t n, void * buf) {
  ssize_t re = 0;
  while (re < n) {
    ssize_t r = read(fd, buf + re, n - re);
    if (r == -1) return 0;//die("read");
    if (r == 0) break;
    re += r;
  }
  memset(buf + re, 0, n - re);
  return re;
}

/* fdへ, bufからnバイト書く */
ssize_t write_n(int fd, ssize_t n, void * buf) {
  ssize_t wr = 0;
  while (wr < n) {
    ssize_t w = write(fd, buf + wr, n - wr);
    if (w == -1) return 0; //die("write");
    wr += w;
  }
  return wr;
}

/* 標本(整数)を複素数へ変換 */
void sample_to_complex(sample_t * s, 
		       complex double * X, 
		       long n) {
  long i;
  for (i = 0; i < n; i++) X[i] = s[i];
}

/* 複素数を標本(整数)へ変換. 虚数部分は無視 */
void complex_to_sample(complex double * X, 
		       sample_t * s, 
		       long n) {
  long i;
  for (i = 0; i < n; i++) {
    s[i] = creal(X[i]);
  }
}

/* 高速(逆)フーリエ変換;
   w は1のn乗根.
   フーリエ変換の場合   偏角 -2 pi / n
   逆フーリエ変換の場合 偏角  2 pi / n
   xが入力でyが出力.
   xも破壊される
 */
void fft_r(complex double * x, 
	   complex double * y, 
	   long n, 
	   complex double w) {
  if (n == 1) { y[0] = x[0]; }
  else {
    complex double W = 1.0; 
    long i;
    for (i = 0; i < n/2; i++) {
      y[i]     =     (x[i] + x[i+n/2]); /* 偶数行 */
      y[i+n/2] = W * (x[i] - x[i+n/2]); /* 奇数行 */
      W *= w;
    }
    fft_r(y,     x,     n/2, w * w);
    fft_r(y+n/2, x+n/2, n/2, w * w);
    for (i = 0; i < n/2; i++) {
      y[2*i]   = x[i];
      y[2*i+1] = x[i+n/2];
    }
  }
}

void fft(complex double * x, 
	 complex double * y, 
	 long n) {
  long i;
  double arg = 2.0 * M_PI / n;
  complex double w = cos(arg) - 1.0j * sin(arg);
  fft_r(x, y, n, w);
  for (i = 0; i < n; i++) y[i] /= n;
}

void ifft(complex double * y, 
	  complex double * x, 
	  long n) {
  double arg = 2.0 * M_PI / n;
  complex double w = cos(arg) + 1.0j * sin(arg);
  fft_r(y, x, n, w);
}

int pow2check(long N) {
  long nn = N;
  while (nn > 1) {
    if (nn % 2) return 0;
    nn = nn / 2;
  }
  return 1;
}

void print_complex(FILE * wp, 
		   complex double * Y, long n) {
  long i;
  for (i = 0; i < n; i++) {
    fprintf(wp, "%ld %f %f %f %f\n", 
	    i, 
	    creal(Y[i]), cimag(Y[i]),
	    cabs(Y[i]), atan2(cimag(Y[i]), creal(Y[i])));
  }
}

int init_connect_from_client(int port_num, int argc){
    int ss=socket(PF_INET, SOCK_STREAM, 0);//SOCK_STREAM,TCP
    if(argc!=2){
        die("the size of marameter is wrong./n");
    }

    struct sockaddr_in addr;
    addr.sin_family=AF_INET;//IPv4
    addr.sin_port=htons(port_num);//port
    addr.sin_addr.s_addr=INADDR_ANY;//どのIPアドレスからでも良いです
    if(bind(ss, (struct sockaddr *)&addr, sizeof(addr))==-1) die("bind");//待つ

    listen(ss, 10);//接続受付開始宣言

    struct sockaddr_in client_addr;
    socklen_t len=sizeof(struct sockaddr_in);
    int s = accept(ss, (struct sockaddr *)&client_addr, &len);
    if(s==-1) die("open");

    return s;
}

void *exe_recv(void *tmp){
    sample_t data;
    int* s=(int *)tmp;
    while(1){
        int n2=recv(*s, &data, 1, 0);
        if(n2==-1) continue;
        putchar(data);//nバイトもらって一文字ずつputcharできない…
    }
}


void exe_send(int s){
    FILE *p;
    int cnt=0;
    ssize_t n=4096;
    sample_t *buf = calloc(sizeof(sample_t), n);
    sample_t MAX=0, min=0;
    //FILE * wp = fopen("fft.dat", "wb");//
    //if (wp == NULL) die("fopen");

    if((p=popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r"))==NULL) die("popen");
    int p_num=fileno(p);
    while(1){
        ssize_t m=read_n(p_num, n*sizeof(sample_t), buf);//m文字読み込む
        if(m==0) continue;
        for(int i=0; i<m; i++){
          if(buf[i]>MAX) MAX=buf[i];
          else if(buf[i]<min) min=buf[i];
        }
        if(MAX-min<1000){//振幅が小さい時はsend,fftしない；連続で振幅が小さいときのみ飛ばす
          cnt++;
          if(cnt>100){
            continue;
          }
        }else cnt=0;
        MAX=0,min=0;

        int n1=send(s, buf, n*sizeof(sample_t), 0);//必要最低限だけ送る。
        if(n1==-1) continue;
    }
    fclose(p);
    exit(1);
}

int main(int argc, char **argv){

    int port_num = atoi(argv[1]);

    int s=init_connect_from_client(port_num, argc);

    pthread_t pthread;

    pthread_create(&pthread, NULL, &exe_recv, &s);
    exe_send(s);
    pthread_join(pthread, NULL);

    close(s);
    
    return 0;
}