// A test program for fribidi_get_bracket()
#include "fribidi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  FriBidiCharSet utf8 = fribidi_parse_charset ("UTF-8");
  FriBidiChar ubuf[BUFSIZ];

  const char *buf="〈〉〈〉";
  int len = fribidi_charset_to_unicode (utf8, buf, strlen(buf), ubuf);

  for (int i=0; i<len; i++)
    {
      FriBidiBracketType bracketing_type = fribidi_get_bracket(ubuf[i]);

      printf("ch id is_open = U+%04x U+%04x %d\n",
             ubuf[i],
             bracketing_type.bracket_id,
             bracketing_type.is_open);
    }
  
  exit(0);
}
