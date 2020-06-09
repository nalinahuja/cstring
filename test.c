#include <stdio.h>

#include "cstring.h"

int main() {
  string * new = cstring("hello");
  append(new, "bruh");
  prepend(new, "asdfasdf");
  append(new, "bruh");
}
