#include <stdio.h>

#include "cstring.h"

int main() {
  for (int i = 0; i < 10; i++) {
    string * new = cstring("hello");

    append(new, "bruh");
    prepend(new, "asdfasdf");
    append(new, "bruh");

    printf("%s\n", str(new));

    if (i > 5) {
      delete(new);
    }
  }
}
