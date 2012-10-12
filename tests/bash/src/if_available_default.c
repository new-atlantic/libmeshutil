#include <stdbool.h>
#include <stdio.h>

#include "../../../src/batman_adv.h"

int main (void) {
  if (mu_batman_adv_if_available(NULL, NULL)) {
    printf("TRUE");
  } else {
    printf("FALSE");
  }
}
