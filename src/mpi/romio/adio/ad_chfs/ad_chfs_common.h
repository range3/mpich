#ifndef AD_CHFS_COMMON_H_INCLUDED
#define AD_CHFS_COMMON_H_INCLUDED

#include <limits.h>
#include "adio.h"

struct ADIOI_CHFS_fs_s {
  char absolute_path[PATH_MAX];
};
typedef struct ADIOI_CHFS_fs_s ADIOI_CHFS_fs;

MPI_Offset ADIOI_CHFS_get_size_coll(ADIO_File fd, int* error_code);
char* ADIOI_CHFS_get_absolute_path(const char* relative_path,
                                   char* buf,
                                   size_t size);

#endif
