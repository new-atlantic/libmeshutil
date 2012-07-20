#ifndef MESHUTIL_H
#define MESHUTIL_H 1

/// Most functions in libmeshutil accept an int *error function argument.
/// These functions accept also NULL, in which case the error is ignored.
/// MU_SET_ERROR is used to avoid assigning to the NULL pointer.
#define MU_SET_ERROR(eptr, eval) if ( eptr ) { *eptr = eval; }

#endif                          /* MESHUTIL_H */
