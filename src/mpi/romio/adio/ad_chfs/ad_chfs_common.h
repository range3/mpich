#ifndef AD_CHFS_COMMON_H_INCLUDED
#define AD_CHFS_COMMON_H_INCLUDED

#include "adio.h"

struct ADIOI_CHFS_fs_s {
  MPI_Offset filesize;
};
typedef struct ADIOI_CHFS_fs_s ADIOI_CHFS_fs;


MPI_Offset ADIOI_CHFS_get_size_coll(ADIO_File fd, int* error_code);

#endif
