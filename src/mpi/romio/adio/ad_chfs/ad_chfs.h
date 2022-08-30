/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#ifndef AD_CHFS_H_INCLUDED
#define AD_CHFS_H_INCLUDED

#include "adio.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>

#include <chfs.h>

void ADIOI_CHFS_Init(int *error_code);
void ADIOI_CHFS_Term(int *error_code);
void ADIOI_CHFS_Open(ADIO_File fd, int *error_code);
void ADIOI_CHFS_Close(ADIO_File fd, int *error_code);
void ADIOI_CHFS_ReadContig(ADIO_File fd, void *buf, int count,
                             MPI_Datatype datatype, int file_ptr_type,
                             ADIO_Offset offset, ADIO_Status * status, int
                             *error_code);
void ADIOI_CHFS_WriteContig(ADIO_File fd, const void *buf, int count,
                              MPI_Datatype datatype, int file_ptr_type,
                              ADIO_Offset offset, ADIO_Status * status, int
                              *error_code);
void ADIOI_CHFS_IwriteContig(ADIO_File fd, const void *buf, int count,
                               MPI_Datatype datatype, int file_ptr_type,
                               ADIO_Offset offset, ADIO_Request * request, int
                               *error_code);
void ADIOI_CHFS_IreadContig(ADIO_File fd, void *buf, int count,
                              MPI_Datatype datatype, int file_ptr_type,
                              ADIO_Offset offset, ADIO_Request * request, int
                              *error_code);
int ADIOI_CHFS_ReadDone(ADIO_Request * request, ADIO_Status * status, int
                          *error_code);
int ADIOI_CHFS_WriteDone(ADIO_Request * request, ADIO_Status * status, int
                           *error_code);
void ADIOI_CHFS_ReadComplete(ADIO_Request * request, ADIO_Status * status, int
                               *error_code);
void ADIOI_CHFS_WriteComplete(ADIO_Request * request, ADIO_Status * status, int *error_code);
void ADIOI_CHFS_Fcntl(ADIO_File fd, int flag, ADIO_Fcntl_t * fcntl_struct, int *error_code);
void ADIOI_CHFS_WriteStrided(ADIO_File fd, const void *buf, int count,
                               MPI_Datatype datatype, int file_ptr_type,
                               ADIO_Offset offset, ADIO_Status * status, int *error_code);
void ADIOI_CHFS_ReadStrided(ADIO_File fd, void *buf, int count,
                              MPI_Datatype datatype, int file_ptr_type,
                              ADIO_Offset offset, ADIO_Status * status, int
                              *error_code);
void ADIOI_CHFS_WriteStridedColl(ADIO_File fd, const void *buf, int count,
                                   MPI_Datatype datatype, int file_ptr_type,
                                   ADIO_Offset offset, ADIO_Status * status, int
                                   *error_code);
void ADIOI_CHFS_ReadStridedColl(ADIO_File fd, void *buf, int count,
                                  MPI_Datatype datatype, int file_ptr_type,
                                  ADIO_Offset offset, ADIO_Status * status, int
                                  *error_code);
void ADIOI_CHFS_IreadStrided(ADIO_File fd, void *buf, int count,
                               MPI_Datatype datatype, int file_ptr_type,
                               ADIO_Offset offset, ADIO_Request * request, int
                               *error_code);
void ADIOI_CHFS_IwriteStrided(ADIO_File fd, const void *buf, int count,
                                MPI_Datatype datatype, int file_ptr_type,
                                ADIO_Offset offset, ADIO_Request * request, int
                                *error_code);
void ADIOI_CHFS_Flush(ADIO_File fd, int *error_code);
void ADIOI_CHFS_Resize(ADIO_File fd, ADIO_Offset size, int *error_code);
ADIO_Offset ADIOI_CHFS_SeekIndividual(ADIO_File fd, ADIO_Offset offset,
                                        int whence, int *error_code);
void ADIOI_CHFS_SetInfo(ADIO_File fd, MPI_Info users_info, int *error_code);
void ADIOI_CHFS_Get_shared_fp(ADIO_File fd, int size, ADIO_Offset * shared_fp, int *error_code);
void ADIOI_CHFS_Set_shared_fp(ADIO_File fd, ADIO_Offset offset, int *error_code);
void ADIOI_CHFS_Delete(const char *filename, int *error_code);

#endif /* AD_CHFS_H_INCLUDED */
