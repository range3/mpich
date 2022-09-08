/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "ad_chfs_common.h"
#include "adio_extern.h"
#include "adioi.h"

void ADIOI_CHFS_Fcntl(ADIO_File fd,
                      int flag,
                      ADIO_Fcntl_t* fcntl_struct,
                      int* error_code) {
  int myrank, nprocs;
  static char myname[] = "ADIOI_CHFS_Fcntl";
  struct ADIOI_CHFS_fs_s* chfs_fs;

#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_Fcntl called on %s\n", myrank, nprocs,
          fd->filename);
#endif

  *error_code = MPI_SUCCESS;

  switch (flag) {
    case ADIO_FCNTL_GET_FSIZE:
      chfs_fs = (ADIOI_CHFS_fs*)fd->fs_ptr;
      fcntl_struct->fsize = 0;
      struct stat st;
      if (chfs_stat(chfs_fs->absolute_path, &st)) {
        *error_code = ADIOI_Err_create_code(myname, fd->filename, errno);
      } else {
        fcntl_struct->fsize = st.st_size;
      }
      break;

    case ADIO_FCNTL_SET_DISKSPACE:
    case ADIO_FCNTL_SET_ATOMICITY:
    default:
      /* --BEGIN ERROR HANDLING-- */
      *error_code = MPIO_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE,
                                         myname, __LINE__, MPI_ERR_ARG,
                                         "**flag", "**flag %d", flag);
      return;
      /* --END ERROR HANDLING-- */
  }
}
