#include "fribidi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  FriBidiLevel *levels = NULL;
  FriBidiCharType *types = NULL;
  FriBidiBracketType *bracket_types = NULL;
  const char * text="AB(CD[&ef]!)gh";  /* The first N0 example from UAX#9 */
  int i;

  int argp=1;
  if (argc > argp)
    text = argv[argp++];

  FriBidiParType base_dir= FRIBIDI_PAR_RTL;
  FriBidiCharSet caprtl = fribidi_parse_charset ("CapRTL");

  fribidi_set_debug(255);
  int code_points_len = strlen(text);
  FriBidiChar * code_points = g_malloc(sizeof(FriBidiChar) * code_points_len);
  code_points_len = fribidi_charset_to_unicode (caprtl, text, code_points_len, code_points);

  bracket_types = g_malloc(sizeof(FriBidiBracketType) * code_points_len);
  types = g_malloc ( sizeof(FriBidiCharType) * code_points_len);
  int types_len = code_points_len;
  levels = g_malloc (sizeof (FriBidiLevel) * code_points_len);

  for (i=0; i<types_len; i++)
    {
      types[i] = fribidi_get_bidi_type(code_points[i]);
      bracket_types[i] = fribidi_get_bracket(code_points[i]);
    }

  /* Use if to get around ignored return code */
  if (fribidi_get_par_embedding_levels_ex (types, bracket_types, types_len,
                                           &base_dir,
                                           levels))
    ;
  
  for (i=0; i<code_points_len; i++)
    {
      char buf[BUFSIZ];

      fribidi_unicode_to_charset (caprtl, &code_points[i], 1, buf);
      printf("%-3s ", buf);
    }
  printf("\n");

  for (i=0; i<types_len; i++)
    printf("%-3s ", fribidi_get_bidi_type_name(types[i]));
  printf("\n");

  for (i=0; i<types_len; i++)
    printf("%-2d  ", levels[i]);
  printf("\n");

  exit(0);
}
