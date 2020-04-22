#include <stdio.h>
#include "cstring.h"

int main() {
  string * new_string = cstring(NULL);

  append(new_string, "world");
  prepend(new_string, "hello ");

  string * subst = substrn(new_string, 1, 4);

  printf("%s\n", new_string->str);
  printf("%s\n", subst->str);


  delete(subst);
  delete(new_string);
}
