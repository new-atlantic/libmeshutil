/*
 * Copyright 2012 Torsti Schulz
 *
 * This file is part of the meshutil library.
 *
 * libmeshutil is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libmeshutil is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/// @file

#ifdef __linux

#define _XOPEN_SOURCE 700

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>

#include "meshutil.h"
#include "linux.h"
#include "batman_adv.h"

/// String in a batman_adv interface originator table indicating that no
/// nodes are available.
#define NO_NODES_IN_RANGE_STR "No batman nodes in range ...\n"
/// Path to the root of the directory containting Linux kernel modules.
#define KERNEL_MODULE_ROOT "/lib/modules/"
/// Path to batman_adv kernel module under a specific kernel version
/// module directory.
#define BATMAN_ADV_KMOD_PATH "/kernel/net/batman-adv/batman-adv.ko"
/// Path to the version of the batman_adv kernel module in the sys filesystem.
#define BATMAN_ADV_KMOD_VERSION_PATH "/sys/module/batman_adv/version"
/// Path to the sys filesystem directory containing virtual network devices.
#define VIRTUAL_NETWORK_IF_PATH_ROOT "/sys/devices/virtual/net/"
/// Originators file field header for interface names
#define BATMAN_ADV_ORIGINATORS_IFNAME_HEADER "outgoingIF"

/*** STATIC FUNCTIONS ***/

static bool interface_dependent_path(const char  *const path_root,
                                     const char  *const interface_name,
                                     const char  *const suffix,
                                           char **const path_string,
                                           int   *const error)
{
   if (!path_root) {
      return false;
   }

   if (!interface_name) {
      if(!suffix) {
         *path_string = calloc(strlen(path_root) + 1
                              + strlen("bat0") + 1, sizeof(char));
         if(!*path_string) {
            MU_SET_ERROR(error, errno);
            return false;
         }
         strcat(*path_string, path_root);
         strcat(*path_string, "/");
         strcat(*path_string, "bat0");
         return true;
      } else {
         *path_string = calloc(strlen(path_root)+ 1
                              + strlen("bat0")
                              + strlen(suffix) + 1, sizeof(char));
         if(!*path_string) {
            MU_SET_ERROR(error, errno);
            return false;
         }
         strcat(*path_string, path_root);
         strcat(*path_string, "/");
         strcat(*path_string, "bat0");
         strcat(*path_string, suffix);
         return true;
      }

   } else {
      if(!suffix) {
         *path_string = calloc(strlen(path_root) + 1
                              + strlen(interface_name) + 1,
                              sizeof(char));
         if(!*path_string) {
            MU_SET_ERROR(error, errno);
            return false;
         }

         strcat(*path_string, path_root);
         strcat(*path_string, "/");
         strcat(*path_string, interface_name);
         return true;
      } else {
         *path_string = calloc(strlen(path_root) + 1
                              + strlen(interface_name)
                              + strlen(suffix) + 1,
                              sizeof(char));
         if(!*path_string) {
            MU_SET_ERROR(error, errno);
            return false;
         }

         strcat(*path_string, path_root);
         strcat(*path_string, "/");
         strcat(*path_string, interface_name);
         strcat(*path_string, suffix);
         return true;
      }
   }
}

/*** FUNCTION REPLACEMENT MACROS ***/

#define BATMAN_ADV_ORIGINATORS_FILE                               \
   char *debugfs_root = mu_linux_debugfs_mount_point(NULL);       \
   char *batman_debugfs_dir = NULL;                               \
                                                                  \
   if (!debugfs_root) {                                           \
      MU_SET_ERROR(error, errno);                                 \
      return 0;                                                   \
   }                                                              \
                                                                  \
   batman_debugfs_dir = calloc (strlen (debugfs_root)             \
                                + strlen("/batman_adv/") + 1,     \
                                sizeof (char));                   \
   if (!batman_debugfs_dir) {                                     \
      MU_SET_ERROR(error, errno);                                 \
      free (debugfs_root);                                        \
      return 0;                                                   \
   }                                                              \
                                                                  \
   strcat (batman_debugfs_dir, debugfs_root);                     \
   strcat (batman_debugfs_dir, "/batman_adv/");                   \
   free (debugfs_root);                                           \
                                                                  \
   if (!interface_dependent_path(batman_debugfs_dir,              \
                                 interface_name,                  \
                                 "/originators",                  \
                                 &bat_interface_originators_file, \
                                 error)) {                        \
      free (batman_debugfs_dir);                                  \
      return 0;                                                   \
   }                                                              \
                                                                  \
   free (batman_debugfs_dir);                                     \

