#include <stdio.h>
#include "cstring.h"

int main() {
  string * test = cstring(NULL);

  append(test, NULL);

  printf("%s", str(test));
}
