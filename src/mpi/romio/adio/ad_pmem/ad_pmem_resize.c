/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_pmem.h"
#include "adioi.h"

void ADIOI_PMEM_Resize(ADIO_File fd, ADIO_Offset size, int *error_code)
{
    int myrank, nprocs;

    *error_code = MPI_SUCCESS;

    MPI_Comm_size(fd->comm, &nprocs);
    MPI_Comm_rank(fd->comm, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_Resize called on %s\n", myrank, nprocs, fd->filename);
}
