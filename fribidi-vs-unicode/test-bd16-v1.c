// A small test program of bd16-v1
#include <stdio.h>
#include <stdlib.h>
#include "bd16-v1.h"

int main(int argc, char **argv)
{
  const char *text = "a(b{c}d)";
  printf("Testing: %s\n", text);
  pairing_node_t *pairs = find_pairings(text);
  while(pairs)
    {
      pairing_node_t *p = pairs;
      printf("%d,%d ",pairs->start,pairs->end);
      pairs = pairs->next;
      free(p);
    }
  printf("\n");
  
  exit(0);
}
