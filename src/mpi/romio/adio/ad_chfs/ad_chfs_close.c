/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "adioi.h"
#include "ad_chfs_common.h"

void ADIOI_CHFS_Close(ADIO_File fd, int* error_code) {
  static char myname[] = "ADIOI_CHFS_Close";
  struct ADIOI_CHFS_fs_s *chfs_fs;

#ifdef DEBUG
  int myrank, nprocs;
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_Close called on %s\n", myrank, nprocs,
          fd->filename);
#endif

  *error_code = MPI_SUCCESS;
  if (chfs_close(fd->fd_sys)) {
    *error_code = ADIOI_Err_create_code(myname, fd->filename, errno);
    return;
  }
  chfs_fs = (ADIOI_CHFS_fs*) fd->fs_ptr;
  ADIOI_Free(chfs_fs);
  fd->fs_ptr = NULL;
  fd->fd_sys = -1;
}
