/// @file batman_adv.h

/* Abbreviations used:
 *
 *     batman_adv:  B.A.T.M.A.N. advanced
 *     hwaddr:      MAC address of network interface
 *     if:          network interface
 *     kmod:        kernel module
 *     mac_addr:    MAC address
 *     mu:          meshutil
 */

#ifndef MESHUTIL_BATMAN_ADV_H
#define MESHUTIL_BATMAN_ADV_H 1

#ifdef __linux

#include <stdbool.h>

/// Struct for a linked list of node MAC addresses
struct mu_bat_mesh_node {
     char mac_addr[18];
     struct mu_bat_mesh_node *next;
};

/**
 * @brief Test whether the batman_adv kernel module is available on the
 *        current kernel.
 *
 * @param  *error See documetation for *error.
 *
 * @retval  true  The module is available.
 * @retval  false The module is not available. Also returned if an error
 *                occurred.
 **/
bool mu_batman_adv_kmod_available(int *error);

/**
 * @brief Test whether the batman_adv kernel module is loaded.
 *
 * @param  *error See documetation for *error.
 *
 * @retval  true  The module is loaded.
 * @retval  false The module is not loaded. Also returned if an error
 *                occurred.
 **/
bool mu_batman_adv_kmod_loaded(int *error);

/**
 * @brief Get the kernel module version.
 *
 * @param  *error See documetation for *error.
 *
 * @return Pointer to kernel module version string. The pointer has to be
 * free()'d by the caller.
 * @retval NULL Returned on failure.
 */
char *mu_batman_adv_kmod_version(int *error);

/**
 * @brief Check whether a bat interface is available.
 *
 * @param *error See documetation for *error.
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @retval true  The interface is available.
 * @retval false The interface is not available. Also returned if an error
 *               ocurred.
 */
bool mu_batman_adv_if_available(char *interface_name, int *error);

/**
 * @brief Check whether a bat interface is up.
 *
 * @param *error See documetation for *error.
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @retval true  The interface is up.
 * @retval false The interface is not up. Also returned if an error
 *               ocurred.

 */
bool mu_batman_adv_if_up(char *interface_name, int *error);

/**
 * @brief Get the MAC address of the bat interface.
 *
 * @param *error See documetation for *error.
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @return Pointer to interface MAC address string. The pointer has to be
 * free()'d by the caller.
 * @retval NULL Returned on failure.
 */
char *mu_batman_adv_if_hwaddr(char *interface_name, int *error);

/**
 * @brief Get the number of nodes in the mesh.
 *
 * @param *error See documetation for *error.
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @return Number of nodes in the mesh. Count includes self.
 * @retval 0 Zero returned on error.
 */
unsigned int mu_batman_adv_mesh_n_nodes(char *interface_name, int *error);

/**
 * @brief Get the addresses of the nodes in the mesh.
 *
 * @param *error See documetation for *error.
 * @param *n_nodes Pointer to an integer that will hold the number of nodes
 *                 addresses the mesh. Does not include self.
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @return Pointer to a linked list of nodes in the mesh. The links in this
 *         list have to be free()'d by the caller.
 * @retval NULL Returned on failure.
 */
struct mu_bat_mesh_node *mu_batman_adv_mesh_node_addresses(
                                                           char *interface_name,
                                                           int  *n_nodes,
                                                           int  *error);

/**
 * @brief Get the addresses of neighbouring nodes (next hops) in the mesh.
 *
 * @param *error See documetation for *error.
 * @param *n_nodes Pointer to an integer that will hold the number of next hop
 *                 addresses the mesh.
 * @param  potential If true, includes potential next hops (i.e. routes not
 *                   used, but available).
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @return Pointer to a linked list of next hop addresses. The links in this
 *         list have to be free()'d by the caller.
 * @retval NULL Returned on failure.
 */
struct mu_bat_mesh_node *mu_batman_adv_next_hop_addresses(
                                                          char *interface_name,
                                                          bool  potential,
                                                          int  *n_nodes,
                                                          int  *error);

/**
 * @brief Checks whether a node is a next hop
 *
 * @param *error See documetation for *error.
 * @param  potential If true, includes potential next hops (i.e. routes not
 *                   used, but available).
 * @param *interface_name Name of the interface to be looked for. If NULL is
 *                        passed looks for "bat0", the default interface.
 *
 * @retval true Node is a next hop.
 * @retval false Node is not a next hop. Also returned on error.
 *
 */
bool mu_batman_adv_node_is_next_hop(struct mu_bat_mesh_node *node,
                                    bool potential,
                                    int  *error);

#endif                          /* __linux */
#endif                          /* MESHUTIL_BATMAN_ADV_H */
