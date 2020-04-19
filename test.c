#include <stdio.h>
#include "cstring.h"

int main() {
  string * s = cstring(NULL);

  append(s, "hello1000000000");

  printf("%s\n", str(s));

  rem(s, 5);

  printf("%s\n", str(s));

  delete(s);
}
