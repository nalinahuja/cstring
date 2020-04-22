#include <stdio.h>
#include "cstring.h"

int main() {
  string * a = cstring(NULL);
  string * b = cstring("asdf");
  string * c = substrn(b, 1, 2);

  string * cpy = copy(b);
  append(cpy, "asdfasdfadsf");
  append(a, "henlo");

  printf("%s\n", str(a));
  printf("%s\n", str(b));
  printf("%s\n", str(c));
  printf("%s\n", str(cpy));
  delete_all();
}
