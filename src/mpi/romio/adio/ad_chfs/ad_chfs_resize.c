/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "adioi.h"

void ADIOI_CHFS_Resize(ADIO_File fd, ADIO_Offset size, int* error_code) {
  static char myname[] = "ADIOI_CHFS_Resize";
#ifdef DEBUG
  int myrank, nprocs;
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_Resize called on %s, sz=%d\n", myrank,
          nprocs, fd->filename, size);
#endif

  *error_code = MPI_SUCCESS;

  if (chfs_truncate(fd->filename, size)) {
    *error_code = ADIOI_Err_create_code(myname, fd->filename, errno);
  }
}
