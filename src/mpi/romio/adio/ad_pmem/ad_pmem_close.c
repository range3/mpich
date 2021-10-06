/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_pmem.h"
#include "adioi.h"

void ADIOI_PMEM_Close(ADIO_File fd, int *error_code) {
  int myrank, nprocs;

  fd->fd_sys = -1;
  *error_code = MPI_SUCCESS;

  if (fd->hints && fd->hints->pmem.pool_list)
    ADIOI_Free(fd->hints->pmem.pool_list);

#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_Close called on %s\n", myrank, nprocs,
          fd->filename);
#endif
}
