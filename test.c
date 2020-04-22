#include <stdio.h>
#include "cstring.h"

int main() {
  string * a = cstring(NULL);
  string * b = cstring("asdf");
  string * c = substrn(b, 1, 2);
}
