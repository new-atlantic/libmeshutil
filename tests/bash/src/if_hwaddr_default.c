#include <stdbool.h>
#include <stdio.h>

#include "batman_adv.h"

int main (void) {
  char *mac_addr = mu_batman_adv_if_hwaddr(NULL, NULL);
  if (mac_addr) {
    printf("%s", mac_addr);
  } else {
    printf("ERROR");
  }
}
