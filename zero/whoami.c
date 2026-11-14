#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc > 1) {
    if (strcmp(argv[1], "--help")) {
      printf("Usage: whoami [OPTION]...");
    } else {
      printf("Try 'whoami --help'");
    }
  }

}
