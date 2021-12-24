/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_pmem.h"
#include "adioi.h"
#include "pmembb/pmembb.h"

void ADIOI_PMEM_Close(ADIO_File fd, int *error_code) {
  int myrank, nprocs;
  PMemBBPool* pop;

#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_Close called on %s\n", myrank, nprocs,
          fd->filename);
#endif

  pop = fd->fs_ptr;

  if (pop) {
    pmembb_pool_close(pop);
    fd->fs_ptr = NULL;
  }

  ADIOI_GEN_Close(fd, error_code);
}
