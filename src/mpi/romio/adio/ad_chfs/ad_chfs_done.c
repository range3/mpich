/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "adioi.h"

int ADIOI_CHFS_ReadDone(ADIO_Request * request, ADIO_Status * status, int
                          *error_code)
{
#ifdef DEBUG
    int myrank, nprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_ReadDone called on ADIO_REQUEST_NULL\n", myrank, nprocs);
#endif

    *error_code = MPI_SUCCESS;
    return 1;
}


int ADIOI_CHFS_WriteDone(ADIO_Request * request, ADIO_Status * status, int
                           *error_code)
{
#ifdef DEBUG
    int myrank, nprocs;
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_WriteDone called on ADIO_REQUEST_NULL\n", myrank, nprocs);
#endif

    *error_code = MPI_SUCCESS;
    return 1;
}
