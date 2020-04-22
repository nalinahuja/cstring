#include <stdio.h>
#include "cstring.h"

int main() {
  string * a = cstring(NULL);
  string * b = cstring("asdf");
  string * c = substrn(b, 1, 2);

  string * cpy = copy(a);

  printf("%s\n", str(a));
  printf("%s\n", str(b));
  printf("%s\n", str(c));
  delete_all();
}
