/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_pmem.h"
#include "adioi.h"

void ADIOI_PMEM_ReadContig(ADIO_File fd, void *buf, int count,
                           MPI_Datatype datatype, int file_ptr_type,
                           ADIO_Offset offset, ADIO_Status *status,
                           int *error_code) {
  int myrank, nprocs;
  MPI_Count datatype_size, len;

  *error_code = MPI_SUCCESS;

  MPI_Type_size_x(datatype, &datatype_size);
  len = datatype_size * (ADIO_Offset)count;
  if (file_ptr_type == ADIO_INDIVIDUAL) {
    offset = fd->fp_ind;
  }

#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_ReadContig called on %s\n", myrank,
          nprocs, fd->filename);
  FPRINTF(stdout, "[%d/%d]    reading (buf = %p, loc = %lld, sz = %lld)\n",
          myrank, nprocs, buf, (long long)offset, (long long)len);
#endif

  ADIOI_PMEM_Flush(fd, error_code);
  if (*error_code != MPI_SUCCESS) {
    return;
  }

  ADIOI_GEN_ReadContig(fd, buf, count, datatype, file_ptr_type, offset, status,
                       error_code);
  if (*error_code != MPI_SUCCESS) {
    return;
  }

  fd->fp_sys_posn = offset + len;
  if (file_ptr_type == ADIO_INDIVIDUAL) {
    fd->fp_ind += len;
  }

#ifdef HAVE_STATUS_SET_BYTES
  MPIR_Status_set_bytes(status, datatype, datatype_size * count);
#endif
}

void ADIOI_PMEM_ReadStrided(ADIO_File fd, void *buf, int count,
                            MPI_Datatype datatype, int file_ptr_type,
                            ADIO_Offset offset, ADIO_Status *status,
                            int *error_code) {
  int myrank, nprocs;

  *error_code = MPI_SUCCESS;

  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_ReadStrided called on %s\n", myrank,
          nprocs, fd->filename);
  FPRINTF(stdout, "[%d/%d]    calling ADIOI_GEN_ReadStrided\n", myrank, nprocs);

  ADIOI_GEN_ReadStrided(fd, buf, count, datatype, file_ptr_type, offset, status,
                        error_code);
}
