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

#endif                          /* __linux */
#endif                          /* MESHUTIL_BATMAN_ADV_H */
