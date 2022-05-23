/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "adioi.h"

void ADIOI_CHFS_Open(ADIO_File fd, int* error_code) {
  static char myname[] = "ADIOI_CHFS_Open";

#ifdef DEBUG
  int myrank, nprocs;
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_Open called on %s\n", myrank, nprocs,
          fd->filename);
#endif

  *error_code = MPI_SUCCESS;

  ADIOI_CHFS_Init(error_code);
  if (*error_code != MPI_SUCCESS) {
    return;
  }

  int32_t flags;
  if (fd->access_mode & ADIO_RDONLY) {
    flags |= O_RDONLY;
  }
  if (fd->access_mode & ADIO_WRONLY) {
    flags |= O_WRONLY;
  }
  if (fd->access_mode & ADIO_RDWR) {
    flags |= O_RDWR;
  }
  if (fd->access_mode & ADIO_APPEND) {
    flags |= O_APPEND;
  }
  if (fd->access_mode & ADIO_EXCL) {
    flags |= O_EXCL;
  }

  mode_t perm, mask;
  if (fd->perm == ADIO_PERM_NULL) {
    mask = umask(S_IWGRP | S_IWOTH);
    umask(mask);
    perm = 0666 ^ mask;
  } else {
    perm = fd->perm;
  }
  perm = S_IFREG | perm;

  // ADIOI_CHFS_fs* chfs_fs = NULL;
  // chfs_fs = (ADIOI_CHFS_fs*)ADIOI_Malloc(sizeof(ADIOI_CHFS_fs));
  // if (chfs_fs == NULL) {
  //   *error_code =
  //       MPIO_Err_create_code(*error_code, MPIR_ERR_RECOVERABLE, myname,
  //                            __LINE__, MPI_ERR_OTHER, "**nomem2", 0);
  //   goto on_abort;
  // }
  // memset(chfs_fs, 0, sizeof(ADIOI_CHFS_fs));

  if (fd->access_mode & ADIO_CREATE) {
    fd->fd_sys = chfs_create(fd->filename, flags, perm);
  } else {
    fd->fd_sys = chfs_open(fd->filename, flags);
  }

  if (fd->fd_sys < 0) {
    *error_code = ADIOI_Err_create_code(myname, fd->filename, errno);
    goto on_abort;
  }

  // fd->fs_ptr = chfs_fs;
  return;

on_abort:
  // if (chfs_fs) {
  //   ADIOI_Free(chfs_fs);
  // }
  ADIOI_CHFS_Term(error_code);
}