/*** API FUNCTIONS ***/

/* Implementation notes:
 * - Checks for the availability of the file batman-adv.ko in the module
 *   directory of the currently loaded kernel.
 */
bool mu_badv_kmod_available(int *const error)
{
   MU_SET_ERROR(error, 0);

   struct  utsname system_version_info;
   size_t  module_name_length;
   char   *module_name = NULL;

   if (!uname(&system_version_info)) {
      module_name_length = (strlen(KERNEL_MODULE_ROOT)
                            + strlen(system_version_info.release)
                            + strlen(BATMAN_ADV_KMOD_PATH));

      module_name = calloc(module_name_length + 1, sizeof(char));
      if(!module_name) {
         MU_SET_ERROR(error, errno);
         return false;
      }

      strcat(module_name, KERNEL_MODULE_ROOT);
      strcat(module_name, system_version_info.release);
      strcat(module_name, BATMAN_ADV_KMOD_PATH);

      if (!access(module_name, F_OK)) {
         free(module_name);
         return true;
      } else if (errno == ENOENT) {
         free(module_name);
         return false;
      } else {
         MU_SET_ERROR(error, errno);
         free(module_name);
         return false;
      }
   } else {
      MU_SET_ERROR(error, errno);
      return false;
   }
}

/* Implementation notes:
 * - Checks that the kernel module version file is available under sysfs.
 */
bool mu_badv_kmod_loaded(int *const error)
{
   MU_SET_ERROR(error, 0);

   /// TODO: VERIFY! Checking the sys filesystem should work since 2010.0.0,
   ///       not before.
   if (!access(BATMAN_ADV_KMOD_VERSION_PATH, F_OK)) {
      return true;
   } else if (errno == ENOENT) {
      return false;
   } else {
      MU_SET_ERROR(error, errno);
      return false;
   }
}

/* Implementation notes:
 * - Reads the kernel module version file from sysfs.
 */
char *mu_badv_kmod_version(int *const error)
{
   MU_SET_ERROR(error, 0);

   FILE *fp;
   char *line = NULL;
   size_t len = 0;
   ssize_t read;

   fp = fopen (BATMAN_ADV_KMOD_VERSION_PATH, "r");

   if (!fp) {
      MU_SET_ERROR(error, errno);
      return NULL;
   }

   if ((read = getline(&line, &len, fp)) != -1) {
      fclose(fp);
      line[strlen(line)-1] = '\0'; // Remove new-line character.
      return line;
   } else {
      MU_SET_ERROR(error, errno);
      fclose(fp);
      return NULL;
   }
}

/* Implementation notes:
 * - Checks that the bat interface directory is available under sysfs.
 */
bool mu_badv_if_available(const char *const interface_name, int *const error)
{
   MU_SET_ERROR(error, 0);

   char *bat_interface_path = NULL;

   if (!interface_dependent_path(VIRTUAL_NETWORK_IF_PATH_ROOT,
                                 interface_name,
                                 NULL,
                                 &bat_interface_path,
                                 error)) {
      return false;
   }

   // TODO: Checking the sys filesystem should work since 2010.0.0, not before.
   if (!access (bat_interface_path, F_OK)) {
      free(bat_interface_path);
      return true;
   } else if (errno == ENOENT) {
      free(bat_interface_path);
      return false;
   } else {
      MU_SET_ERROR(error, errno);
      free(bat_interface_path);
      return false;
   }
}

/* Implementation notes:
 * - Reads the operstate and carrier files in the bat interface directory
 *   under sysfs.
 */
