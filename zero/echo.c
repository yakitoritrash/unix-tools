#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (strcmp(argv[1], "-n") != 0) {
    for (int i = 1; i < argc; i++) {
      if (i != argc - 1) {
        printf("%s ", argv[i]);
      } else {
        printf("%s", argv[i]);
      }
    }
    printf("\n");
  } else {
    for (int i = 2; i < argc; i++) {
      if (i != argc - 1) {
        printf("%s ", argv[i]);
      } else {
        printf("%s", argv[i]);
      }
    }
  }
}
