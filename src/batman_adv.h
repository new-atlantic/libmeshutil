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

/* Abbreviations used:
 *
 *     batman_adv:  B.A.T.M.A.N. advanced
 *     badv:        B.A.T.M.A.N. advanced
 *     hwaddr:      MAC address of network interface
 *     if:          network interface
 *     kmod:        kernel module
 *     mac_addr:    MAC address
 *     mu:          meshutil
 */

#ifndef MESHUTIL_BATMAN_ADV_H
#define MESHUTIL_BATMAN_ADV_H 1

#ifdef __linux

#define MAC_ADDR_CHAR_REPRESENTATION_LEN 17

#include <stdbool.h>

/// Struct for a linked list of node MAC addresses.
/// TODO: Representing MAC addresses as a 18 byte string is wasteful.
struct mu_bat_mesh_node {
          char              mac_addr[MAC_ADDR_CHAR_REPRESENTATION_LEN + 1];
   struct mu_bat_mesh_node *next;
};

/**
 * @brief Test whether the batman_adv kernel module is available on the
 *        current kernel.
 *
 * @param  *error See documentation for *error.
 *
 * @retval  true  The module is available.
 * @retval  false The module is not available. Also returned if an error
 *                occurred.
 */
bool
__attribute__ ((visibility("default")))
mu_badv_kmod_available(int *const error);

/**
 * @brief Test whether the batman_adv kernel module is loaded.
 *
 * @param  *error See documentation for *error.
 *
 * @retval  true  The module is loaded.
 * @retval  false The module is not loaded. Also returned if an error
 *                occurred.
 */
bool
__attribute__ ((visibility("default")))
mu_badv_kmod_loaded(int *const error);

/**
 * @brief Get the kernel module version.
 *
 * @param  *error See documentation for *error.
 *
 * @return Pointer to kernel module version string. The pointer has to be
 * free()'d by the caller.
 * @retval NULL Returned on failure.
 */
char
__attribute__ ((visibility("default")))
*mu_badv_kmod_version(int *const error);

/**
 * @brief Check whether a bat interface is available.
 *
 * @param *error See documentation for *error.
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @retval true  The interface is available.
 * @retval false The interface is not available. Also returned if an error
 *               ocurred.
 */
bool
__attribute__ ((visibility("default")))
mu_badv_if_available(const char *const interface_name, int *const error);

/**
 * @brief Check whether a bat interface is up.
 *
 * @param *error See documentation for *error.
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @retval true  The interface is up.
 * @retval false The interface is not up. Also returned if an error
 *               ocurred.
 */
bool
__attribute__ ((visibility("default")))
mu_badv_if_up(const char *const interface_name, int *const error);

/**
 * @brief Get the MAC address of the bat interface.
 *
 * @param *error See documentation for *error.
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @return Pointer to interface MAC address string. The pointer has to be
 * free()'d by the caller.
 * @retval NULL Returned on failure.
 */
char
__attribute__ ((visibility("default")))
*mu_badv_if_hwaddr(const char *const interface_name, int *const error);

/**
 * @brief Get the number of nodes in the mesh.
 *
 * @param *error See documentation for *error.
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @return Number of nodes in the mesh. Count includes self.
 * @retval 0 Zero returned on error.
 */
unsigned int
__attribute__ ((visibility("default")))
mu_badv_mesh_n_nodes(const char *const interface_name, int *const error);

/**
 * @brief Get the addresses of the nodes in the mesh.
 *
 * @param *error See documentation for *error.
 * @param *n_nodes Pointer to an integer that will hold the number of nodes
 *                 addresses the mesh. Does not include self.
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @return Pointer to a linked list of nodes in the mesh. The links in this
 *         list have to be free()'d by the caller.
 * @retval NULL Returned on failure or when no nodes available.
 */
struct mu_bat_mesh_node
__attribute__ ((visibility("default")))
*mu_badv_mesh_node_addresses(const char *const interface_name,
                                   int  *const n_nodes,
                                   int  *const error);

/**
 * @brief Get the addresses of neighbouring nodes (next hops) in the mesh.
 *
 * @param *error See documentation for *error.
 * @param *n_nodes Pointer to an integer that will hold the number of next hop
 *                 addresses the mesh.
 * @param  potential If true, includes potential next hops (i.e. routes not
 *                   used, but available).
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @return Pointer to a linked list of next hop addresses. The links in this
 *         list have to be free()'d by the caller.
 * @retval NULL Returned on failure or when no nodes available.
 */
struct mu_bat_mesh_node
__attribute__ ((visibility("default")))
*mu_badv_next_hop_addresses(const char *const interface_name,
                            const bool        potential,
                                  int  *const n_nodes,
                                  int  *const error);

/**
 * @brief Checks whether a node is a next hop
 *
 * @param *error See documentation for *error.
 * @param  potential If true, includes potential next hops (i.e. routes not
 *                   used, but available).
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @retval true Node is a next hop.
 * @retval false Node is not a next hop. Also returned on error.
 *
 */
bool
__attribute__ ((visibility("default")))
mu_badv_node_is_next_hop(const        char             *const interface_name,
                         const struct mu_bat_mesh_node *const node,
                         const        bool                    potential,
                                      int              *const error);

/**
 * @brief Checks via which network interface a node is accessible.
 *
 * @param *error See documentation for *error.
 * @param *node  The node that is to be checked.
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @return Pointer to string containing interface name.
 * @retval NULL An error occurred.
 */
char
__attribute__ ((visibility("default")))
*mu_badv_node_accessible_via_if(
   const        char             *const interface_name,
   const struct mu_bat_mesh_node *const node,
                int              *const error
   );

/**
 * @brief Checks when a node was last seen.
 *
 * @param *error See documentation for *error.
 * @param *node  The node that is to be checked.
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 */
double
__attribute__ ((visibility("default")))
mu_badv_node_last_seen(const        char             *const interface_name,
                       const struct mu_bat_mesh_node *const node,
                                    int              *const error);

/**
 * @brief Checks what is the next hop node to reach *node.
 *
 * If self returns the passed pointer! So always the return value
 * before calling free() to avoid trying to free() allocated memory twice.
 *
 * @param *error See documentation for *error.
 * @param *node  The node that is to be checked.
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @return Pointer to the next hop node.
 */
struct mu_bat_mesh_node
__attribute__ ((visibility("default")))
*mu_badv_node_next_hop(const        char             *const interface_name,
                       const struct mu_bat_mesh_node *const node,
                                    int              *const error);

#endif                          /* __linux */
#endif                          /* MESHUTIL_BATMAN_ADV_H */
