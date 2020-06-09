#include <stdio.h>

#include "cstring.h"

// TODO, stress test the program and fix bugs

int main() {
  for (int i = 0; i < 10; i++) {
    string * new = cstring("hello");

    append(new, "bruh");
    prepend(new, "asdfasdf");
    append(new, "bruh");
    delete(new);
  }
}
