#include <stdio.h>

#include "cstring.h"

// TODO, stress test the program and fix bugs

int main() {
  string * new = cstring(NULL);

  for (int i = 0; i < 10; i++) {
    append(new, "hello");
  }

  for (int i = 0; i < 10; i++) {
    prepend(new, "REE");
  }

  string * sub = substr(new, 10);

  concat(new, sub);

  printf("Str: %s\n", str(new));
  printf("Len: %d\n", len(new));
  printf("Cap: %d\n", cap(new));
}
