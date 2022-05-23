/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
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
  MPI_Count datatype_size;
  MPI_Type_size_x(datatype, &datatype_size);

#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_ReadContig called on %s\n", myrank,
          nprocs, fd->filename);
#endif

  *error_code = MPI_SUCCESS;

  if (file_ptr_type != ADIO_EXPLICIT_OFFSET) {
    offset = fd->fp_ind;
    fd->fp_ind += datatype_size * count;
    fd->fp_sys_posn = fd->fp_ind;
  } else {
    fd->fp_sys_posn = offset + datatype_size * count;
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
      return;
    }
    xfered += ss;
  }

#ifdef HAVE_STATUS_SET_BYTES
  MPIR_Status_set_bytes(status, datatype, datatype_size * count);
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
