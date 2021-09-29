/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#ifndef AD_PMEM_H_INCLUDED
#define AD_PMEM_H_INCLUDED

#include "adio.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

void ADIOI_PMEM_Open(ADIO_File fd, int *error_code);
void ADIOI_PMEM_Close(ADIO_File fd, int *error_code);
void ADIOI_PMEM_ReadContig(ADIO_File fd, void *buf, int count,
                           MPI_Datatype datatype, int file_ptr_type,
                           ADIO_Offset offset, ADIO_Status *status,
                           int *error_code);
void ADIOI_PMEM_WriteContig(ADIO_File fd, const void *buf, int count,
                            MPI_Datatype datatype, int file_ptr_type,
                            ADIO_Offset offset, ADIO_Status *status,
                            int *error_code);
void ADIOI_PMEM_IwriteContig(ADIO_File fd, const void *buf, int count,
                             MPI_Datatype datatype, int file_ptr_type,
                             ADIO_Offset offset, ADIO_Request *request,
                             int *error_code);
void ADIOI_PMEM_IreadContig(ADIO_File fd, void *buf, int count,
                            MPI_Datatype datatype, int file_ptr_type,
                            ADIO_Offset offset, ADIO_Request *request,
                            int *error_code);
int ADIOI_PMEM_ReadDone(ADIO_Request *request, ADIO_Status *status,
                        int *error_code);
int ADIOI_PMEM_WriteDone(ADIO_Request *request, ADIO_Status *status,
                         int *error_code);
void ADIOI_PMEM_ReadComplete(ADIO_Request *request, ADIO_Status *status,
                             int *error_code);
void ADIOI_PMEM_WriteComplete(ADIO_Request *request, ADIO_Status *status,
                              int *error_code);
void ADIOI_PMEM_Fcntl(ADIO_File fd, int flag, ADIO_Fcntl_t *fcntl_struct,
                      int *error_code);
void ADIOI_PMEM_WriteStrided(ADIO_File fd, const void *buf, int count,
                             MPI_Datatype datatype, int file_ptr_type,
                             ADIO_Offset offset, ADIO_Status *status,
                             int *error_code);
void ADIOI_PMEM_ReadStrided(ADIO_File fd, void *buf, int count,
                            MPI_Datatype datatype, int file_ptr_type,
                            ADIO_Offset offset, ADIO_Status *status,
                            int *error_code);
void ADIOI_PMEM_WriteStridedColl(ADIO_File fd, const void *buf, int count,
                                 MPI_Datatype datatype, int file_ptr_type,
                                 ADIO_Offset offset, ADIO_Status *status,
                                 int *error_code);
void ADIOI_PMEM_ReadStridedColl(ADIO_File fd, void *buf, int count,
                                MPI_Datatype datatype, int file_ptr_type,
                                ADIO_Offset offset, ADIO_Status *status,
                                int *error_code);
void ADIOI_PMEM_IreadStrided(ADIO_File fd, void *buf, int count,
                             MPI_Datatype datatype, int file_ptr_type,
                             ADIO_Offset offset, ADIO_Request *request,
                             int *error_code);
void ADIOI_PMEM_IwriteStrided(ADIO_File fd, const void *buf, int count,
                              MPI_Datatype datatype, int file_ptr_type,
                              ADIO_Offset offset, ADIO_Request *request,
                              int *error_code);
void ADIOI_PMEM_Flush(ADIO_File fd, int *error_code);
void ADIOI_PMEM_Resize(ADIO_File fd, ADIO_Offset size, int *error_code);
ADIO_Offset ADIOI_PMEM_SeekIndividual(ADIO_File fd, ADIO_Offset offset,
                                      int whence, int *error_code);
void ADIOI_PMEM_SetInfo(ADIO_File fd, MPI_Info users_info, int *error_code);
void ADIOI_PMEM_Get_shared_fp(ADIO_File fd, int size, ADIO_Offset *shared_fp,
                              int *error_code);
void ADIOI_PMEM_Set_shared_fp(ADIO_File fd, ADIO_Offset offset,
                              int *error_code);
void ADIOI_PMEM_Delete(const char *filename, int *error_code);

#endif /* AD_PMEM_H_INCLUDED */
