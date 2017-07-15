#include "fribidi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  FriBidiLevel *levels = NULL;
  FriBidiCharType *types = NULL;
  FriBidiBracketType *bracket_types = NULL;
  const char * text="AB(CD[&ef]!)gh";  // The first N0 example from UAX#9
  FriBidiParType base_dir= FRIBIDI_PAR_RTL;
  FriBidiCharSet caprtl = fribidi_parse_charset ("CapRTL");

  fribidi_set_debug(255);
  int code_points_len = strlen(text);
  FriBidiChar * code_points = g_malloc(sizeof(FriBidiChar) * code_points_len);
  fribidi_charset_to_unicode (caprtl, text, code_points_len, code_points);

  bracket_types = g_malloc(sizeof(FriBidiBracketType) * code_points_len);
  types = g_malloc ( sizeof(FriBidiCharType) * code_points_len);
  int types_len = code_points_len;
  levels = g_malloc (sizeof (FriBidiLevel) * code_points_len);

  for (int i=0; i<types_len; i++)
    {
      types[i] = fribidi_get_bidi_type(code_points[i]);
      bracket_types[i] = fribidi_get_bracket(code_points[i]);
    }

  fribidi_get_par_embedding_levels (types, bracket_types, types_len,
                                    &base_dir,
                                    levels);
  
  for (int i=0; i<types_len; i++)
    printf("%c   ", text[i]);
  printf("\n");

  for (int i=0; i<types_len; i++)
    printf("%-3s ", fribidi_get_bidi_type_name(types[i]));
  printf("\n");

  for (int i=0; i<types_len; i++)
    printf("%-2d  ", levels[i]);
  printf("\n");

  exit(0);
}
