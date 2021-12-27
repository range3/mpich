/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_pmem.h"
#include "adio_extern.h"
#include "adioi.h"

void ADIOI_PMEM_Fcntl(ADIO_File fd, int flag, ADIO_Fcntl_t *fcntl_struct,
                      int *error_code) {
  int myrank, nprocs;
  static char myname[] = "ADIOI_PMEM_FCNTL";

  *error_code = MPI_SUCCESS;

#ifdef DEBUG
  MPI_Comm_size(fd->comm, &nprocs);
  MPI_Comm_rank(fd->comm, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_PMEM_Fcntl called on %s\n", myrank, nprocs,
          fd->filename);
#endif

  switch (flag) {
  case ADIO_FCNTL_GET_FSIZE:
    ADIOI_GEN_Fcntl(fd, flag, fcntl_struct, error_code);
    break;

  case ADIO_FCNTL_SET_DISKSPACE:
  case ADIO_FCNTL_SET_ATOMICITY:
  default:
    *error_code = MPIO_Err_create_code(MPI_SUCCESS, MPIR_ERR_RECOVERABLE,
                                       myname, __LINE__, MPI_ERR_ARG, "**flag",
                                       "**flag %d", flag);
    return;
    /* --END ERROR HANDLING-- */
  }
}