bool mu_badv_if_up(const char *const interface_name, int *const error)
{
   MU_SET_ERROR(error, 0);

   char *bat_interface_operstate_file = NULL;
   char *bat_interface_carrier_file   = NULL;

   if (!interface_dependent_path(VIRTUAL_NETWORK_IF_PATH_ROOT,
                                 interface_name,
                                 "/operstate",
                                 &bat_interface_operstate_file,
                                 error)) {
      return false;
   }

   if (!interface_dependent_path(VIRTUAL_NETWORK_IF_PATH_ROOT,
                                 interface_name,
                                 "/carrier",
                                 &bat_interface_carrier_file,
                                 error)) {
      free (bat_interface_operstate_file);
      return false;
   }

   FILE *fp;
   char *line = NULL;
   size_t len = 0;
   ssize_t read;

   fp = fopen (bat_interface_operstate_file, "r");
   free(bat_interface_operstate_file);

   if (!fp) {
      MU_SET_ERROR(error, errno);
      free(bat_interface_carrier_file);
      return false;
   }

   // Check operstate.
   if ((read = getline(&line, &len, fp)) != -1) {
      fclose(fp);
      if (!strcmp("false\n", line)) {
         free(bat_interface_carrier_file);
         free(line);
         return false;
      } else if (!strcmp("up\n", line)) {
         ; // We still need to check carrier.
      } else if (!strcmp("unknown\n", line)) {
         ; // We still need to check carrier.
      } else {
         free(bat_interface_carrier_file);
         free(line);
         return false;
      }
   } else {
      MU_SET_ERROR(error, errno);
      free(bat_interface_carrier_file);
      fclose(fp);
      return false;
   }

   fp = fopen (bat_interface_carrier_file, "r");
   free(bat_interface_carrier_file);

   if (!fp) {
      MU_SET_ERROR(error, errno);
      free(line);
      return false;
   }

   if ((read = getline(&line, &len, fp)) != -1) {
      fclose(fp);
      if (!strcmp("1\n", line)) {
         free(line);
         return true;
      } else {
         free(line);
         return false;
      }
   } else {
      MU_SET_ERROR(error, errno);
      fclose(fp);
      return false;
   }
}

/* Implementation notes:
 * - Reads the address file in the bat interface directory under sysfs.
 */
char *mu_badv_if_hwaddr(const char *const interface_name, int *const error)
{
   MU_SET_ERROR(error, 0);

   char *bat_interface_address = NULL;

   if (!interface_dependent_path(VIRTUAL_NETWORK_IF_PATH_ROOT,
                                 interface_name,
                                 "/address",
                                 &bat_interface_address,
                                 error)) {
      return false;
   }

   FILE *fp;
   char *line = NULL;
   size_t len = 0;
   ssize_t read;

   fp = fopen (bat_interface_address, "r");
   free(bat_interface_address);

   if (!fp) {
      MU_SET_ERROR(error, errno);
      return NULL;
   }


   if ((read = getline(&line, &len, fp)) != -1) {
      fclose(fp);
      line[strlen(line)-1] = '\0'; // Strip trailing new-line.
      return line;
   } else {
      MU_SET_ERROR(error, errno);
      fclose(fp);
      return NULL;
   }
}

/* Implementation notes:
 * - Parses the originators table in the bat interface directory under sysfs.
 */
/// TODO: Does the return value have to be unsigned?
unsigned int mu_badv_mesh_n_nodes(const char *const interface_name,
                                        int  *const error)
{
   MU_SET_ERROR(error, 0);
   char *bat_interface_originators_file = NULL;

   BATMAN_ADV_ORIGINATORS_FILE;

   FILE *fp;
   char *line = NULL;
   size_t len = 0;
   ssize_t read;

   fp = fopen (bat_interface_originators_file, "r");
   free(bat_interface_originators_file);

   if (!fp) {
      MU_SET_ERROR(error, errno);
      return 0;
   }

   unsigned int counter = 0;

   while ((read = getline(&line, &len, fp)) != -1) {
      counter++;
      if (!strcmp(line, NO_NODES_IN_RANGE_STR)) {
         free(line);
         fclose(fp);
         return 1; // Count self as one node.
      }
   }

   free(line);
   fclose(fp);
   return counter - 2 + 1; // Remove two header lines and add self.
}

/* Implementation notes:
 * - Parses the originators table in the bat interface directory under sysfs.
 */
