/** @file batman_adv.h
 * meshutil API for B.A.T.M.A.N. advanced
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

/** @page pg_batman_adv_api   API for B.A.T.M.A.N. advanced
 *
 * B.A.T.M.A.N. advanced is a mesh routing protocol over Layer 2 of the network
 * stack. It is implemented as a Linux kernel module.
 *
 * Abbreviations used:
 *
 * * batman_adv, badv:  **B.A.T.M.A.N. advanced**
 * * hwaddr:            **MAC address of network interface**
 * * if:                **network interface**
 * * kmod:              **kernel module**
 * * mac_addr:          **MAC address**
 * * mu_:               **meshutil API prefix**
 *
 * interface_name
 *
 * Functions having the const char *const interface_name function parameter can
 * also be passed NULL in which case the default interface (bat0) is assumed.
 *
 * potential
 *
 * If potential is passed as true the function will count potential next hops
 * as next hops. Potential next hops are nodes which might be the first hop on
 * the way to reach another node (including the potential next hop itself), but
 * which at that moment are not being used for reasons determined by the routing
 * protocol.
 */

#ifndef MESHUTIL_BATMAN_ADV_H
#define MESHUTIL_BATMAN_ADV_H 1

#ifdef __linux

/*******************************************************************************
*   HEADER FILES                                                               *
*******************************************************************************/

#include <stdbool.h>

/*******************************************************************************
*   CONSTANT DEFINITIONS                                                       *
*******************************************************************************/

#define MAC_ADDR_CHAR_REPRESENTATION_LEN 17

/*******************************************************************************
*   TYPE DEFINITIONS                                                           *
*******************************************************************************/

/** Struct for a linked list of node MAC addresses.
 *  TODO: Representing MAC addresses as a 18 byte string is wasteful.
 */
struct mu_bat_mesh_node {
          char              mac_addr[MAC_ADDR_CHAR_REPRESENTATION_LEN + 1];
   struct mu_bat_mesh_node *next;
};

/*******************************************************************************
*   PUBLIC API FUNCTION DECLARATIONS                                           *
*******************************************************************************/

/**
 * @brief Test whether the batman_adv kernel module is available.
 *
 * The function only checks for the module for the currently running kernel.
 *
 * @param *error [out] For setting error codes on function failure.
 *
 * @retval  true  The module is available.
 * @retval  false The module is not available. Also returned if an error
 *                occurred!
 */
bool
mu_badv_kmod_available(int *const error)
__attribute__ ((visibility("default")));

/**
 * @brief Test whether the batman_adv kernel module is loaded.
 *
 * @param *error [out] For setting error codes on function failure.
 *
 * @retval  true  The module is loaded.
 * @retval  false The module is not loaded. Also returned if an error
 *                occurred!
 */
bool
mu_badv_kmod_loaded(int *const error)
__attribute__ ((visibility("default")));

/**
 * @brief Get the kernel module version.
 *
 * @param *error [out] For setting error codes on function failure.
 *
 * @return Pointer to kernel module version string. The pointer has to be
 *         free()'d by the caller.
 *
 * @retval NULL Returned on failure.
 */
char
*mu_badv_kmod_version(int *const error)
__attribute__ ((visibility("default")));

/**
 * @brief Test whether a bat interface is available.
 *
 * @param *interface_name [in]  Name of the bat interface.
 * @param *error          [out] For setting error codes on function failure.
 *
 * @retval true  The interface is available.
 * @retval false The interface is not available. Also returned if an error
 *               ocurred!
 */
bool
mu_badv_if_available(const char *const interface_name, int *const error)
__attribute__ ((visibility("default")));

/**
 * @brief Test whether a bat interface is up.
 *
 * If the interface is up it does not guarantee that the interface is connected
 * to a network.
 *
 * @param *interface_name [in]  Name of the bat interface.
 * @param *error          [out] For setting error codes on function failure.
 *
 * @retval true  The interface is up.
 * @retval false The interface is not up. Also returned if an error
 *               ocurred!
 */
bool
mu_badv_if_up(const char *const interface_name, int *const error)
__attribute__ ((visibility("default")));

/**
 * @brief Get the MAC address of the bat interface.
 *
 * This is the MAC address of the bat interface, not the interface(s) via which
 * the bat interace is/can be connected to a network.
 *
 * @param *interface_name [in]  Name of the bat interface.
 * @param *error          [out] For setting error codes on function failure.
 *
 * @return Pointer to interface MAC address string. The pointer has to be
 *         free()'d by the caller.
 *
 * @retval NULL Returned on failure.
 */
char
*mu_badv_if_hwaddr(const char *const interface_name, int *const error)
__attribute__ ((visibility("default")));

/**
 * @brief Get the number of nodes in the mesh.
 *
 * The function will return 1 if the interface is up, as it interprets
 * (intentionally) the unconnected interface as a mesh of one node (i.e. own
 * node). This means that a return value greater than 1 means that the own node
 * is connected to a mesh network (TODO: or self has multiple interfaces?).
 *
 * @param *interface_name [in]  Name of the bat interface.
 * @param *error          [out] For setting error codes on function failure.
 *
 * @return Number of nodes in the mesh. Count includes self.
 *
 * @retval 0 Zero returned on error.
 */
