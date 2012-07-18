
#ifndef MESHUTIL_BATMAN_ADV_H
#define MESHUTIL_BATMAN_ADV_H 1

#ifdef __linux

#include <stdbool.h>

bool mu_ba_kmod_available(int *error);

#endif /* __linux */
#endif /* MESHUTIL_BATMAN_ADV_H */

