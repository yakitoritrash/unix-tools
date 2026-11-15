#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char *argv[]) {
  if (argc > 1) {
    if (strcmp(argv[1], "--help") == 0) {
      printf("Usage: whoami [OPTION]...");
    } else {
      printf("Try 'whoami --help'");
    }
  } else {
      char *s;
      s = getpwuid(1000)->pw_name;
      printf("%s\n", s);
      return 0;
  }
}
