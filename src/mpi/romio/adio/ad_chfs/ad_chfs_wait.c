/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "adioi.h"

void ADIOI_CHFS_ReadComplete(ADIO_Request * request, ADIO_Status * status, int
                               *error_code)
{
    int myrank, nprocs;

    *error_code = MPI_SUCCESS;

#ifdef DEBUG
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_ReadComplete called \n", myrank, nprocs);
#endif

    /* do something with status set bytes? */
}

void ADIOI_CHFS_WriteComplete(ADIO_Request * request, ADIO_Status * status, int
                                *error_code)
{
    int myrank, nprocs;

    *error_code = MPI_SUCCESS;

#ifdef DEBUG
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_WriteComplete called\n", myrank, nprocs);
#endif

    /* do something with status_set_bytes? */
}
