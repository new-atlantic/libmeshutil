#include <stdbool.h>
#include <stdio.h>

#include "batman_adv.h"

int main (void) {
  char *mac_addr = mu_batman_adv_if_hwaddr("not_default", NULL);
  if (mac_addr) {
    printf("%s", mac_addr);
  } else {
    printf("ERROR");
  }
}

