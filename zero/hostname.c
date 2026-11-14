#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc > 1) {
    if (strcmp(argv[1], "--help") == 0) {
      printf("Usage: hostname [OPTION...] [NAME]");
    } else {
      printf("Try 'hostname --help'");
    }
  } else {
      char s[256];
      gethostname(s, sizeof(s));
      printf("%s", s);
  }
}

