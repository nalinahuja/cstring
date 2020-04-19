#include <stdio.h>
#include "cstring.h"

int main() {
  string * s = new_string();

  append(s, "hello0000");

  printf("%s\n", str(s));

  rem(s, 0);

  printf("%s\n", str(s));

  delete(s);
}
