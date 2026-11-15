#include <stdio.h>
#include <unistd.h>

int str_eq(char *s, char *t) {
  while (*s != '\0' && *t != '\0') {
    if (*s != *t) {
      return 0;
    }
    s++;
    t++;
  }
  return 1;
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    if (str_eq(argv[1], "-h")) {
      printf("pwd: usage: pwd [-LP]");
    }
  } else {
      char s[256];
      getcwd(s, sizeof(s));
      printf("%s\n", s);
  }
}

