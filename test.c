#include <stdio.h>
#include "cstring.h"

int main() {
  string * newstring = cstring(NULL);

  append(newstring, "hello");
  append(newstring, "world");
  insert(newstring, " ", 5);
  int index = find(newstring, "el");

  printf("%d\n", index);
  printf("%s\n", newstring->str);

  // delete_all();
}
