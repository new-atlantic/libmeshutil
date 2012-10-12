/// @file batman_adv.h

#ifndef MESHUTIL_BATMAN_ADV_H
#define MESHUTIL_BATMAN_ADV_H 1

#ifdef __linux

#include <stdbool.h>

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
 * @return Pointer to kernel module version string.
 * @return Returns NULL on failure.
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

#endif                          /* __linux */
#endif                          /* MESHUTIL_BATMAN_ADV_H */
