#include <stdio.h>
#include "cstring.h"

int main() {
 string *test = cstring("hello");
 insert(test, "asdf", 0);
 printf("%s\n", test->str);
}
