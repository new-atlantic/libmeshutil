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

/*
 * Helper functions for the Linux environment.
 */

#ifndef MESHUTIL_LINUX_H
#define MESHUTIL_LINUX_H 1

#ifdef __linux

bool mu_linux_debugfs_mounted(void);

char* mu_linux_debugfs_mount_point(void);

#endif                          /* __linux */
#endif                          /* MESHUTIL_BATMAN_ADV_H */
