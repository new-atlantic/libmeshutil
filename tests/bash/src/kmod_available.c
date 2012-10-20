#include <stdbool.h>
#include <stdio.h>

#include "batman_adv.h"

int main (void) {
  if (mu_badv_kmod_available(NULL)) {
    printf("TRUE");
  } else {
    printf("FALSE");
  }
}
