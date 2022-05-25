/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "adioi.h"

#include "mpiu_greq.h"
#include "../../mpi-io/mpioimpl.h"

/* ADIOI_CHFS_IwriteContig()
 *
 * Implemented by immediately calling WriteContig()
 */
void ADIOI_CHFS_IwriteContig(ADIO_File fd, const void *buf, int count,
                               MPI_Datatype datatype, int file_ptr_type,
                               ADIO_Offset offset, ADIO_Request * request, int
                               *error_code)
{
    ADIO_Status status;
    int myrank, nprocs;
    MPI_Count typesize, len;

    *error_code = MPI_SUCCESS;

    MPI_Type_size_x(datatype, &typesize);
#ifdef DEBUG
    MPI_Comm_size(fd->comm, &nprocs);
    MPI_Comm_rank(fd->comm, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_IwriteContig called on %s\n",
            myrank, nprocs, fd->filename);
    FPRINTF(stdout, "[%d/%d]    calling ADIOI_CHFS_WriteContig\n", myrank, nprocs);
#endif

    len = count * typesize;
    ADIOI_CHFS_WriteContig(fd, buf, len, MPI_BYTE, file_ptr_type, offset, &status, error_code);
    MPIO_Completed_request_create(&fd, len, error_code, request);

}

void ADIOI_CHFS_IwriteStrided(ADIO_File fd, const void *buf, int count,
                                MPI_Datatype datatype, int file_ptr_type,
                                ADIO_Offset offset, ADIO_Request * request, int
                                *error_code)
{
    ADIO_Status status;
    int myrank, nprocs;
    MPI_Count typesize;

    *error_code = MPI_SUCCESS;

    MPI_Type_size_x(datatype, &typesize);
#ifdef DEBUG
    MPI_Comm_size(fd->comm, &nprocs);
    MPI_Comm_rank(fd->comm, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_IwriteStrided called on %s\n",
            myrank, nprocs, fd->filename);
    FPRINTF(stdout, "[%d/%d]    calling ADIOI_CHFS_WriteStrided\n", myrank, nprocs);
#endif

    ADIOI_CHFS_WriteStrided(fd, buf, count, datatype, file_ptr_type, offset, &status, error_code);
    MPIO_Completed_request_create(&fd, count * typesize, error_code, request);

}
