// A straight translation of algo_n0.py to C.

#include <glib.h>
#include <string.h>
#include <stdio.h>
#include "algo-n0.h"
#include "bd16.h"

gboolean charinstring(gchar ch, const char *string)
{
  const char *p=string;
  while(*p)
      if (*p++ == ch)
        return TRUE;
  return FALSE;
}

gboolean is_strong(bidi_class_t bc)
{
  return bc=='R' || bc=='L';
}

bidi_class_t get_bidi_class(gchar ch)
{
  if (charinstring(ch, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"))
    return (bidi_class_t)'R';
  if (charinstring(ch, "abcdefghijklmnopqrstuvwxyz"))
    return (bidi_class_t)'L';
  if (charinstring(ch, "({[]})&!"))
    return (bidi_class_t)'N';
  if (charinstring(ch, " "))
    return (bidi_class_t)'N';
  return (bidi_class_t)'?';
}

// Find a strong character between start and end (non-inclusive)
bidi_class_t find_strong(bidi_class_t *bidi_classes, int start, int end)
{
  int dir= end < start ? -1 : 1;
  int i;
  
  for (i=start; i!=end; i+= dir)
    {
      if (is_strong(bidi_classes[i]))
        return bidi_classes[i];
    }
  return (bidi_class_t)'?';
}

// Used to find a character beetween start and end with the matching bidi class
gboolean find_matching_strong(bidi_class_t *bidi_classes, int start, int end, bidi_class_t match)
{
  int i;
  for (i=start; i<end; i++)
    if (bidi_classes[i] == match)
      return TRUE;
  return FALSE;
}

void test_n0(gchar *text, bidi_class_t embedding_direction)
{
  int n = strlen(text);
  int i;
  for (i=0; i<n; i++)
      printf("%2d ",i);
  printf("\n");
  for (i=0; i<n; i++)
      printf("%2c ",text[i]);
  printf("\n");
    

  pairing_node_t *pairings = find_pairings(text);

  bidi_class_t *bidi_classes = g_new0(bidi_class_t, n);
  for (i=0; i<n; i++)
    bidi_classes[i] = get_bidi_class(text[i]);
  for (i=0; i<n; i++)
      printf("%2c ",bidi_classes[i]);
  printf("\n");

  pairing_node_t *pr = pairings;
  while(pr)
    {
      char *rule = NULL;
      gboolean has_strong = find_matching_strong(bidi_classes,
                                                 pr->start, pr->end,
                                                 embedding_direction);
      bidi_class_t preceding_strong = find_strong(bidi_classes,
                                                  pr->start, -1);

      // Rule N0b
      if (has_strong)
        {
          rule= g_strdup_printf("N0b: %c->%c",bidi_classes[pr->start],embedding_direction);
          bidi_classes[pr->start]=bidi_classes[pr->end]=embedding_direction;
        }

      else if (charinstring(preceding_strong,"RL"))
        {
          // Rule N0c1
          // printf("preceding_strong embedding_direction=%c %c",
          //        preceding_strong,embedding_direction);
          if (preceding_strong != embedding_direction)
            {
              rule= g_strdup_printf("N0c1: %c->%c",
                                    bidi_classes[pr->start],
                                    preceding_strong);
              bidi_classes[pr->start]=bidi_classes[pr->end]=preceding_strong;
            }
          // Rule N0c2
          else
            {
              rule= g_strdup_printf("N0c2: %c->%c",
                                    bidi_classes[pr->start],embedding_direction);
              bidi_classes[pr->start]=bidi_classes[pr->end]=embedding_direction;
            }
        }
      else
        {
          // N0d - Do nothing;
          rule = g_strdup("N0d");
        }
      for (i=0; i<n; i++)
        printf("%2c ",bidi_classes[i]);
      printf("%s\n", rule);
      g_free(rule);
      pr = pr->next;
    }

  g_free(bidi_classes);
  free_parings(pairings);
  printf("\n");
}

