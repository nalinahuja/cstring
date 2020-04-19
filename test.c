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

  string * s2 = substrn(s, 1, 4);
  printf("%s\n", str(s2));

  printf("%d\n", len(s));
  printf("%d\n", cap(s));
  printf("%s\n", str(s));

  delete();


}
