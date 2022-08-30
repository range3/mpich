/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "ad_chfs_common.h"
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
  struct ADIOI_CHFS_fs_s* chfs_fs = (ADIOI_CHFS_fs*)fd->fs_ptr;;
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

  off_t write_offset = offset;
  if (file_ptr_type == ADIO_INDIVIDUAL) {
    write_offset = fd->fp_ind;
  }
  size_t data_size = datatype_size * count;
  size_t xfered = 0;
  ssize_t ss;
  while (xfered < data_size) {
    ss = chfs_pwrite(fd->fd_sys, buf + xfered, data_size - xfered,
                     write_offset + xfered);
#ifdef DEBUG
    FPRINTF(stdout, "[%d/%d]    chfs_pwrite xfered=%d,sz=%d,ofs=%d,ss=%d\n",
            myrank, nprocs, xfered, data_size - xfered, write_offset + xfered,
            ss);
#endif
    if (ss < 0) {
      *error_code = ADIOI_Err_create_code(myname, fd->filename, errno);
      fd->fp_sys_posn = -1;
      return;
    }

    xfered += ss;
  }

  fd->fp_sys_posn = write_offset + xfered;
  if (file_ptr_type == ADIO_INDIVIDUAL) {
    fd->fp_ind += xfered;
#ifdef DEBUG
    FPRINTF(stdout, "[%d/%d]    new file position is %lld\n", myrank, nprocs,
            (long long)fd->fp_ind);
#endif
  }

  if (fd->fp_sys_posn > chfs_fs->filesize) {
    chfs_fs->filesize = fd->fp_sys_posn;
  }

#ifdef HAVE_STATUS_SET_BYTES
  if (status && ss >= 0) {
    MPIR_Status_set_bytes(status, datatype, datatype_size * count);
  }
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
  MPI_Count datatype_size;
  MPI_Type_size_x(datatype, &datatype_size);
  FPRINTF(stdout, "[%d/%d]    writing (buf = %p, loc = %lld, sz = %lld)\n",
          myrank, nprocs, buf, (long long)offset,
          (long long)datatype_size * (long long)count);
  FPRINTF(stdout, "[%d/%d]    calling ADIOI_GEN_WriteStrided\n", myrank,
          nprocs);
#endif

  *error_code = MPI_SUCCESS;
  ADIOI_GEN_WriteStrided(fd, buf, count, datatype, file_ptr_type, offset,
                         status, error_code);
}
