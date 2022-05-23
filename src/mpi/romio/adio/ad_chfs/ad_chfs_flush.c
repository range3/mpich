/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "adioi.h"

void ADIOI_CHFS_Flush(ADIO_File fd, int* error_code) {
  static char myname[] = "ADIOI_CHFS_Flush";
#ifdef DEBUG
  int myrank, nprocs;
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_Flush called on %s\n", myrank, nprocs,
          fd->filename);
#endif

  *error_code = MPI_SUCCESS;
  if (chfs_fsync(fd->fd_sys)) {
    *error_code = ADIOI_Err_create_code(myname, fd->filename, errno);
  }
}
