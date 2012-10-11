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
          return line;
     } else {
          MU_SET_ERROR(error, errno);
          return NULL;
     }
}

#endif                          /* __linux */
