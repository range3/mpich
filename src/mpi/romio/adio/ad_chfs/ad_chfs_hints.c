/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "adioi.h"
#ifdef ROMIO_BGL
#include "../ad_bgl/ad_bgl.h"
#endif
void ADIOI_CHFS_SetInfo(ADIO_File fd, MPI_Info users_info, int *error_code)
{
#ifdef DEBUG
    int myrank, nprocs;
    MPI_Comm_size(fd->comm, &nprocs);
    MPI_Comm_rank(fd->comm, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_SetInfo called on %s\n", myrank, nprocs, fd->filename);
    FPRINTF(stdout, "[%d/%d]    calling ADIOI_GEN_SetInfo\n", myrank, nprocs);
#endif

    *error_code = MPI_SUCCESS;
    ADIOI_GEN_SetInfo(fd, users_info, error_code);
}
