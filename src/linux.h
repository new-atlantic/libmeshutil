/** @file linux.h
 * Internal Linux specific non-mesh API
 */

/*
 * Copyright © 2012 Torsti Schulz
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

#ifndef MESHUTIL_LINUX_H
#define MESHUTIL_LINUX_H 1

#ifdef __linux

/*******************************************************************************
*   HEADER FILES                                                               *
*******************************************************************************/

#include <stdbool.h>

/*******************************************************************************
*   PRIVATE API FUNCTION DECLARATIONS                                          *
*******************************************************************************/

/**
 * @brief PRIVATE Test whether the Linux debug filesystem is mounted.
 *
 * @param *error [out] For setting error codes on function failure.
 *
 * @retval true  debugfs is mounted
 * @retval false debugfs is not mounted and/or an error occurred.
 */
bool
mu_linux_debugfs_mounted(int *const error)
__attribute__ ((visibility("hidden")));

/**
 * @brief PRIVATE Get the path to the mount point of the Linux debug filesystem.
 *
 * If debugfs is mounted at multiple mount points, returns the first one.
 *
 * @param *error [out] For setting error codes on function failure.
 *
 * @return Pointer to string containing the mount path.
 * @retval NULL debugfs not mounted or other error occurred.
 */
char
*mu_linux_debugfs_mount_point(int *const error)
__attribute__ ((visibility("hidden")));

#endif                          /* __linux */
#endif                          /* MESHUTIL_BATMAN_ADV_H */
