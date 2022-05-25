/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "adioi.h"

void ADIOI_CHFS_Get_shared_fp(ADIO_File fd, int size, ADIO_Offset * shared_fp, int *error_code)
{
#ifdef DEBUG
    int myrank, nprocs;
    MPI_Comm_size(fd->comm, &nprocs);
    MPI_Comm_rank(fd->comm, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_Get_shared_fp called on %s\n",
            myrank, nprocs, fd->filename);
#endif

    *error_code = MPI_SUCCESS;
}
