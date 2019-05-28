#include <stdio.h>

char get_first_char(char *s){
  return s[0];
}

int main(int argc, char **argv) {
  char *gn = argv[1];
  char *fn = argv[2];
  char g = get_first_char(gn);
  char f = get_first_char(fn);
  printf("initial of %s %s, is %c%c.\n",gn ,fn, g, f);
  return 0;
}
