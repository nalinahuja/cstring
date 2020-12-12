#include <stdio.h>
#include "cstring.h"

int main() {
  string * new = cstring(NULL);
  append(new, "Hello world!");
  printf("%s\n", str(new));
}
