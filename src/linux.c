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

#include "meshutil.h"
#include "linux.h"

#define PROC_MOUNTS_PATH "/proc/mounts"

/* Implementation notes:
 * - Parses /proc/mounts.
 */
bool mu_linux_debugfs_mounted(int *error)
{
   MU_SET_ERROR(error, 0);

   FILE *fp;
   char *line = NULL;
   size_t len = 0;
   ssize_t read;

   fp = fopen (PROC_MOUNTS_PATH, "r");

   if (!fp) {
      MU_SET_ERROR(error, errno);
      return false;
   }

   while ((read = getline(&line, &len, fp)) != -1) {
      // Terminate *line after mount type field.
      *strchr (strchr (strchr (line, ' ') + 1, ' ') + 1, ' ') = '\0';
      // Test for debugfs only in the mount type field.
      if (strstr(strchr (strchr (line, ' ') + 1, ' '), "debugfs")) {
         free (line);
         fclose (fp);
         return true;
      }
   }

   free(line);
   fclose (fp);
   return false;
}

/* Implementation notes:
 * - Parses /proc/mounts.
 */
char* mu_linux_debugfs_mount_point(int *error)
{
   MU_SET_ERROR(error, 0);

   FILE *fp;
   char *line = NULL;
   size_t len = 0;
   ssize_t read;

   char *mount_point = NULL;
   char *tmp_line = NULL;

   fp = fopen (PROC_MOUNTS_PATH, "r");

   if (!fp) {
      MU_SET_ERROR(error, errno);
      return NULL;
   }

   while ((read = getline(&line, &len, fp)) != -1) {
      // Terminate *line after mount type field.
      *strchr (strchr (strchr (line, ' ') + 1, ' ') + 1, ' ') = '\0';
      // Test for debugfs only in the mount type field.
      if (strstr(strchr (strchr (line, ' ') + 1, ' '), "debugfs")) {
         tmp_line = strchr(line, ' ');

         if(!tmp_line) {
            MU_SET_ERROR(error, errno);
            free (line);
            fclose (fp);
            return NULL;
         }

         tmp_line = tmp_line + 1;

         *strchr(tmp_line, ' ') = '\0'; // End the string. Next whitespace from
                                        // mount path.

         /// TODO: Unescape unicode sequences in mount point (e.g. whitespace)
         mount_point = calloc (strlen (tmp_line) + 1, sizeof (char));
         strncpy (mount_point, tmp_line, strlen (tmp_line));
         free (line);
         fclose (fp);
         return mount_point;
      }
   }

   free (line);
   fclose (fp);
   return NULL;
}

#endif                          /* __linux */
