#include "fribidi-brackets.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  fribidi_uint8 type;
  FriBidiChar ch = 0;
  fribidi_int8 foo;

  int ret = fribidi_get_bracket('}',
                                // output
                                &type,
                                &ch);

  printf("ret type ch = %d %d %c\n", ret, type, ch);
  
  exit(0);
}
