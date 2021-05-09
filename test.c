#include <stdio.h>
#include "cstring.h"

int main() {
  string * s = cstring(NULL);

  append(s, "bruh");

  printf("%s\n", str(s));
}
