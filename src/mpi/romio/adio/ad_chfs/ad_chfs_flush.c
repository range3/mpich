/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "ad_chfs_common.h"
#include "adioi.h"
#include "chfs.h"

void ADIOI_CHFS_Flush(ADIO_File fd, int* error_code) {
  static char myname[] = "ADIOI_CHFS_Flush";
  int myrank, nprocs;
  struct ADIOI_CHFS_fs_s* chfs_fs;
  struct stat st;
  MPI_Offset max_filesize;

  MPI_Comm_rank(fd->comm, &myrank);
#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_Flush called on %s\n", myrank, nprocs,
          fd->filename);
#endif

  *error_code = MPI_SUCCESS;
  if (chfs_fsync(fd->fd_sys)) {
    *error_code = ADIOI_Err_create_code(myname, fd->filename, errno);
    return;
  }
  fd->dirty_write = 0;

  chfs_fs = (ADIOI_CHFS_fs*)fd->fs_ptr;

  if (myrank == fd->hints->ranklist[0]) {
    if (chfs_stat(fd->filename, &st)) {
      *error_code = ADIOI_Err_create_code(myname, fd->filename, errno);
      return;
    }
    if (chfs_fs->filesize < st.st_size) {
      chfs_fs->filesize = st.st_size;
    }
  }

  MPI_Allreduce(&chfs_fs->filesize, &max_filesize, 1, MPI_LONG, MPI_MAX, fd->comm);
  chfs_fs->filesize = max_filesize;
}
