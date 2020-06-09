#include <stdio.h>

#include "cstring.h"

// TODO, stress test the program and fix bugs

int main() {
  string * new = cstring(NULL);

  append(new, ", world");
  prepend(new, "hello");
  append(new, "!");

  printf("Len: %d\n", len(new));
  printf("Cap: %d\n", cap(new));
  printf("Str: %s\n", str(new));

  delete(new);

  string * cpy = copy(new);

  printf("Len: %d\n", len(cpy));
  printf("Cap: %d\n", cap(cpy));
  printf("Str: %s\n", str(cpy));
}
