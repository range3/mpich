/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_pmem.h"
#include "ad_pmem_common.h"
#include "adioi.h"

void ADIOI_PMEM_Close(ADIO_File fd, int *error_code) {
  int myrank, nprocs;
  struct ADIOI_PMEM_fs_s *pmem_fs;

#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_Close called on %s\n", myrank, nprocs,
          fd->filename);
#endif

  pmem_fs = fd->fs_ptr;

  if (pmem_fs) {
    if (pmem_fs->bb) {
      pmembb_bb_close(pmem_fs->bb);
    }
    if (pmem_fs->file) {
      pmembb_file_close(pmem_fs->file);
    }
    if (pmem_fs->pool) {
      pmembb_pool_close(pmem_fs->pool);
    }
    ADIOI_Free(pmem_fs);
    fd->fs_ptr = NULL;
  }

  ADIOI_GEN_Close(fd, error_code);
}
