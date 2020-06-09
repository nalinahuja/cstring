#include <stdio.h>

#include "cstring.h"

// TODO, stress test the program and fix bugs

int main() {
  string * new = cstring(NULL);

  append(new, "hello");

  string * sub = substrn(new, 0, len(new) - 1);

  append(sub, "bruh");

  printf("Str: %s\n", str(sub));
  printf("Len: %d\n", len(sub));
  printf("Len: %d\n", cap(sub));
}
