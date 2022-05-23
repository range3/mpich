/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "adioi.h"

void ADIOI_CHFS_WriteContig(ADIO_File fd,
                            const void* buf,
                            int count,
                            MPI_Datatype datatype,
                            int file_ptr_type,
                            ADIO_Offset offset,
                            ADIO_Status* status,
                            int* error_code) {
  static char myname[] = "ADIOI_CHFS_WriteContig";
  int myrank, nprocs;
  MPI_Count datatype_size;
  MPI_Type_size_x(datatype, &datatype_size);

#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_WriteContig called on %s\n", myrank,
          nprocs, fd->filename);
  FPRINTF(stdout, "[%d/%d]    writing (buf = %p, loc = %lld, sz = %lld)\n",
          myrank, nprocs, buf, (long long)offset,
          (long long)datatype_size * (long long)count);
#endif
  *error_code = MPI_SUCCESS;

  off_t write_offset;
  if (file_ptr_type != ADIO_EXPLICIT_OFFSET) {
    write_offset = fd->fp_ind;
  } else {
    write_offset = offset;
  }
  size_t data_size = datatype_size * count;

  size_t xfered = 0;
  ssize_t ss;
  while (xfered < data_size) {
    ss = chfs_pwrite(fd->fd_sys, buf + xfered, data_size - xfered,
                     write_offset + xfered);
    if (ss < 0) {
      *error_code = ADIOI_Err_create_code(myname, fd->filename, errno);
      return;
    }

    xfered += ss;
  }

  if (file_ptr_type != ADIO_EXPLICIT_OFFSET) {
    fd->fp_ind += data_size;
    fd->fp_sys_posn = fd->fp_ind;
#ifdef DEBUG
    FPRINTF(stdout, "[%d/%d]    new file position is %lld\n", myrank, nprocs,
            (long long)fd->fp_ind);
#endif
  } else {
    fd->fp_sys_posn = write_offset + data_size;
  }

#ifdef HAVE_STATUS_SET_BYTES
  MPIR_Status_set_bytes(status, datatype, datatype_size * count);
#endif
}

void ADIOI_CHFS_WriteStrided(ADIO_File fd,
                             const void* buf,
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
  FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_WriteStrided called on %s\n", myrank,
          nprocs, fd->filename);
  FPRINTF(stdout, "[%d/%d]    calling ADIOI_GEN_WriteStrided\n", myrank,
          nprocs);
#endif

  *error_code = MPI_SUCCESS;
  ADIOI_GEN_WriteStrided(fd, buf, count, datatype, file_ptr_type, offset,
                         status, error_code);
}
