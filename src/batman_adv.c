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
 * - Allocates memory temporarily using malloc().
 * - Is expected to free the malloc'd memory.
 * - Does not check that *error points to accessible memory.
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
          return line;
     } else {
          MU_SET_ERROR(error, errno);
          fclose(fp);
          return NULL;
     }
}

bool mu_batman_adv_if_available(char *interface_name, int *error)
{
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

     if (!fp) {
          MU_SET_ERROR(error, errno);
          free(bat_interface_operstate_file);
          free(bat_interface_carrier_file);
          return false;
     }

     if ((read = getline(&line, &len, fp)) != -1) {
          fclose(fp);
          if (!strcmp("false\n", line)) {
               free(bat_interface_operstate_file);
               free(bat_interface_carrier_file);
               free(line);
               return false;
          } else if (!strcmp("up\n", line)) {
               free(bat_interface_operstate_file);
          } else if (!strcmp("unknown\n", line)) {
               free(bat_interface_operstate_file);
          } else {
               free(bat_interface_operstate_file);
               free(bat_interface_carrier_file);
               free(line);
               return false;
          }
     } else {
          MU_SET_ERROR(error, errno);
          free(bat_interface_operstate_file);
          free(bat_interface_carrier_file);
          fclose(fp);
          return false;
     }

     fp = fopen (bat_interface_carrier_file, "r");

     if (!fp) {
          MU_SET_ERROR(error, errno);
          free(bat_interface_carrier_file);
          free(line);
          return false;
     }

     if ((read = getline(&line, &len, fp)) != -1) {
          fclose(fp);
          if (!strcmp("1\n", line)) {
               free(bat_interface_carrier_file);
               free(line);
               return true;
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
}

#endif                          /* __linux */
