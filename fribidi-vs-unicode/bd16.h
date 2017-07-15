//======================================================================
//  bd16.h - An implementation of the unicode bd16 algorithm.
//
//  Dov Grobgeld <dov.grobgeld@gmail.com>
//  Fri Jul  7 11:06:20 2017
//----------------------------------------------------------------------
#ifndef BD16_H
#define BD16_H

typedef int bool_t;

typedef struct pairing_node_s {
  int start;
  int end;

  // Connect to next and previous nodes.
  struct pairing_node_s *next, *prev;
} pairing_node_t;

//typedef struct pairing_node_s ;
typedef struct {
  int type;
  int opening;
} brack_prop_t;

typedef struct element_s {
  char ch;
  int pos;

  // Connect to next and previous nodes for stack
  struct element_s *next;
} element_t;

pairing_node_t *find_pairings(const char *text);

void free_parings(pairing_node_t *pairings);

#endif /* BD16 */
