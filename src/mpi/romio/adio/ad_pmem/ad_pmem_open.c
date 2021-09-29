/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_pmem.h"
#include "adioi.h"

void ADIOI_PMEM_Open(ADIO_File fd, int *error_code)
{
    int myrank, nprocs;

    fd->fd_sys = 1;
    fd->fd_direct = -1;
    *error_code = MPI_SUCCESS;

    MPI_Comm_size(fd->comm, &nprocs);
    MPI_Comm_rank(fd->comm, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_Open called on %s\n", myrank, nprocs, fd->filename);
}
