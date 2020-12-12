#include <stdio.h>
#include "cstring.h"

int main() {
  string * new = cstring(NULL);
  append(new, "asdf\n");
  printf("%s", str(new));
}
