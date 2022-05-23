/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "ad_chfs.h"
#include "adioi.h"

void ADIOI_CHFS_Delete(const char* filename, int* error_code) {
  static char myname[] = "ADIOI_CHFS_Delete";

#ifdef DEBUG
  int myrank, nprocs;
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  FPRINTF(stdout, "[%d/%d] ADIOI_CHFS_Delete called on %s\n", myrank, nprocs,
          filename);
#endif

  *error_code = MPI_SUCCESS;

  ADIOI_CHFS_Init(error_code);
  if (*error_code != MPI_SUCCESS) {
    return;
  }

  if (chfs_unlink(filename)) {
    *error_code = ADIOI_Err_create_code(myname, filename, errno);
  }

  ADIOI_CHFS_Term(error_code);
}
