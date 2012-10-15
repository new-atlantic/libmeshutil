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

/// @file batman_adv.c

#ifdef __linux

#define _XOPEN_SOURCE 700

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>

#include "meshutil.h"
#include "batman_adv.h"

/* Implementation notes:
 *
 * - Checks for the availability of the file batman-adv.ko in the module
 *   directory of the currently loaded kernel.
 */
bool mu_batman_adv_kmod_available(int *error)
{
     MU_SET_ERROR(error, 0);

     const char *modules_root = "/lib/modules/";
     const char *module_path = "/kernel/net/batman-adv/batman-adv.ko";
     struct utsname system_version_info;
     size_t module_name_length;
     char *module_name;

     if (!uname(&system_version_info)) {
          module_name_length = (strlen(modules_root)
                                + strlen(system_version_info.release)
                                + strlen(module_path));

          module_name = calloc(module_name_length + 1, sizeof(char));

          strcat(module_name, modules_root);
          strcat(module_name, system_version_info.release);
          strcat(module_name, module_path);

          if (!access(module_name, F_OK)) {
               free(module_name);
               return true;
          } else {
               if (errno == ENOENT) {
                    free(module_name);
                    return false;
               } else {
                    MU_SET_ERROR(error, errno);
                    free(module_name);
                    return false;
               }
          }
     } else {
          MU_SET_ERROR(error, errno);
          return false;
     }
}

bool mu_batman_adv_kmod_loaded(int *error)
{
     MU_SET_ERROR(error, 0);

     // FIXME: Checking the sys filesystem should work since 2010.0.0, not before.
     if (!access("/sys/module/batman_adv/version", F_OK)) {
          return true;
     } else {
          if (errno == ENOENT)
               return false;
          else {
               MU_SET_ERROR(error, errno);
               return false;
          }
     }
}

char *mu_batman_adv_kmod_version(int *error)
{
     MU_SET_ERROR(error, 0);

     FILE *fp;
     char *line = NULL;
     size_t len = 0;
     ssize_t read;

     fp = fopen ("/sys/module/batman_adv/version", "r");

     if (!fp) {
          MU_SET_ERROR(error, errno);
          return NULL;
     }

     if ((read = getline(&line, &len, fp)) != -1) {
          fclose(fp);
          line[strlen(line)-1] = '\0';
          return line;
     } else {
          MU_SET_ERROR(error, errno);
          fclose(fp);
          return NULL;
     }
}

bool mu_batman_adv_if_available(char *interface_name, int *error)
{
     MU_SET_ERROR(error, 0);

     char *bat_interface_path_root = "/sys/devices/virtual/net/";
     char *bat_interface_path;

     if (!interface_name) {
          bat_interface_path = calloc(strlen(bat_interface_path_root)
                                      + strlen("bat0") + 1, sizeof(char));
          //FIXME: Check if calloc succeeded.
          strcat(bat_interface_path, bat_interface_path_root);
          strcat(bat_interface_path, "bat0");

     } else {
          bat_interface_path = calloc(strlen(bat_interface_path_root)
                                      + strlen(interface_name) + 1,
                                      sizeof(char));
          //FIXME: Check if calloc succeeded.
          strcat(bat_interface_path, bat_interface_path_root);
          strcat(bat_interface_path, interface_name);
     }

	   // Checking the sys filesystem should work since 2010.0.0, not before.
	   if (!access (bat_interface_path, F_OK)) {
	        free(bat_interface_path);
		      return true;
	   } else {
		      if (errno == ENOENT) {
		           free(bat_interface_path);
			         return false;
		      } else {
    			     MU_SET_ERROR(error, errno);
		           free(bat_interface_path);
			         return false;
			    }
		 }
}

