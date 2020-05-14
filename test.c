#include <stdio.h>

#include "cstring.h"

int main() {
  string * new = cstring(NULL);

  append(new, ", world");
  prepend(new, "hello");
  append(new, "!");

  printf("%s\n", str(new));
}
