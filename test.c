#include <stdio.h>
#include "cstring.h"

int main() {
  string * new = cstring(NULL);

  append(new, ", world");
  prepend(new, "hello");
  insert(new, "!", len(str));

  printf("%s\n", str(new));
}
