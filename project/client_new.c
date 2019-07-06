#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <complex.h>
#include <math.h>
#include <string.h>

#include <ncurses.h>
//gcc -Wall -o client client.c -lm -pthread -lncurses
//./client 100.100.100.100 50000 | play -t raw -b 16 -c 1 -e s -r 44100 -

//rec -t raw -b 16 -c 1 -e s -r 44100 - | nc 100.100.100.100 50000 | play -t raw -b 16 -c 1 -e s -r 44100 -

static char flag;

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
    if (r == -1) die("read");
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
    if (w == -1) die("write");
    wr += w;
  }
  return wr;
}

/* 標本(整数)を複素数へ変換 */
void sample_to_complex(sample_t * s, 
		       complex double * X, 
		       long n) {
  long i;
  for (i = 0; i < n; i++)X[i] = s[i];
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

void *exe_recv(void *tmp){
    ssize_t NMAX=4096;//sendでも宣言されている
    sample_t *buf = calloc(sizeof(sample_t), NMAX);
    //complex double *X=calloc(sizeof(complex double), NMAX);
    //complex double *Y=calloc(sizeof(complex double), NMAX);

    //double f_peak[4]={50, 200, 1000, 10000};//4領域
    //double ampli[4]={1, 1, 1, 1};//0.1,1,2?くらい
    //int f_list[5]={0, 100, 300, 1700, 18300};//変化させる周波数域

    int* s=(int *)tmp;
    while(1){
        ssize_t m=recv(*s, buf, NMAX*sizeof(sample_t), 0);
        if(m==-1 || m==0) continue;

        /*if(flag=='a'){
          endwin();
          exit(1);
        }*/
        //入力に応じて変数を書き換える。ampliについて
        /*
        else if(flag=='b') ampli[0]=0.1;
        else if(flag=='c') ampli[0]=1;
        else if(flag=='d') ampli[0]=2;
        else if(flag=='e') ampli[1]=0.1;
        else if(flag=='f') ampli[1]=1;
        else if(flag=='g') ampli[1]=2;
        else if(flag=='h') ampli[2]=0.1;
        else if(flag=='i') ampli[2]=1;
        else if(flag=='j') ampli[2]=2;
        else if(flag=='k') ampli[3]=0.1;
        else if(flag=='l') ampli[3]=1;
        else if(flag=='m') ampli[3]=2;
        */

        //sample_to_complex(buf, X, m);
        //fft(X,Y,m);

        //bandpass-filter
        /*double fpern=44100/m;
        for(int i=0; i<4; i++){
          for(int freq=f_list[i]/fpern+1; freq<=f_list[i+1]; freq++){
            Y[freq]/=ampli[i];
          }
        }*/

        /*for(int i=f_list[0]/fpern; i <= f_list[1]/fpern; i++){
          Y[i] /= ampli[0];
        }*///bandpass-fileterの書き方

        //ifft(Y,X,n);
        //complex_to_sample(X,buf,m);
        write_n(1, m, buf);
    }
}

void exe_send(int s){
    FILE *p;

    WINDOW *win=initscr();//ncursesの初期化
    noecho();//入力文字を出力しないようにする
    cbreak();//実行すると入力文字を即時利用できる
    nodelay(win, true);//
    timeout(0);
    char flag_tmp;
    ssize_t NMAX=4096;//recvでも宣言されている
    sample_t *data = calloc(sizeof(sample_t), NMAX);
    sample_t MAX=0,min=0;
    int cnt=0;

    if((p=popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r"))==NULL) die("popen");
    int p_num=fileno(p);

    while(1){
        ssize_t m=read_n(p_num, NMAX*sizeof(sample_t), data);
        //コマンドから引数を取る
        flag_tmp=getch();
        if(flag_tmp=='a'){//その文字が意味を与えたている文字であればグローバル変数を書き換える
          flag='a';
        }

        for(int i=0; i<m; i++){
          if(data[i]>MAX) MAX=data[i];
          else if(data[i]<min) min=data[i];
        }
        if(MAX-min<1000){//振幅が小さい時はsend,fftしない；連続で振幅が小さいときのみ飛ばす
          cnt++;
          if(cnt>100){
            continue;
          }
        }else cnt=0;
        min=0;
        MAX=0;

        int n1=send(s, data, m, 0);
        if(n1==-1) continue;
    }
}

int main(int argc, char **argv){
    if(argc!=3){
        printf("The number of argument is wrong.\n");
        return 0;
    }

    //int fd=open(argv[3], O_RDONLY, 0644);
    //if(fd==-1) die("file_open");

    int port_num = atoi(argv[2]);
    int s = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;//IPv4のアドレス
    inet_aton(argv[1], &addr.sin_addr);//IPアドレス
    addr.sin_port = htons(port_num);//port
    int ret = connect(s, (struct sockaddr *)&addr, sizeof(addr));//connect
    
    if(ret==-1) perror("open");

    pthread_t pthread;

    pthread_create(&pthread, NULL, &exe_recv, &s);
    exe_send(s);
    pthread_join(pthread, NULL);

    close(s);

    return 0;
}