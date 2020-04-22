#include <stdio.h>
#include "cstring.h"

int main() {
  string * newstring = cstring(NULL);

  append(newstring, "hello");
  append(newstring, "world");
  insert(newstring, " ", 5);
  int index = find(newstring, "el");

  string * s = substr(newstring, 1);

  printf("%d\n", index);
  printf("%s\n", newstring->str);
}
