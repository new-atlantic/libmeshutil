#include <stdbool.h>
#include <stdio.h>

#include "batman_adv.h"

int main (void) {
  if (mu_batman_adv_kmod_loaded(NULL)) {
    printf("TRUE");
  } else {
    printf("FALSE");
  }
}
