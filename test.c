#include <stdio.h>
#include "cstring.h"

int main() {
  string * s = cstring(NULL);

  append(s, "bruh");

  printf("%s\n", str(s));

  string * sn = substrn(s, 4, 0);

  printf("%s\n", str(sn));
}
