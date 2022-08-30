/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "ad_chfs_common.h"
#include "adioi.h"

void ADIOI_CHFS_ReadContig(ADIO_File fd,
                           void* buf,
                           int count,
                           MPI_Datatype datatype,
                           int file_ptr_type,
                           ADIO_Offset offset,
                           ADIO_Status* status,
                           int* error_code) {
  static char myname[] = "ADIOI_CHFS_ReadContig";
  int myrank, nprocs;
  struct ADIOI_CHFS_fs_s* chfs_fs = (ADIOI_CHFS_fs*)fd->fs_ptr;;
  MPI_Count datatype_size;
  MPI_Type_size_x(datatype, &datatype_size);

#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_ReadContig called on %s\n", myrank,
          nprocs, fd->filename);
#endif

  *error_code = MPI_SUCCESS;

  if (file_ptr_type == ADIO_INDIVIDUAL) {
    offset = fd->fp_ind;
  }

#ifdef DEBUG
  FPRINTF(stdout, "[%d/%d]    reading (buf = %p, loc = %lld, sz = %lld)\n",
          myrank, nprocs, buf, (long long)offset,
          (long long)datatype_size * count);
#endif

  size_t data_size = datatype_size * count;
  size_t xfered = 0;
  ssize_t ss;
  while (xfered < data_size) {
    ss = chfs_pread(fd->fd_sys, buf + xfered, data_size - xfered,
                    offset + xfered);
    if (ss < 0) {
      *error_code = ADIOI_Err_create_code(myname, fd->filename, errno);
      fd->fp_sys_posn = -1;
      return;
    }

    if (ss == 0) {
      break;
    }
    xfered += ss;
  }

  fd->fp_sys_posn = offset + xfered;
  if (file_ptr_type == ADIO_INDIVIDUAL) {
    fd->fp_ind += xfered;
  }

#ifdef HAVE_STATUS_SET_BYTES
  if (status && ss >= 0) {
    MPIR_Status_set_bytes(status, datatype, xfered);
  }
#endif
}

void ADIOI_CHFS_ReadStrided(ADIO_File fd,
                            void* buf,
                            int count,
                            MPI_Datatype datatype,
                            int file_ptr_type,
                            ADIO_Offset offset,
                            ADIO_Status* status,
                            int* error_code) {
#ifdef DEBUG
  int myrank, nprocs;
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_ReadStrided called on %s\n", myrank,
          nprocs, fd->filename);
  FPRINTF(stdout, "[%d/%d]    calling ADIOI_GEN_ReadStrided\n", myrank, nprocs);
#endif

  *error_code = MPI_SUCCESS;

  ADIOI_GEN_ReadStrided(fd, buf, count, datatype, file_ptr_type, offset, status,
                        error_code);
}
