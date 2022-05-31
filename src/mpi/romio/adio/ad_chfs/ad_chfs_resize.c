/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "adioi.h"

void ADIOI_CHFS_Resize(ADIO_File fd, ADIO_Offset size, int* error_code) {
  static char myname[] = "ADIOI_CHFS_Resize";
  int ret;
  int nprocs, myrank;

  MPI_Comm_rank(fd->comm, &myrank);
#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_Resize called on %s, sz=%d\n", myrank,
          nprocs, fd->filename, size);
#endif

  *error_code = MPI_SUCCESS;
  MPI_Barrier(fd->comm);

  if (myrank == 0) {
    ret = chfs_truncate(fd->filename, size);
  }

  MPI_Bcast(&ret, 1, MPI_INT, 0, fd->comm);

  if (ret != 0) {
    *error_code = ADIOI_Err_create_code(myname, fd->filename, errno);
  }
}
