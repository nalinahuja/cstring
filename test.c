#include <stdio.h>
#include "cstring.h"

int main() {
  string * s = new_string();

  append(s, "a");
  append(s, "bdf");
  append(s, "qwer");

  printf("%s\n", s->str);
  printf("%s\n", strnsub(s, 1, 5));
  printf("%s\n", strsub(s, 1));
  delete(s);
}
