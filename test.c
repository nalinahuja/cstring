#include <stdio.h>
#include "cstring.h"

int main() {
  string * new = cstring(NULL);
  append(new, "asdf\n");
  rem(new, 3);
  printf("%s", str(new));
}
