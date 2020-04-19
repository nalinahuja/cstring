#include <stdio.h>
#include "cstring.h"

int main() {
  string * s = new_string();
  delete(s);
}
