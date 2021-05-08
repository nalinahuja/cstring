#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
// #include "cstring.h"

int main() {

  printf("%ld", sizeof(bool));
  int a = 100;

  a = (a << 1);

  char * new_str = (char *) calloc(sizeof(char), (a << 1));

  new_str[0] = '1';
  new_str[1] = 0;

  // printf("%d\n", a);
  //
  // printf("%d\n", a);
  // printf("%s\n", new_str);

  // string * s = cstring(buff);
  //
  // printf("%s", str(s));
  //
  // buff = "qwerqwerqwe";
  //
  // append(s, buff);
  //
  // printf("%s", str(s));
}
