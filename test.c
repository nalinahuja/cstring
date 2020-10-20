#include <stdio.h>
#include "cstring.h"

int main() {
 string *test = cstring("hello");
 insert(test, NULL, 0);
 printf("%s\n", test->str);
}
