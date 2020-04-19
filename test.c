#include <stdio.h>
#include "cstring.h"

int main() {
  string * s = cstring("asdf");

  printf("%d\n", len(s));
  printf("%d\n", cap(s));
  printf("%s\n", str(s));

  append(s, "1234567890asdfqwer");
  prepend(s, "helno");

  insert(s, "bruh", len(s));

  printf("%d\n", len(s));
  printf("%d\n", cap(s));
  printf("%s\n", str(s));

  for (int i = 0; i < 4; i++) {
    set(s, i, 'b');
  }

  printf("%d\n", len(s));
  printf("%d\n", cap(s));
  printf("%s\n", str(s));

  delete(s);
}