struct mu_bat_mesh_node *mu_badv_mesh_node_addresses(
   const char *const interface_name,
         int  *const n_nodes,
         int  *const error)
{
   MU_SET_ERROR(error, 0);

   char *bat_interface_originators_file = NULL;

   BATMAN_ADV_ORIGINATORS_FILE;

   FILE *fp;
   char *line = NULL;
   size_t len = 0;
   ssize_t read;
   struct mu_bat_mesh_node *first_node = NULL;
   struct mu_bat_mesh_node *current_node = NULL;

   fp = fopen (bat_interface_originators_file, "r");
   free(bat_interface_originators_file);

   if (!fp) {
      MU_SET_ERROR(error, errno);
      return NULL;
   }

   if(n_nodes) {
      *n_nodes = 0;
   }
   int counter = 0;

   while ((read = getline(&line, &len, fp)) != -1) {
      counter++;
      if (!strcmp(line, NO_NODES_IN_RANGE_STR)) {
         free(line);
         fclose(fp);
         if(n_nodes) {
            *n_nodes = 0; // This function does not count self.
         }
         return NULL;
      }

      if (counter > 2) {
         // Skip header lines.
         if(n_nodes) {
            *n_nodes += 1;
         }
         current_node = malloc(sizeof(struct mu_bat_mesh_node));
         current_node->next = first_node;
         /// TODO: Validate that a MAC-address is being read.
         memcpy(current_node->mac_addr, line,
                sizeof(char) * MAC_ADDR_CHAR_REPRESENTATION_LEN);
         current_node->mac_addr[MAC_ADDR_CHAR_REPRESENTATION_LEN] = '\0';
         first_node = current_node;
      }
   }

   free(line);
   fclose(fp);
   return first_node;
}

/* Implementation notes:
 * - Parses the originators table in the bat interface directory under sysfs.
 */
struct mu_bat_mesh_node *mu_badv_next_hop_addresses(
   const char *const interface_name,
   const bool        potential,
         int  *const n_nodes,
         int  *const error)
{
   MU_SET_ERROR(error, 0);

   char *bat_interface_originators_file = NULL;

   BATMAN_ADV_ORIGINATORS_FILE;

   FILE *fp;
   char *line = NULL;
   size_t len = 0;
   ssize_t read;
   struct mu_bat_mesh_node *first_node   = NULL;
   struct mu_bat_mesh_node *current_node = NULL;
   char *address_tmp = malloc(sizeof(char) * MAC_ADDR_CHAR_REPRESENTATION_LEN
                              + 1);

   fp = fopen (bat_interface_originators_file, "r");
   free(bat_interface_originators_file);

   if (!fp) {
      MU_SET_ERROR(error, errno);
      free (address_tmp);
      return NULL;
   }

   if(n_nodes) {
      *n_nodes = 0;
   }
   int counter = 0;
   bool duplicate = false;

   while ((read = getline(&line, &len, fp)) != -1) {
      #pragma message "TODO: Next hop retrieval not tested thoroughly."
      counter++;

      if (!strcmp(line, NO_NODES_IN_RANGE_STR)) {
         free(line);
         free(address_tmp);
         fclose(fp);
         if(n_nodes) {
            *n_nodes = 0;
         }
         return NULL;
      }

      if (counter > 2) {
         if(!potential) { // Looking only for actual next hops.
            memcpy(address_tmp, strchr(line, ')') + 2,
                   sizeof(char) * MAC_ADDR_CHAR_REPRESENTATION_LEN);
            address_tmp[MAC_ADDR_CHAR_REPRESENTATION_LEN] = '\0';

            current_node = first_node;
            while(current_node) { // Check if address already in linked list.
               if(!strncmp(current_node->mac_addr,
                           address_tmp,
                           MAC_ADDR_CHAR_REPRESENTATION_LEN)) {
                  duplicate = true;
                  current_node = NULL;
               } else {
                  current_node = current_node->next;
               }
            }

            if(!duplicate) {
               if(n_nodes) {
                  *n_nodes += 1;
               }
               current_node = malloc(sizeof(struct mu_bat_mesh_node));
               current_node->next = first_node;
               memcpy(current_node->mac_addr, address_tmp,
                      sizeof(char) * MAC_ADDR_CHAR_REPRESENTATION_LEN);
               current_node->mac_addr[MAC_ADDR_CHAR_REPRESENTATION_LEN] = '\0';
               first_node = current_node;
            } else {
               duplicate = false;
            }
         } else { // Looking for potential and actual next hops.
            char *tmp_line = NULL;

            tmp_line = strchr(line, ']');
            if(tmp_line) {
               tmp_line = tmp_line + 2;
            }

            do {
               if(tmp_line && strchr(tmp_line, '(')) { // Check that there is
                                                       // still a MAC address
                                                       // in tmp_line.
                  memcpy(address_tmp, tmp_line + 1,
                         sizeof(char) * MAC_ADDR_CHAR_REPRESENTATION_LEN);
                  address_tmp[MAC_ADDR_CHAR_REPRESENTATION_LEN] = '\0';
                  tmp_line = strchr(tmp_line, ')');
                  if(tmp_line) {
                     tmp_line = tmp_line + 1; // TODO: If we are at the last
                                              //       address this lands us on
                                              //       the last new-line char?!
                  }
               } else {
                  tmp_line = NULL;
               }

               current_node = first_node;
               while(current_node) { // Check if address already in linked list.
                  if(!strncmp(current_node->mac_addr, address_tmp,
                              MAC_ADDR_CHAR_REPRESENTATION_LEN)) {
                     duplicate = true;
                     current_node = NULL;
                  } else {
                     current_node = current_node->next;
                  }
               }

               if(!duplicate) {
                  if(n_nodes) {
                     *n_nodes += 1;
                  }
                  current_node = malloc(sizeof(struct mu_bat_mesh_node));
                  current_node->next = first_node;
                  memcpy(current_node->mac_addr, address_tmp,
                         sizeof(char) * MAC_ADDR_CHAR_REPRESENTATION_LEN);
                  current_node->mac_addr[MAC_ADDR_CHAR_REPRESENTATION_LEN] = '\0';
                  first_node = current_node;
               } else {
                  duplicate = false;
               }
            } while(tmp_line);
         }
      }
   }

   free(line);
   free(address_tmp);
   fclose(fp);
   return first_node;
}

