#include <stdbool.h>
#include <stdio.h>

#include "batman_adv.h"

int main (void) {
  printf("%d", mu_batman_adv_mesh_n_nodes(NULL, NULL));
}
