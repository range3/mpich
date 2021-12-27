/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_pmem.h"
#include "ad_pmem_common.h"
#include "adioi.h"
#include "mpi.h"
#include "pmembb/pmembb.h"
#include <fcntl.h>

static bool flush_pmembb_cb(const char *data, size_t size, off64_t ofs,
                            void *userdata, int *error_code) {
#ifdef DEBUG
  FPRINTF(stdout, "    !!!flushing (buf = %p, loc = %lld, sz = %lld)\n", data,
          (long long)ofs, (long long)size);
#endif
  ADIOI_GEN_WriteContig(userdata, data, size, MPI_BYTE, ADIO_EXPLICIT_OFFSET,
                        ofs, NULL, error_code);
  return *error_code == MPI_SUCCESS;
}

void ADIOI_PMEM_Flush(ADIO_File fd, int *error_code) {
#if defined(MPICH) || !defined(PRINT_ERR_MSG)
  static char myname[] = "ADIOI_PMEM_Flush";
#endif
  int myrank, nprocs;
  int flush_error_code;

  *error_code = MPI_SUCCESS;

#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_Flush called on %s\n", myrank, nprocs,
          fd->filename);
#endif

  ADIOI_PMEM_fs *pmemfs = fd->fs_ptr;
  flush_error_code = pmembb_bb_flush_all(pmemfs->bb, fd, flush_pmembb_cb);
  if (flush_error_code) {
    *error_code =
        ADIOI_Err_create_code(myname, fd->filename, -flush_error_code);
    return;
  }
}