/* Implementation notes:
 * - Compares a MAC address against mu_badv_next_hop_addresses.
 */
bool mu_badv_node_is_next_hop(
   const        char             *const interface_name,
   const struct mu_bat_mesh_node *const node,
   const        bool                    potential,
                int              *const error)
{
   MU_SET_ERROR(error, 0);

   struct mu_bat_mesh_node *next_hop_node = NULL;
   struct mu_bat_mesh_node *passed_node   = NULL;
   bool   node_status = false;

   if(!node) {
      /// TODO: Set error to indicate that pointer was NULL?
      return false;
   }

   next_hop_node = mu_badv_next_hop_addresses(interface_name,
                                              potential,
                                              NULL,
                                              error);
   if (!next_hop_node) {
      MU_SET_ERROR(error, errno);
      return false;
   } else {
      while(next_hop_node) {
         if(!strncmp(next_hop_node->mac_addr,
                     node->mac_addr,
                     MAC_ADDR_CHAR_REPRESENTATION_LEN)) {
            node_status = true;
         }
         passed_node = next_hop_node;
         next_hop_node = next_hop_node->next;
         free(passed_node);
      }

      return node_status;
   }
}

char *mu_badv_node_accessible_via_if(
   const        char             *const interface_name,
   const struct mu_bat_mesh_node *const node,
                int              *const error)
{
   MU_SET_ERROR(error, 0);

   if(!node) {
      /// TODO: Set error to indicate that pointer was NULL?
      return NULL;
   }

   char *bat_interface_originators_file = NULL;

   BATMAN_ADV_ORIGINATORS_FILE;

   FILE *fp;
   char *line = NULL;
   size_t len = 0;
   ssize_t read;

   char *iface = NULL;

   fp = fopen (bat_interface_originators_file, "r");
   free(bat_interface_originators_file);

   if (!fp) {
      MU_SET_ERROR(error, errno);
      return 0;
   }

   char* tmp_line = NULL;
   unsigned int counter = 0;

   while ((read = getline(&line, &len, fp)) != -1) {
      counter++;
      if (counter > 2) {
         if (!strncmp(line, node->mac_addr, MAC_ADDR_CHAR_REPRESENTATION_LEN)) {
            tmp_line = strchr(line, '[');
            if(!tmp_line) {
               MU_SET_ERROR(error, errno);
               free (line);
               fclose (fp);
               return NULL;
            }

            tmp_line = tmp_line + 1;

            *strchr(tmp_line, ']') = '\0'; // End the string.

            if (tmp_line[0] == ' ') { // If ifname preceded by space skip it!
               tmp_line = strrchr(tmp_line, ' ');
            }

            iface = calloc((
                     strnlen(tmp_line,
                             strlen(BATMAN_ADV_ORIGINATORS_IFNAME_HEADER)) + 1),
                     sizeof(char));
            strncpy(iface, tmp_line,
                       strnlen(tmp_line,
                       strlen(BATMAN_ADV_ORIGINATORS_IFNAME_HEADER)));
            tmp_line = NULL;
            free (line);
            fclose (fp);
            return iface;
         }
      }
   }

   free (line);
   fclose (fp);
   return NULL;
}

