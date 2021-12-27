/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_pmem.h"
#include "adio.h"
#include "adio/ad_pmem/ad_pmem_common.h"
#include "adioi.h"
#include "pmembb/pmembb.h"

void ADIOI_PMEM_WriteContig(ADIO_File fd, const void *buf, int count,
                            MPI_Datatype datatype, int file_ptr_type,
                            ADIO_Offset offset, ADIO_Status *status,
                            int *error_code) {
#if defined(MPICH) || !defined(PRINT_ERR_MSG)
  static char myname[] = "ADIOI_PMEM_WriteContig";
#endif
#ifdef DEBUG
  int myrank, nprocs;
#endif
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
  FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_WriteContig called on %s\n", myrank,
          nprocs, fd->filename);
  FPRINTF(stdout, "[%d/%d]    writing (buf = %p, loc = %lld, sz = %lld)\n",
          myrank, nprocs, buf, (long long)offset, (long long)len);
#endif

  ADIOI_PMEM_fs *pmemfs = fd->fs_ptr;
  if (pmembb_bb_pwrite(pmemfs->bb, buf, len, offset) < 0) {
    *error_code = ADIOI_Err_create_code(myname, fd->filename, errno);
    fd->fp_sys_posn = -1;
    return;
  }

  fd->fp_sys_posn = offset + len;
  if (file_ptr_type == ADIO_INDIVIDUAL) {
    fd->fp_ind += len;
  }

#ifdef HAVE_STATUS_SET_BYTES
  MPIR_Status_set_bytes(status, datatype, len);
#endif
}

void ADIOI_PMEM_WriteStrided(ADIO_File fd, const void *buf, int count,
                             MPI_Datatype datatype, int file_ptr_type,
                             ADIO_Offset offset, ADIO_Status *status,
                             int *error_code) {
  *error_code = MPI_SUCCESS;
#ifdef DEBUG
  int myrank, nprocs;
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_WriteStrided called on %s\n", myrank,
          nprocs, fd->filename);
  FPRINTF(stdout, "[%d/%d]    calling ADIOI_GEN_WriteStrided\n", myrank,
          nprocs);
#endif

  ADIOI_GEN_WriteStrided(fd, buf, count, datatype, file_ptr_type, offset,
                         status, error_code);
}
