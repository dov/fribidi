#include <stdlib.h>
#include <stdio.h>
#include "algo-n0.h"

int main(int argc, char **argv)
{
  test_n0("AB(CD[&ef]!)gh", 'R');
  test_n0("sm (fa AR) HE", 'R');
  test_n0("AR bo(s)",'R');

  exit(0);
}
