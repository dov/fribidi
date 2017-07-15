// Like bd16.c but with a fixed stack size and with fribidi
// query of the bracket type.
//
// A translation of the BD16.py
//
// Dov Grobgeld <dov.grobgeld@gmail.com>
// 2017-07-07 Fri

#include <stdlib.h>
#include <stdio.h>
#include "bd16-v1.h"
#include "fribidi-brackets.h"

bool_t get_brack_prop(char c,
                      // output
                      brack_prop_t *prop)
{
  FriBidiBracketType bracket_type = fribidi_get_bracket(c);

  if (bracket_type.bracket_id == 0)
    return 0;

  prop->opening = bracket_type.is_open;
  if (prop->opening)
      prop->type = c;
  else
      prop->type = bracket_type.bracket_id;

  printf("c opening type = %c %d %d\n",
         c, prop->opening, prop->type);

  return 1;
}

pairing_node_t *new_pairing_node(int start, int end)
{
  pairing_node_t *node =  g_new0(pairing_node_t, 1);
  node->start = start;
  node->end = end;
  node->next = NULL;

  return node;
}

#define FRIBIDI_BRACKET_PAIR_STACK_SIZE 63

pairing_node_t *find_pairings(const char *text)
{
  pairing_node_t *pairs = NULL;
  element_t *pair_stack;
  pair_stack = (element_t*)malloc (sizeof (pair_stack[0]) *
                                   FRIBIDI_BRACKET_PAIR_STACK_SIZE);
  int pair_stack_size = 0;

  const char *pch = text; // Assume zero terminated
  int pos = 0;
  while(*pch)
    {
      brack_prop_t brack_prop;
      if (get_brack_prop(*pch,
                         // output
                         &brack_prop))
        {
          if (brack_prop.opening)
            {
              if (pair_stack_size==FRIBIDI_BRACKET_PAIR_STACK_SIZE)
                  break;

              // push onto the pair stack
              element_t element = { *pch, pos };
              pair_stack[pair_stack_size++] = element;
            }
          else
            {
              int stack_idx = pair_stack_size - 1;
              brack_prop_t se_brack_prop;
              while(stack_idx >= 0)
                {
                  element_t *pst = &pair_stack[stack_idx];
                  get_brack_prop(pst->ch,
                                 // output
                                 &se_brack_prop);
                  if (se_brack_prop.type == brack_prop.type)
                    {
                      pair_stack_size = stack_idx;

                      pairing_node_t *pair = new_pairing_node(pst->pos, pos);
                      pair->next = pairs;
                      pairs = pair;

                      break;
                    }
                  stack_idx--;
                }
            }
        }
      pch++;
      pos++;
    }

  free(pair_stack);

  // Do we need to sort?
  return pairs;
}
