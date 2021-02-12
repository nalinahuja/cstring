#include <stdio.h>
#include "cstring.h"

int main() {
  char * buff = "asdfasdf";
  string * s = cstring(buff);

  printf("%s", str(s));

  buff = "qwerqwerqwe";

  append(s, buff);

  printf("%s", str(s));
}