bool mu_batman_adv_if_up(char *interface_name, int *error)
{
     MU_SET_ERROR(error, 0);

     char *bat_interface_path_root = "/sys/devices/virtual/net/";
     char *bat_interface_operstate_file;
     char *bat_interface_carrier_file;

     if (!interface_name) {
          bat_interface_operstate_file = calloc(strlen(bat_interface_path_root)
                                         + strlen("bat0") + strlen("/operstate")
                                         + 1, sizeof(char));
          //FIXME: Check if calloc succeeded.
          bat_interface_carrier_file = calloc(strlen(bat_interface_path_root)
                                       + strlen("bat0") + strlen("/carrier")
                                       + 1, sizeof(char));
          //FIXME: Check if calloc succeeded.
          strcat(bat_interface_operstate_file, bat_interface_path_root);
          strcat(bat_interface_operstate_file, "bat0/operstate");
          strcat(bat_interface_carrier_file, bat_interface_path_root);
          strcat(bat_interface_carrier_file, "bat0/carrier");

     } else {
          bat_interface_operstate_file = calloc(strlen(bat_interface_path_root)
                                         + strlen(interface_name)
                                         + strlen("/operstate")
                                         + 1,
                                         sizeof(char));
          //FIXME: Check if calloc succeeded.
          bat_interface_carrier_file = calloc(strlen(bat_interface_path_root)
                                         + strlen(interface_name)
                                         + strlen("/carrier")
                                         + 1,
                                         sizeof(char));
          //FIXME: Check if calloc succeeded.
          strcat(bat_interface_operstate_file, bat_interface_path_root);
          strcat(bat_interface_operstate_file, interface_name);
          strcat(bat_interface_operstate_file, "/operstate");
          strcat(bat_interface_carrier_file, bat_interface_path_root);
          strcat(bat_interface_carrier_file, interface_name);
          strcat(bat_interface_carrier_file, "/carrier");

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

     if ((read = getline(&line, &len, fp)) != -1) {
          fclose(fp);
          if (!strcmp("false\n", line)) {
               free(bat_interface_carrier_file);
               free(line);
               return false;
          } else if (!strcmp("up\n", line)) {
               ;
          } else if (!strcmp("unknown\n", line)) {
               ;
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

char *mu_batman_adv_if_hwaddr(char *interface_name, int *error)
{
     MU_SET_ERROR(error, 0);

     char *bat_interface_path_root = "/sys/devices/virtual/net/";
     char *bat_interface_address;

     if (!interface_name) {
          bat_interface_address = calloc(strlen(bat_interface_path_root)
                                      + strlen("bat0/address") + 1,
                                      sizeof(char));
          //FIXME: Check if calloc succeeded.
          strcat(bat_interface_address, bat_interface_path_root);
          strcat(bat_interface_address, "bat0/address");

     } else {
          bat_interface_address = calloc(strlen(bat_interface_path_root)
                                      + strlen(interface_name)
                                      + strlen("/address")
                                      + 1,
                                      sizeof(char));
          //FIXME: Check if calloc succeeded.
          strcat(bat_interface_address, bat_interface_path_root);
          strcat(bat_interface_address, interface_name);
          strcat(bat_interface_address, "/address");
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
          line[strlen(line)-1] = '\0';
          return line;
     } else {
          MU_SET_ERROR(error, errno);
          fclose(fp);
          return NULL;
     }


}

unsigned int mu_batman_adv_mesh_n_nodes(char *interface_name, int *error)
{
     MU_SET_ERROR(error, 0);

     //FIXME: function should dynamically determine the mount path of debugfs
     //       and not assume /sys/kernel/debug.

     char *bat_interface_debug_path_root = "/sys/kernel/debug/batman_adv/";
     char *bat_interface_originators_file;

     if (!interface_name) {
          bat_interface_originators_file = calloc(
                                    strlen(bat_interface_debug_path_root)
                                    + strlen("bat0")
                                    + strlen("/originators") + 1,
                                    sizeof(char));
          //FIXME: Check if calloc succeeded.
          strcat(bat_interface_originators_file, bat_interface_debug_path_root);
          strcat(bat_interface_originators_file, "bat0/originators");

     } else {
          bat_interface_originators_file =
                                  calloc(strlen(bat_interface_debug_path_root)
                                  + strlen(interface_name)
                                  + strlen("/originators")
                                  + 1, sizeof(char));
          //FIXME: Check if calloc succeeded.
          strcat(bat_interface_originators_file, bat_interface_debug_path_root);
          strcat(bat_interface_originators_file, interface_name);
          strcat(bat_interface_originators_file, "/originators");
     }


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
          if (!strcmp(line, "No batman nodes in range ...\n")) {
               free(line);
               fclose(fp);
               return 1; // Count self as one node.
          }
     }

     free(line);
     fclose(fp);
     return counter - 2 + 1; // First two lines are header lines + self.
}

struct mu_bat_mesh_node *mu_batman_adv_mesh_node_addresses(
                                                           char *interface_name,
                                                           int  *n_nodes,
                                                           int  *error)
{
     MU_SET_ERROR(error, 0);

     //FIXME: function should dynamically determine the mount path of debugfs
     //       and not assume /sys/kernel/debug.

     char *bat_interface_debug_path_root = "/sys/kernel/debug/batman_adv/";
     char *bat_interface_originators_file;

     if (!interface_name) {
          bat_interface_originators_file = calloc(
                                    strlen(bat_interface_debug_path_root)
                                    + strlen("bat0")
                                    + strlen("/originators") + 1,
                                    sizeof(char));
          //FIXME: Check if calloc succeeded.
          strcat(bat_interface_originators_file, bat_interface_debug_path_root);
          strcat(bat_interface_originators_file, "bat0/originators");

     } else {
          bat_interface_originators_file =
                                  calloc(strlen(bat_interface_debug_path_root)
                                  + strlen(interface_name)
                                  + strlen("/originators")
                                  + 1, sizeof(char));
          //FIXME: Check if calloc succeeded.
          strcat(bat_interface_originators_file, bat_interface_debug_path_root);
          strcat(bat_interface_originators_file, interface_name);
          strcat(bat_interface_originators_file, "/originators");
     }

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

     *n_nodes = 0;
     int counter = 0;

     while ((read = getline(&line, &len, fp)) != -1) {
          counter++;
          if (!strcmp(line, "No batman nodes in range ...\n")) {
               free(line);
               fclose(fp);
               *n_nodes = 0;
               return NULL;
          }
          if (counter > 2) {
               *n_nodes += 1;
               current_node = malloc(sizeof(struct mu_bat_mesh_node));
               current_node->next = first_node;
               memcpy(current_node->mac_addr, line, sizeof(char) * 17);
               current_node->mac_addr[17] = '\0';
               first_node = current_node;
          }
     }

     free(line);
     fclose(fp);
     return first_node;
}
#endif                          /* __linux */
