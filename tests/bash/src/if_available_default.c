#include <stdbool.h>
#include <stdio.h>

#include "batman_adv.h"

int main (void) {
  if (mu_badv_if_available(NULL, NULL)) {
    printf("TRUE");
  } else {
    printf("FALSE");
  }
}