double mu_badv_node_last_seen(
   const        char             *const interface_name,
   const struct mu_bat_mesh_node *const node,
                int              *const error)
{
   MU_SET_ERROR(error, 0);

   if(!node) {
      /// TODO: Set error to indicate that pointer was NULL?
      return 0;
   }

   char *bat_interface_originators_file = NULL;

   BATMAN_ADV_ORIGINATORS_FILE;

   FILE *fp;
   char *line = NULL;
   size_t len = 0;
   ssize_t read;
   double  last_seen;

   fp = fopen (bat_interface_originators_file, "r");
   free(bat_interface_originators_file);

   if (!fp) {
      MU_SET_ERROR(error, errno);
      return 0;
   }

   unsigned int counter = 0;
   char* tmp_line = NULL;

   while ((read = getline(&line, &len, fp)) != -1) {
      counter++;
      if (counter > 2) {
         if (!strncmp(line, node->mac_addr, MAC_ADDR_CHAR_REPRESENTATION_LEN)) {
            tmp_line = strchr(line, ' ');
            if(!tmp_line) {
               MU_SET_ERROR(error, errno);
               free (line);
               fclose (fp);
               return 0;
            }

            *strchr(tmp_line, 's') = '\0'; // End the string.;

            tmp_line = strrchr(tmp_line, ' ') + 1; // Skip to last whitespace + 1
                                                   // before last seen value.

            if (sscanf(tmp_line, "%lf", &last_seen) != 1) {
               MU_SET_ERROR(error, errno);
               last_seen = 0;
            }

            tmp_line = NULL;
            free (line);
            fclose (fp);
            return last_seen;
         }
      }
   }

   free (line);
   fclose (fp);
   return 0;
}

struct mu_bat_mesh_node *mu_badv_node_next_hop(
   const        char             *const interface_name,
   const struct mu_bat_mesh_node *const node,
                int              *const error)
{
   MU_SET_ERROR(error, 0);

   if(!node) {
      /// TODO: Set error to indicate that pointer was NULL?
      return NULL;
   }

   char *bat_interface_originators_file = NULL;

   BATMAN_ADV_ORIGINATORS_FILE;

   struct mu_bat_mesh_node *next_hop_node = NULL;

   FILE *fp;
   char *line = NULL;
   size_t len = 0;
   ssize_t read;

   fp = fopen (bat_interface_originators_file, "r");
   free(bat_interface_originators_file);

   if (!fp) {
      MU_SET_ERROR(error, errno);
      return NULL;
   }

   unsigned int counter = 0;
   char address_tmp[MAC_ADDR_CHAR_REPRESENTATION_LEN + 1];

   while ((read = getline(&line, &len, fp)) != -1) {
      counter++;
      if (counter > 2) {
         if (!strncmp(line, node->mac_addr, MAC_ADDR_CHAR_REPRESENTATION_LEN)) {
            memcpy(address_tmp, strchr(line, ')') + 2,
                   sizeof(char) * MAC_ADDR_CHAR_REPRESENTATION_LEN);
            address_tmp[MAC_ADDR_CHAR_REPRESENTATION_LEN] = '\0';
            if (!strncmp(address_tmp, node->mac_addr,
                         MAC_ADDR_CHAR_REPRESENTATION_LEN)) {
               next_hop_node = (struct mu_bat_mesh_node *) node;
            } else {
               next_hop_node = malloc(sizeof(struct mu_bat_mesh_node));
               if(!next_hop_node) {
                  MU_SET_ERROR(error, errno);
                  fclose(fp);
                  free(line);
               }
               memcpy(next_hop_node->mac_addr, address_tmp,
                      sizeof(char) * MAC_ADDR_CHAR_REPRESENTATION_LEN);
               next_hop_node->mac_addr[MAC_ADDR_CHAR_REPRESENTATION_LEN] = '\0';
               next_hop_node->next = NULL;
            }
            break;
         }
      }
   }

   free (line);
   fclose (fp);
   return next_hop_node;
}

#endif                          /* __linux */
