// A translation of the BD16.py
//
// Dov Grobgeld <dov.grobgeld@gmail.com>
// 2017-07-07 Fri

#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "bd16.h"

bool_t get_brack_prop(char c,
                      // output
                      brack_prop_t *prop)
{
  if (c=='(' || c==')')
    {
      prop->type = 1;
      prop->opening = (c=='(');
      return 1;
    }
  if (c=='[' || c==']')
    {
      prop->type = 2;
      prop->opening = (c=='[');
      return 1;
    }
  if (c=='{' || c=='}')
    {
      prop->type = 3;
      prop->opening = (c=='{');
      return 1;
    }
  return 0;
}

pairing_node_t *new_pairing_node(int start, int end)
{
  pairing_node_t *node =  g_new0(pairing_node_t, 1);
  node->start = start;
  node->end = end;
  node->next = NULL;

  return node;
}

element_t *new_element(char ch, int pos)
{
  element_t *node =  g_new0(element_t, 1);
  node->ch = ch;
  node->pos = pos;
  node->next = NULL;
  return node;
}

pairing_node_t *find_pairings(const char *text)
{
  pairing_node_t *pairs = NULL;
  element_t *pair_stack = NULL;

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
              // push onto the pair stack
              element_t *node = new_element(*pch, pos);
              node->next = pair_stack;
              pair_stack = node;
            }
          else
            {
              element_t *pst = pair_stack;
              brack_prop_t se_brack_prop;
              while(pst)
                {
                  get_brack_prop(pst->ch,
                                 // output
                                 &se_brack_prop);
                  if (se_brack_prop.type == brack_prop.type)
                    {
                      // pop until we hit pst
                      while(pair_stack != pst)
                        {
                          element_t *p = pair_stack;
                          pair_stack = pair_stack->next;
                          free(p);
                        }

                      pairing_node_t *pair = new_pairing_node(pst->pos, pos);
                      pair->next = pairs;
                      pairs = pair;
                      pair_stack = pair_stack->next;
                      free(pst);
                      break;
                    }
                  pst=pst->next;
                }
            }
        }
      pch++;
      pos++;
    }

  // No pair found. just clean the stack
  while (pair_stack)
    {
      element_t *p = pair_stack;
      pair_stack = pair_stack->next;
      free(p);
    }
  
  // Do we need to sort?
  return pairs;
}

