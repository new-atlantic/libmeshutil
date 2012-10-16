#include <stdbool.h>
#include <stdio.h>

#include "batman_adv.h"

int main (void) {
   int number = 0;
   int error  = 0;
   struct mu_bat_mesh_node *node;

   node = mu_batman_adv_next_hop_addresses("not_default", false, &number, &error);
   if (!node) {
      if(!error) {
         printf("NO NODES");
      } else {
         printf("ERROR");
      }
   } else {
      printf("%d", number);
      while(node) {
         printf(" %s", node->mac_addr);
         node = node->next;
      }
   }
}
