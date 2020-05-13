#include <stdio.h>
#include "cstring.h"

int main() {
  string * new = cstring(NULL);

  append(new, "hello");

  printf("%s\n", str(new));
}
