#include <stdio.h>
#include "cstring.h"

int main() {
  string * new_string = cstring(NULL);

  append(new_string, "world");
  prepend(new_string, "hello ");

  printf("%s\n", new_string->str);

  delete(new_string);
}
