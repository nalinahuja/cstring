#include <stdio.h>
#include "cstring.h"

int main() {
  string * new_string = cstring(NULL);

  printf("%s\n", new_string->str);

  delete(new_string);

  delete_all();
}