unsigned int
mu_badv_mesh_n_nodes(const char *const interface_name, int *const error)
__attribute__ ((visibility("default")));

/**
 * @brief Get the addresses of the nodes in the mesh.
 *
 * The function gets the MAC addresses of all nodes in the mesh. It returns a
 * linked list with each address represented once. (TODO: a node with more than
 * one interface might be multiple times in the list?). The function does not
 * guarantee that the node is still reachable via the mesh.
 *
 * @param *interface_name [in]  Name of the bat interface.
 * @param *n_nodes        [out] Pointer to an integer that will hold the number
 *                              of node addresses in the mesh. Does not include
 *                              self. Can be ignored by passing NULL.
 * @param *error          [out] For setting error codes on function failure.
 *
 * @return Pointer to a linked list of nodes in the mesh. The links in this
 *         list have to be free()'d by the caller.
 *
 * @retval NULL Returned on failure or when no nodes available.
 *
 * @see mu_badv_node_last_seen
 */
struct mu_bat_mesh_node
*mu_badv_mesh_node_addresses(const char *const interface_name,
                                   int  *const n_nodes,
                                   int  *const error)
__attribute__ ((visibility("default")));

/**
 * @brief Get the addresses of next hops in the mesh.
 *
 * If potential next hops are included (potential passed as true) it returns the
 * neighbours of the own node. This means all nodes that are potentially
 * reachable with only one hop. Depending on the connection infrastructure and,
 * in case of wireless connections, the transmissivity of the intervening this
 * could be interpreted as the physically closest nodest.
 *
 * @param *interface_name [in]  Name of the bat interface.
 * @param  potential      [in]  Whether to include potential next hops.
 * @param *n_nodes        [out] Pointer to an integer that will hold the number
 *                              of node addresses in the mesh. Does not include
 *                              self. Can be ignored by passing NULL.
 * @param *error          [out] For setting error codes on function failure.
 *
 * @return Pointer to a linked list of next hop addresses. The links in this
 *         list have to be free()'d by the caller.
 *
 * @retval NULL Returned on failure or when no nodes available.
 */
struct mu_bat_mesh_node
*mu_badv_next_hop_addresses(const char *const interface_name,
                            const bool        potential,
                                  int  *const n_nodes,
                                  int  *const error)
__attribute__ ((visibility("default")));

/**
 * @brief Tests whether a node is a next hop
 *
 * If potential next hops are included (potential passed as true) it can be used
 * to test if a mesh node is a neighbour, i.e. potentially one hop away. 
 *
 * @param *interface_name [in]  Name of the bat interface.
 * @param  potential      [in]  Whether to include potential next hops.
 * @param *node           [in]  The node that is being tested.
 * @param *error          [out] For setting error codes on function failure.
 *
 * @retval true  Node is a next hop.
 * @retval false Node is not a next hop. Also returned on error.
 *
 */
bool
mu_badv_node_is_next_hop(const        char             *const interface_name,
                         const struct mu_bat_mesh_node *const node,
                         const        bool                    potential,
                                      int              *const error)
__attribute__ ((visibility("default")));

/**
 * @brief Gets the interface via which the node is accessible.
 *
 * This referes to the (usually physical) interface attached to the bat
 * interface via which the node is visible.
 *
 * @param *interface_name [in]  Name of the bat interface.
 * @param *node           [in]  The node that is being tested.
 * @param *error          [out] For setting error codes on function failure.
 *
 * @return Pointer to string containing interface name.
 *
 * @retval NULL An error occurred.
 */
char
*mu_badv_node_accessible_via_if(
   const        char             *const interface_name,
   const struct mu_bat_mesh_node *const node,
                int              *const error)
__attribute__ ((visibility("default")));

/**
 * @brief Get the time since a node was last seen.
 *
 * A high value can mean that either the node is no longer accessible via the
 * mesh or that the connection is weak.
 *
 * @param *interface_name [in]  Name of the bat interface.
 * @param *node           [in]  The node that is being tested.
 * @param *error          [out] For setting error codes on function failure.
 *
 * @return The time since the node was last seen.
 *
 * @retval 0 An error occurred.
 */
double
mu_badv_node_last_seen(const        char             *const interface_name,
                       const struct mu_bat_mesh_node *const node,
                                    int              *const error)
__attribute__ ((visibility("default")));

/**
 * @brief Get the next hop node to reach a node.
 *
 * This is always an actual next hop, never a potential next hop.
 *
 * If self returns the passed pointer! So check always the return value
 * before calling free() on the returned pointer. Otherwise you might end up
 * calling free() twice on the same block of allocated memory.
 *
 * @param *interface_name [in]  Name of the bat interface.
 * @param *node           [in]  The node that is being tested.
 * @param *error          [out] For setting error codes on function failure.
 *
 * @return Pointer to the next hop node.
 *
 * @retval NULL An error occurred.
 */
struct mu_bat_mesh_node
*mu_badv_node_next_hop(const        char             *const interface_name,
                       const struct mu_bat_mesh_node *const node,
                                    int              *const error)
__attribute__ ((visibility("default")));

#endif                          /* __linux */
#endif                          /* MESHUTIL_BATMAN_ADV_H */
