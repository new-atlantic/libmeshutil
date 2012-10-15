#include <stdbool.h>
#include <stdio.h>

#include "batman_adv.h"

int main (void) {
  if (mu_batman_adv_if_up("not_default", NULL)) {
    printf("TRUE");
  } else {
    printf("FALSE");
  }
}
