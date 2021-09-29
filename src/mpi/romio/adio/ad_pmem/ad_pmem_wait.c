/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_pmem.h"
#include "adioi.h"

void ADIOI_PMEM_ReadComplete(ADIO_Request * request, ADIO_Status * status, int
                               *error_code)
{
    int myrank, nprocs;

    *error_code = MPI_SUCCESS;

    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_ReadComplete called \n", myrank, nprocs);

    /* do something with status set bytes? */
}

void ADIOI_PMEM_WriteComplete(ADIO_Request * request, ADIO_Status * status, int
                                *error_code)
{
    int myrank, nprocs;

    *error_code = MPI_SUCCESS;

    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_WriteComplete called\n", myrank, nprocs);

    /* do something with status_set_bytes? */
}
