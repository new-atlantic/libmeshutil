#include <stdbool.h>
#include <stdio.h>

#include "../../../src/batman_adv.h"

int main (void) {
  if (mu_batman_adv_kmod_available(NULL)) {
    printf("TRUE");
  } else {
    printf("FALSE");
  }
}
