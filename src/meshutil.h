/** @file meshutil.h
 * Common definitions for meshutil
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

#ifndef MESHUTIL_H
#define MESHUTIL_H 1

/*******************************************************************************
*   FUNCTION MACROS                                                            *
*******************************************************************************/

/**
 * @brief Macro to prevent assigning error values to a NULL pointer.
 * 
 * Most functions in libmeshutil accept an int *error function argument.
 * These functions accept also NULL, in which case the error value is ignored.
 * MU_SET_ERROR is used in the meshutil library to avoid assigning to th
 * NULL pointer. It is not meant to be used by client applications.
 */
#define MU_SET_ERROR(eptr, eval) if ( eptr ) { *eptr = eval; }

#endif                          /* MESHUTIL_H */
